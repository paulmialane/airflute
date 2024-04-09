#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/gpio.h>
#include "usb_midi/usb_midi.h"
#include "gamme.h"



/************************ LEDs ************************/
/*
static struct gpio_dt_spec usb_midi_available_led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static struct gpio_dt_spec midi_rx_led = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static struct gpio_dt_spec midi_tx_led = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

static void init_leds()
{
	gpio_pin_configure_dt(&usb_midi_available_led, GPIO_OUTPUT_ACTIVE);
	gpio_pin_set_dt(&usb_midi_available_led, 0);

	gpio_pin_configure_dt(&midi_rx_led, GPIO_OUTPUT_ACTIVE);
	gpio_pin_set_dt(&midi_rx_led, 0);

	gpio_pin_configure_dt(&midi_tx_led, GPIO_OUTPUT_ACTIVE);
	gpio_pin_set_dt(&midi_tx_led, 0);
}

static void set_usb_midi_available_led(int is_available)
{
	gpio_pin_set_dt(&usb_midi_available_led, is_available);
}

static void flash_tx_led()
{
	gpio_pin_set_dt(&midi_tx_led, 1);
	k_work_cancel_delayable(&tx_led_off_work);
	k_work_schedule(&tx_led_off_work, Z_TIMEOUT_MS(LED_FLASH_DURATION_MS));
}

static void flash_rx_led()
{
	gpio_pin_set_dt(&midi_rx_led, 1);
	k_work_cancel_delayable(&rx_led_off_work);
	k_work_schedule(&rx_led_off_work, Z_TIMEOUT_MS(LED_FLASH_DURATION_MS));
}
*/

static struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

static void init_leds()
{
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	gpio_pin_set_dt(&led, 0);
}

static void set_usb_midi_available_led(int is_available)
{
	gpio_pin_set_dt(&led, is_available);
}

static void flash_tx_led()
{
	gpio_pin_set_dt(&led, 1);
	k_work_cancel_delayable(&tx_led_off_work);
	k_work_schedule(&tx_led_off_work, Z_TIMEOUT_MS(LED_FLASH_DURATION_MS));
}

static void flash_rx_led()
{
	gpio_pin_set_dt(&led, 1);
	k_work_cancel_delayable(&rx_led_off_work);
	k_work_schedule(&rx_led_off_work, Z_TIMEOUT_MS(LED_FLASH_DURATION_MS));
}

/****************** Work queue callbacks ******************/

void on_event_tx(struct k_work *item)
{
	if (sample_app_state.usb_midi_is_available && !sample_app_state.sysex_tx_in_progress) {
		uint8_t msg[3] = {sample_app_state.tx_note_off ? 0x90 : 0x80, TX_NOTE_NUMBER,
				  TX_NOTE_VELOCITY};
		usb_midi_tx(0, msg);
		sample_app_state.tx_note_off = !sample_app_state.tx_note_off;
	}
}



void on_button_press(struct k_work *item)
{
	play_note(item);
}

void nothing()
{
	
}

void on_rx_led_off(struct k_work *item)
{
	gpio_pin_set_dt(&led, 0);
}

void on_tx_led_off(struct k_work *item)
{
	gpio_pin_set_dt(&led, 0);
}

/************************ Buttons ************************/

static struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw0), gpios, {0});
static struct gpio_callback button_cb_data;

static void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	k_work_submit(&button_press_work);
}

static void init_button()
{
	__ASSERT_NO_MSG(device_is_ready(button.port));
	int ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	__ASSERT_NO_MSG(ret == 0);
	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	__ASSERT_NO_MSG(ret == 0);

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	ret = gpio_add_callback(button.port, &button_cb_data);
	__ASSERT_NO_MSG(ret == 0);
}

/****************** USB MIDI callbacks ******************/

static void midi_message_cb(uint8_t *bytes, uint8_t num_bytes, uint8_t cable_num)
{
	/* printk("rx non-sysex, cable %d: ", cable_num);
	for (int i = 0; i < num_bytes; i++) {
			printk("%02x ", bytes[i]);
	}
	printk("\n"); */
	flash_rx_led();
}

static void sysex_start_cb(uint8_t cable_num)
{
	sample_app_state.sysex_rx_data_byte_count = 0;
	flash_rx_led();
}

static void sysex_data_cb(uint8_t *data_bytes, uint8_t num_data_bytes, uint8_t cable_num)
{
	sample_app_state.sysex_rx_data_byte_count += num_data_bytes;
	flash_rx_led();
}

static void sysex_end_cb(uint8_t cable_num)
{
	// printk("rx sysex, cable %d: %d data bytes\n", cable_num,
	//       sample_app_state.sysex_rx_data_byte_count);
	flash_rx_led();
}

static void usb_midi_available_cb(int is_available)
{
	sample_app_state.usb_midi_is_available = is_available;
	set_usb_midi_available_led(is_available);
	if (is_available) {
		sample_app_state.tx_note_off = 0;
	}
}

static void usb_midi_tx_done_cb()
{
	if (sample_app_state.sysex_tx_in_progress) {
		uint8_t chunk[3] = {0, 0, 0};
		for (int i = 0; i < 3; i++) {
			if (sample_app_state.sysex_tx_data_byte_count ==
			    SYSEX_TX_MESSAGE_SIZE - 1) {
				chunk[i] = 0xf7;
			} else {
				chunk[i] = sample_app_state.sysex_tx_data_byte_count % 128;
			}
			sample_app_state.sysex_tx_data_byte_count++;
			if (sample_app_state.sysex_tx_data_byte_count == SYSEX_TX_MESSAGE_SIZE) {
				sample_app_state.sysex_tx_in_progress = 0;
				break;
			}
		}
		// printk("sending sysex chunk %02x %02x %02x\n", chunk[0], chunk[1], chunk[2]);
		flash_tx_led();
		usb_midi_tx(SYSEX_TX_CABLE_NUM, chunk);
	}
}

/****************** Sample app ******************/
void main(void)
{
	//printk("Begins main function\n");
	init_leds();
	init_button();
	//printk("Button initialized\n");
	k_work_init(&button_press_work, on_button_press);
	k_work_init(&event_tx_work, nothing);
	k_work_init_delayable(&rx_led_off_work, on_rx_led_off);
	k_work_init_delayable(&tx_led_off_work, on_tx_led_off);

	/* Register USB MIDI callbacks */
	struct usb_midi_cb_t callbacks = {.available_cb = usb_midi_available_cb,
					  .tx_done_cb = usb_midi_tx_done_cb,
					  .midi_message_cb = midi_message_cb,
					  .sysex_data_cb = sysex_data_cb,
					  .sysex_end_cb = sysex_end_cb,
					  .sysex_start_cb = sysex_start_cb};
	usb_midi_register_callbacks(&callbacks);

	/* Init USB */
	int enable_rc = usb_enable(NULL);
	printk("enable_rc == %d\n", enable_rc);
	__ASSERT(enable_rc == 0, "Failed to enable USB");
	// printk("USB initialized");

	/* Send MIDI messages periodically */
	while (1) {
		k_work_submit(&event_tx_work);
		k_msleep(TX_INTERVAL_MS);
	}
}

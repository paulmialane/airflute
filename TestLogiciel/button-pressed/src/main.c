/*
 Source : https://michaelangerer.dev/zephyr/2021/12/21/zephyr-basics-gpio.html
*/


#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

// To avoid oscillations, we wait for the button to stabilize
#define DEBOUNCE_TIMEOUT_MS 100

#define LED0 DT_ALIAS(led0)
#define SW0 DT_ALIAS(sw0)

const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0, gpios);
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0, gpios);

uint64_t last_time = 0;
struct gpio_callback button_cb_data;

void button_callback(const struct device *dev,
	struct gpio_callback *cb, uint32_t pins)
{
  int state = gpio_pin_get_dt(&button);
  uint64_t now = k_uptime_get();
  if ((now - last_time) > DEBOUNCE_TIMEOUT_MS)
  {
      if (state == 1){
        gpio_pin_toggle_dt(&led);
        printf("Key released\n");
      }
      else {
        gpio_pin_toggle_dt(&led);
        printf("Key pressed\n");
      }
  }
  last_time = now;
}


void main(void)
{

  // Configure the LED pin as output
  gpio_pin_configure_dt(&led, GPIO_OUTPUT);
  gpio_pin_toggle_dt(&led);
  // Configure the button pin as input
  gpio_pin_configure_dt(&button, GPIO_INPUT);

  // Configure the interrupt on button press (pin goes from high to low)
  gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_BOTH);

  // Setup the button press callback
  gpio_init_callback(&button_cb_data, button_callback, BIT(button.pin));
  gpio_add_callback(button.port, &button_cb_data);
}

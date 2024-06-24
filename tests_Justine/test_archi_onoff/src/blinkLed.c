#include "blinkLed.h"
#include <stdbool.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#include "basicFunctions.h"



///////////////////////////////blinking a led to be sure it's alive////////////////////////////////////////////////

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);



void blink(void)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led)) {
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
	}

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
		}

		led_state = !led_state;
		printk("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS);
	}
}

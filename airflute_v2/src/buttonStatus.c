/*
 * MIT License
 * 
 * Copyright (c) 2024 Justine OGER, Lucas MICHEL, Paul MIALANE
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdint.h>

#include "buttonStatus.h"


#define EXT_GPIO_CTRL_1 DT_NODELABEL(pcf8574_1)

// The structure to control the device
static const struct device * const ext_gpio_1 = DEVICE_DT_GET(EXT_GPIO_CTRL_1);

#define SLEEP_TIME_MS 1000

gpio_port_value_t val_gpio_1;

/* Sensors are not mapped in the same order on
 * the I2C bus and in the "natural" way.
 *
 * (Understand as : the 3rd hole on the flute isn't 
 * necessarily the 3rd sensor on the I2C bus)
 *
 * This Conversion Table offers a conveniant way
 * to convert from a view to another.
 */
static const uint8_t CT[8] = {
	(1UL<<4), // button 0
	(1UL<<5), // button 1
	(1UL<<6), // button 2
	(1UL<<0), // button 3
	(1UL<<7), // button 4
	(1UL<<1), // button 5
	(1UL<<2), // button 6
	(1UL<<3), // button 7
};

uint8_t conv(uint8_t in) {
	
	/* 
	 * Convert a combination of inputs (represented as an
	 * 8 bits unsigned integer) from the viex of the I2C bus
	 * to the natural way
	 */

	uint8_t out = 0;

	for(int i=0;i<8;i++) {
		if (in & (1UL << i)) out = out | CT[i];
	}
	return out;
}


uint8_t getCombination(void){

	/* 
	 * A function that returns the current combination of
	 * inputs on the flute
	 */

	gpio_port_get(ext_gpio_1, &val_gpio_1);
	
	// We invert the value because the button is active low
	return conv(~val_gpio_1);
}

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
// structure to control the device
static const struct device * const ext_gpio_1 = DEVICE_DT_GET(EXT_GPIO_CTRL_1);

#define SLEEP_TIME_MS 1000

gpio_port_value_t val_gpio_1;

static const unsigned int CT[8] = {
  (1UL<<4), // bouton 0(sur la carte)
  (1UL<<5), // bouton 1
  (1UL<<6), // bouton 2
  (1UL<<0), // bouton 3
  (1UL<<7), // bouton 4
  (1UL<<1), // bouton 5
  (1UL<<2), // bouton 6
  (1UL<<3), // bouton 7
};

unsigned int conv(unsigned int in){
  unsigned int out = 0;
  for(int i=0;i<8;i++){
    if (in & (1UL << i)) out = out | CT[i];
  }
  return out;
}


// A function to check if a button is pressed
uint8_t getCombination(void){
    gpio_port_get(ext_gpio_1, &val_gpio_1);
    // We invert the value because the button is active low
    return conv(~val_gpio_1);
}

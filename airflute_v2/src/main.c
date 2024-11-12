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

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>


#include "blinkLed.h"
#include "mainCycle.h"

#define STACKSIZE 1024
#define PRIORITY 4

void mainCycleInit(){
    // Here we initialize the main thread

    mainCycleThread();
}
 


// A thread to blink the led as it is usefull to know if the board is still alive
K_THREAD_DEFINE(blinkId, STACKSIZE, blink, NULL, NULL, NULL, PRIORITY, 0, 0);

K_THREAD_DEFINE(mainId, STACKSIZE, mainCycleInit, NULL, NULL, NULL, PRIORITY, 0, 0);

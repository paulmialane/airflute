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
#include <zephyr/drivers/adc.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "blowSensor.h"

static const struct adc_dt_spec adc_a0 =  ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);

// TODO : Declarer en static
const int sensor_threshold = 35;

// une seule donnée de 16bits
int16_t buf = 33;
struct adc_sequence sequence = {
	.buffer = &buf,
	/* buffer size in bytes, not number of samples */
	.buffer_size = sizeof(buf),
	//Optional
	//.calibrate = true,
};


int sensorInit(void) {
	int err1;

	// Configure the button pin as input
	if(adc_is_ready_dt(&adc_a0)) {
		printf("ADC is ready\n");
	} else {
		printf("ADC is NOT ready\n");
	}

	err1 = adc_channel_setup_dt(&adc_a0);
	if (err1 < 0) {
		printk("Error in ADC setup (%d)\n", err1);
		return err1;
	}

	err1 = adc_sequence_init_dt(&adc_a0, &sequence);
	if (err1 < 0) {
		printk("Error in the ADC sequence initialization (%d)\n", err1);
		return err1;
	}

	int mean=0;
	int length = 100;
	for (int i=0; i<length-1; i++) {
		err1 = adc_read(adc_a0.dev, &sequence);
		mean += buf; 
		k_msleep(10);
	}

	int standard = (int) mean/length;
	printk("Sensor initialized with standard value %i\n", standard);

	return standard;
}

uint8_t filter(int data, int standard) {
	//pas mal de travail ici
	if (data > standard - sensor_threshold) {
		return 0;
	} else if (standard - data - sensor_threshold > 127) {
		return 127;
	}

	return (uint8_t)(standard - data - sensor_threshold);
}







/// fonctions

bool blowingOnOff(int standard){

	int err;
	err = adc_read(adc_a0.dev, &sequence);

	int data_out = 0;
	if (err < 0) {
		// TODO : Handle error
		printk("Error reading the ADC (#%d)", err);
		return 0;
	}
	else {
		data_out = filter(buf, standard);
		return data_out;
	}
}

uint8_t blowingStrength(int standard){
	int err;
	err = adc_read(adc_a0.dev, &sequence);

	uint8_t data_out = 0;
	if (err < 0) {
		printk("Error reading the ADC (#%d)", err);
	}
	else {
		data_out = filter(buf, standard);
	}

	return data_out;
}

uint8_t strengthCategory(uint8_t strength){
	/*
	 * We define 3 different strengths, based on the value of strength between 0 ad 127.
	 */

	if (strength < 60){
		return 50;
	}
	else{
		if(strength < 100){
			return 90;
		}
		else{
			return 120;
		}
	}
}

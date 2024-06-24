#include "basicFunctions.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>
#include <stdio.h>

void copyArray(int tab1[8], int tab2[8]){
	for (int i = 0; i < 8; i++)
	{
		tab1[i] = tab2[8];
	}
	
}

bool isEqual(int combi1[8], int combi2[8]){
	for (int i = 0; i < 8; i++)
	{
		if (combi1[i] != combi2[i]){
			return false;
		} 
	}
	return true;
};

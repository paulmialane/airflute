#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <math.h>


static const struct adc_dt_spec adc_a0 =  ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);

int sensor_threshold = 10;

// une seule donnée de 16bits
int16_t buf = 33;
struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
		//Optional
		//.calibrate = true,
	};









int init_capteur(){
   
   int err1;
  // Configure the button pin as input
   if(adc_is_ready_dt(&adc_a0))
   printf("ADC is ready\n");
   else
   printf("ADC NOT ready\n");

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

   
   int moyenne=0;
   int longueur = 100;
   for (int i =0; i<longueur-1; i++){
      err1 = adc_read(adc_a0.dev, &sequence);
      moyenne += buf; 
      k_msleep(10);
   }
   return (int) moyenne/longueur;
}

int filtre(int data, int REFERENCE){//pas mal de travail ici
   if (data>REFERENCE-sensor_threshold){
      return 0;
   }
   else if (REFERENCE-data>127) {
      return 127;
   }
   
   return REFERENCE-data;
}

#define REFERENCE init_capteur()
//printf("capteur initialisé à %i\n",REFERENCE);







/// fonctions

int souffle_yes_no(void){
   int err;
  // Configure the button pin as input
   /*
   if(adc_is_ready_dt(&adc_a0))
   printf("ADC is ready\n");
   else
   printf("ADC NOT ready\n");

   err = adc_channel_setup_dt(&adc_a0);
   if (err < 0) {
      printk("Error in ADC setup (%d)\n", err);
      return err;
   }
   err = adc_sequence_init_dt(&adc_a0, &sequence);
   if (err < 0) {
      printk("Error in the ADC sequence initialization (%d)\n", err);
      return err;
   }
   */


   err = adc_read(adc_a0.dev, &sequence);
   int data = err;
   if (data>REFERENCE-sensor_threshold){
      return 0;
   }
   else{
      return 1;
   }
}


int souffle_force(void){

   int err;
  // Configure the button pin as input
  /*
   if(adc_is_ready_dt(&adc_a0))
   printf("ADC is ready\n");
   else
   printf("ADC NOT ready\n");

   err = adc_channel_setup_dt(&adc_a0);
   if (err < 0) {
      printk("Error in ADC setup (%d)\n", err);
      return err;
   }
   err = adc_sequence_init_dt(&adc_a0, &sequence);
   if (err < 0) {
      printk("Error in the ADC sequence initialization (%d)\n", err);
      return err;
   }
   */


   err = adc_read(adc_a0.dev, &sequence);
   int data_out=0;
   if (err < 0) {
         printk("Error reading the ADC (#%d)", err);
   }
   else {
         data_out = filtre(buf, REFERENCE);
   }
   return data_out;
}

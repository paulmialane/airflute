#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <math.h>


#define LED0 DT_ALIAS(led0)

const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0, gpios);

static const struct adc_dt_spec adc_a0 =  ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);

// une seule donnée de 16bits
int16_t buf = 33;
struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
		//Optional
		//.calibrate = true,
	};


int init_capteur(int err){
   int moyenne=0;
   int longueur = 100;
   for (int i =0; i<longueur-1; i++){
      err = adc_read(adc_a0.dev, &sequence);
      moyenne += buf; 
      k_msleep(10);
   }
   return (int) moyenne/longueur;
}

int filtre(int data, int reference){//pas mal de travail ici
   if (data>reference-50){
      return 0;
   }
   else if (reference-data>256) {
      return pow(256,2);
   }
   
   printk("données référence : %i   données capteur :%i \n",reference, data);
   return (int) pow(reference-data,2);
}

int main(void)
{
   int err;

  // Configure the LED pin as output
  gpio_pin_configure_dt(&led, GPIO_OUTPUT);
  gpio_pin_toggle_dt(&led);

  // Configure the button pin as input
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


   int reference = init_capteur(err);
   printf("capteur initialisé à %i\n",reference);
   int data_out;

   while (1){
      err = adc_read(adc_a0.dev, &sequence);
      if (err < 0) {
         printk("Error reading the ADC (#%d)", err);
         continue;
      } else {
         data_out = filtre(buf, reference);
         printk("%i\n", data_out);
      }
      k_msleep(500);
   }

}

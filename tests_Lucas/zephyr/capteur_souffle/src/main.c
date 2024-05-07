#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>


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

   while (1){
      err = adc_read(adc_a0.dev, &sequence);
      if (err < 0) {
         printk("Error reading the ADC (#%d)", err);
         continue;
      } else {
         printk("%i\n", buf);
      }
      k_msleep(100);
   }



}

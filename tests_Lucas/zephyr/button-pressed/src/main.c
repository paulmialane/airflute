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

  // Configure the LED pin as output
  gpio_pin_configure_dt(&led, GPIO_OUTPUT);
  gpio_pin_toggle_dt(&led);
  // Configure the button pin as input
  
  if(adc_is_ready_dt(&adc_a0))
    printf("ADC is ready\n");
  else
    printf("ADC NOT ready\n");

  adc_channel_setup_dt(&adc_a0);
  adc_sequence_init_dt(&adc_a0, &sequence);

while (1){
  adc_read(adc_a0.dev, &sequence);
  printf("%x\n",buf);
   k_msleep(5000);

}



}

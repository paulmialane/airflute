#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <math.h>


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define TRIG_BAT DT_ALIAS(sw0)

static const struct adc_dt_spec adc_a7 = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);

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
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&TRIG_BAT)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&TRIG_BAT, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	int err;
  // Configure the button pin as input
  if(adc_is_ready_dt(&adc_a7))
    printf("ADC is ready\n");
  else
    printf("ADC NOT ready\n");

   err = adc_channel_setup_dt(&adc_a7);
   if (err < 0) {
      printk("Error in ADC setup (%d)\n", err);
      return err;
   }
   err = adc_sequence_init_dt(&adc_a7, &sequence);
   if (err < 0) {
      printk("Error in the ADC sequence initialization (%d)\n", err);
      return err;
   }
	int data_out;


	while (1) {

		ret = gpio_pin_set_dt(&TRIG_BAT,1);
		if (ret < 0) {
			return 0;
		}

		err = adc_read(adc_a7.dev, &sequence);
      if (err < 0) {
         printk("Error reading the ADC (#%d)", err);
         continue;
      } else {
         data_out = buf;
         printk("%i\n", data_out);
      }
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

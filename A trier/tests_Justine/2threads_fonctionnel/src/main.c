#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7


#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(LED1_NODE, okay)
#error "Unsupported board: led1 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(LED2_NODE, okay)
#error "Unsupported board: led2 devicetree alias is not defined"
#endif

struct printk_data_t {
	void *fifo_reserved; /* 1st word reserved for use by fifo */
	uint32_t led;
	uint32_t n; 
    uint32_t sleep;
};




K_FIFO_DEFINE(red_fifo);
K_FIFO_DEFINE(green_fifo);
K_FIFO_DEFINE(blue_fifo);

struct led {
	struct gpio_dt_spec spec;
	uint8_t num;
};

static const struct led led0 = {
	.spec = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, {0}),
	.num = 0,
};

static const struct led led1 = {
	.spec = GPIO_DT_SPEC_GET_OR(LED1_NODE, gpios, {0}),
	.num = 1,
};

static const struct led led2 = {
	.spec = GPIO_DT_SPEC_GET_OR(LED1_NODE, gpios, {0}),
	.num = 2,
};


void blink_n_times(const struct led *led, uint32_t sleep_ms, int n,  uint32_t id)
{
    const struct gpio_dt_spec *spec = &led->spec;
	int cnt = 0;
	int ret;

    if (!device_is_ready(spec->port)) {
		printk("Error: %s device is not ready\n", spec->port->name);
		return;
	}

	ret = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure pin %d (LED '%d')\n",
			ret, spec->pin, led->num);
		return;
	}

    while (cnt<2*n) {
		gpio_pin_set(spec->port, spec->pin, cnt % 2);

		k_msleep(sleep_ms);
		cnt++;
	}
    
}




////////////////SENDING DATA SAYING TO BLINK A CERTAIN LED A CERTAIN NUMBER OF TIMES////////////////////////////


void send_red(void)
{
    int c = 1;

    while(1){
        struct printk_data_t tx_data = { .led = 0, .n = c, .sleep = 100};

		size_t size = sizeof(struct printk_data_t);
		char *mem_ptr = k_malloc(size);
		__ASSERT_NO_MSG(mem_ptr != 0);

		memcpy(mem_ptr, &tx_data, size);

		k_fifo_put(&red_fifo, mem_ptr);
        printk("Asked for led%d to blink %d times every %dms", tx_data.led, tx_data.n, tx_data.sleep);

        k_msleep(200*c+1000);
        c++;
    }
}

void send_green(void)
{
    int c = 1;

    while(1){
        struct printk_data_t tx_data = { .led = 1, .n = c, .sleep = 200};

		size_t size = sizeof(struct printk_data_t);
		char *mem_ptr = k_malloc(size);
		__ASSERT_NO_MSG(mem_ptr != 0);

		memcpy(mem_ptr, &tx_data, size);

		k_fifo_put(&green_fifo, mem_ptr);
        printk("Asked for led%d to blink %d times every %dms", tx_data.led, tx_data.n, tx_data.sleep);

        k_msleep(400*c+1000);
        c++;
    }
}

void send_blue(void)
{
    int c = 1;

    while(1){
        struct printk_data_t tx_data = { .led = 2, .n = c, .sleep = 500};

		size_t size = sizeof(struct printk_data_t);
		char *mem_ptr = k_malloc(size);
		__ASSERT_NO_MSG(mem_ptr != 0);

		memcpy(mem_ptr, &tx_data, size);

		k_fifo_put(&blue_fifo, mem_ptr);
        printk("Asked for led%d to blink %d times every %dms", tx_data.led, tx_data.n, tx_data.sleep);

        k_msleep(1000*c+1000);
        c++;
    }
}


///////////////ATTENDENT EN PERMANENCE DE RECEVOIR UNE COMMANDE POUR BLINK UNE LED N FOIS////////////////

void receive_red(void)
{
    while(1){
        struct printk_data_t *rx_data = k_fifo_get(&red_fifo, K_FOREVER);
        blink_n_times(&led0, rx_data->sleep, rx_data->n, 0);
        printk("led0 will blink %d times every %dms", rx_data->n, rx_data->sleep);
        k_free(rx_data);
    }
}

void receive_green(void)
{
    while(1){
        struct printk_data_t *rx_data = k_fifo_get(&green_fifo, K_FOREVER);
        blink_n_times(&led1, rx_data->sleep, rx_data->n, 1);
        printk("&led1 will blink %d times every %dms", rx_data->n, rx_data->sleep);
        k_free(rx_data);
    }
}

void receive_blue(void)
{
    while(1){
        struct printk_data_t *rx_data = k_fifo_get(&blue_fifo, K_FOREVER);
        blink_n_times(&led2, rx_data->sleep, rx_data->n, 1);
        printk("&led2 will blink %d times every %dms", rx_data->n, rx_data->sleep);
        k_free(rx_data);
    }
}


K_THREAD_DEFINE(send_red_id, STACKSIZE, send_red, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(send_green_id, STACKSIZE, send_green, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(send_blue_id, STACKSIZE, send_blue, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(receive_red_id, STACKSIZE, receive_red, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(receive_green_id, STACKSIZE, receive_green, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(receive_blue_id, STACKSIZE, receive_blue, NULL, NULL, NULL, PRIORITY, 0, 0);



/*K_THREAD_DEFINE(blink0_id, STACKSIZE, blink0, NULL, NULL, NULL,
		PRIORITY, 0, 0);
K_THREAD_DEFINE(blink1_id, STACKSIZE, blink1, NULL, NULL, NULL,
		PRIORITY, 0, 0);
K_THREAD_DEFINE(uart_out_id, STACKSIZE, uart_out, NULL, NULL, NULL,
		PRIORITY, 0, 0);*/

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define EXT_GPIO_CTRL_1 DT_NODELABEL(pcf8574_1)
//structure pour contrôler le device
static const struct device * const ext_gpio_1 = DEVICE_DT_GET(EXT_GPIO_CTRL_1);

#define SLEEP_TIME_MS 1000

int main(void) { //ça semble important que ça soit un int
    // pas vraiment besoin de configurer les pins en entrée pour le PCF8574,
    // il faut juste éviter d'écrire dessus
    // une variable pour récupérer la valeur du GPIO
    gpio_port_value_t val_gpio_1; // devrait être de type gpio_port_value_t (était un int)

    k_msleep(SLEEP_TIME_MS);
    printk("coucou\n");
    k_msleep(SLEEP_TIME_MS);

    if(!device_is_ready(ext_gpio_1)){
        printk("oups le contrôleur de GPIOS n'est pas prêt\n");
        k_msleep(SLEEP_TIME_MS);
    }
    printk("c'est prêt !!\n");
    k_msleep(SLEEP_TIME_MS);

    while (1){
            gpio_port_get(ext_gpio_1, &val_gpio_1); // ou gpio_pin_get(...) si on veut juste une des pins

            printk("--> %02x\n",val_gpio_1);
            k_msleep(SLEEP_TIME_MS);
    }
}

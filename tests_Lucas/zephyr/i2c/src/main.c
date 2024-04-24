


#include <stdio.h>
#include <zephyr/drivers/gpio.h>

#define EXT_GPIO_CTRL_1 DT_NODELABEL(i2c1) 
//structure pour contrôler le device

#define ext_gpio_1 DEVICE_DT_GET(EXT_GPIO_CTRL_1)
//static const struct device * const ext_gpio_1 = DEVICE_DT_GET(EXT_GPIO_CTRL_1);


void main(void) {

    if(!device_is_ready(ext_gpio_1)){
        printf("oups le contrôleur de GPIOS n'est pas prêt\n");
        //return ;
    }
    // pas vraiment besoin de configurer les pins en entrée pour le PCF8574,
    // il faut juste éviter d'écrire dessus
    // une variable pour récupérer la valeur du GPIO 
    int val_gpio_1; // devrait être de type gpio_port_value_t

    gpio_port_get(ext_gpio_1, &val_gpio_1); // ou gpio_pin_get(...) si on veut juste une des pins

    printf("--> %02x\n",val_gpio_1);
}
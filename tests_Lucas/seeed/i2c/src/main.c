#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define EXT_GPIO_CTRL_1 DT_NODELABEL(pcf8574_1)
//structure pour contrôler le device
static const struct device * const ext_gpio_1 = DEVICE_DT_GET(EXT_GPIO_CTRL_1);

#define SLEEP_TIME_MS 1000

int getBit(int n, int k)
{
    return (n & (1<<k)) != 0;
}

gpio_port_value_t val_gpio_1;


//fonction pour le traitement
int *is_pressed(void){
    gpio_port_get(ext_gpio_1, &val_gpio_1);

    return (int*) &val_gpio_1;
}

//main (à enlever pour l'intégration)

int main(void) { //ça semble important que ça soit un int
    // pas vraiment besoin de configurer les pins en entrée pour le PCF8574,
    // il faut juste éviter d'écrire dessus
    // une variable pour récupérer la valeur du GPIO
    gpio_port_value_t val_gpio_1; // devrait être de type gpio_port_value_t (était un int)
    bool buttons_pressed[8];

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

            //conversion en liste de booléens
            for (int k = 7; k>=0; k--){
                buttons_pressed[k]=getBit(val_gpio_1,k);
                //printk(" %d ;",getBit(val_gpio_1,k));
            }


            //printk("--> %02x\n",val_gpio_1);
            
            //partie print
            for (int k = 0; k<=7; k++){
                printk(" %d ;",buttons_pressed[k]);
            }
            printk(" %x", val_gpio_1);

            
            k_msleep(SLEEP_TIME_MS);
    }
}

# Simple Hello World

`prj.conf` contient une option spécifique pour la carte XIAO (retarde le démarrage).

## Pour compiler

```sh
export ZEPHY_BASE=<l'endroit où vous avez récupéré le dépôt de zephyr>
west build -b xiao_ble_sense
```

>TODO: modifier les paramètre du projet pour y préciser la carte 


## Pour programmer

1. double click sur le bouton reset de la carte
2. elle apparait comme "clé usb"
3. copier fichier uf2 dans le répertoire build/zephyr dans la clé usb

```sh
cp ./build/zephyr/zephyr.uf2 /media/paulm/XIAO-SENSE
```

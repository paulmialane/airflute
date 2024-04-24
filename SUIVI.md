# Compte rendu séances

### Séance 14/02

- Création du dépot git et d'une branche 'main'
- Création du fichier 'CompteRendu.md' où chacun peut écrire ce qu'il a fait durant la séance, afin d'avoir un suivi du projet pour les encadrants, mais surtout pour nous.
- Nous avons listé les tâches à faire, ou tout du moins les grands axes auxquels il faudra s'attaquer.
- Répartition des taches avec des responsables de tâche.
- Première ébauche de planning

Pour la prochaine séance, nous souhaiterions avoir commencé à regarder les différents aspects du projet pour pouvoir estimer la durée et le travail nécessaires et donc préciser notre planning.

### Séance 28/02
- Découverte du capteur infrarouge et test de démonstration 
- Découverte du convertisseur I2C 
- Plannification 
- Hello world sur la carte et configuration de l'ordi de Paul pour programmer la carte
- Avancement des recherches sur le MIDI et comment l'envoyer/recevoir (cf. fichier Protocole_Midi.md)
- Seule bibliotheque MIDI par USB trouvee : zephyr-usb-midi

### Séance du 13/03

- Premiers prototypes de bec de flute a l'imprimante 3D
- Recherche de la "bonne" résistance coté phototransistor pour détecter la présence d'un doigt
- Montage avec soudure du capteur au FabLab
- Presque capable de detecter le capteur bouché sur la XIAO

### Séance du 20/03

- Programme de test fonctionnel, on peut detecter un capteur bouché via pin GPIO
- Première prise en main de l'I2C
- Recherche d'une bibliotheque pour le MIDI via USB
- Test de la bibliotheque zephyr-usb-midi, problemes detectes

### Séance du 26/03

- Tentatives de debug de la bibliotheque zephyr-usb-midi pour notre carte
- Premiers programmes sur la carte STM32 sur laquelle zephyr-usb-midi fonctionne ; on arrive a etre detecte en tant que MIDI device, a envoyer la meme note en boucle

### Séance du 02/04

- Sur la carte STM32 : envoi d'une note declenche par un bouton, plusieurs methodes de decision ; tentative d'utilisation d'un bouton optique pour déclencher l'envoi : echec (problemes d'overlay)
- Tentatives de debug de la bibliotheque zephyr-usb-midi pour notre carte

### Séance du 09/04

- Essai de la bibliotheque zephyr-ble-midi pour la carte XIAO BLE sense, problemes
- Encore des essais d'utilisation du bouton optique pour declencher l'envoi de notes sur la carte STM32 
- Renseignement sur les threads et messages queues pour l'organisation du code general




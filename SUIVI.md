# Compte rendu séances

### Séance 14/02

- Création du dépot git et d'une branche 'main'
- Création du fichier 'CompteRendu.md' où chacun peut écrire ce qu'il a fait durant la séance, afin d'avoir un suivi du projet pour les encadrants, mais surtout pour nous.
- Nous avons listé les tâches à faire, ou tout du moins les grands axes auxquels il faudra s'attaquer.
- Répartition des taches avec des responsables de tâche.
- Première ébauche de planning

Pour la prochaine séance, nous souhaiterions avoir commencé à regarder les différents aspects du projet pour pouvoir estimer la durée et le travail nécessaires et donc préciser notre planning.

### Séance 28/02
- Lucas : Découverte du capteur infrarouge et test de démonstration 
- Lucas : Découverte du convertisseur I2C 
- Paul : Plannification 
- Paul : Hello world sur la carte et configuration de l'ordi de Paul pour programmer la carte
- Justine : Avancement des recherches sur le MIDI et comment l'envoyer/recevoir (cf. fichier Protocole_Midi.md)
- Justine : Seule bibliotheque MIDI par USB trouvee : zephyr-usb-midi

### Séance du 13/03

- Lucas : Premiers prototypes de bec de flute a l'imprimante 3D
- Paul : Recherche de la "bonne" résistance coté phototransistor pour détecter la présence d'un doigt
- Lucas : Montage avec soudure du capteur au FabLab
- Paul : Presque capable de detecter le capteur bouché sur la XIAO

### Séance du 20/03

- Lucas : Programme de test fonctionnel, on peut detecter un capteur bouché via pin GPIO
- Lucas : Première prise en main de l'I2C
- Justine : Recherche d'une bibliotheque pour le MIDI via USB
- Paul : Test de la bibliotheque zephyr-usb-midi, problemes detectes

### Séance du 26/03

- Paul : Tentatives de debug de la bibliotheque zephyr-usb-midi pour notre carte
- Justine : Premiers programmes sur la carte STM32 sur laquelle zephyr-usb-midi fonctionne ; on arrive a etre detecte en tant que MIDI device, a envoyer la meme note en boucle
- Lucas : Fabrication de plusieurs capteurs optiques pour pourvoir faire des tests en parallèle

### Séance du 02/04

- Justine : Sur la carte STM32 : envoi d'une note declenche par un bouton, plusieurs methodes de decision ; tentative d'utilisation d'un bouton optique pour déclencher l'envoi : echec (problemes d'overlay)
- Paul : Tentatives de debug de la bibliotheque zephyr-usb-midi pour notre carte
- Lucas : Conception de bec en PVC et du module de détection du souffle

### Séance du 09/04

- Paul : Essai de la bibliotheque zephyr-ble-midi pour la carte XIAO BLE sense, problemes
- Encore des essais d'utilisation du bouton optique pour declencher l'envoi de notes sur la carte STM32 
- Justine : Renseignement sur les threads et messages queues pour l'organisation du code general
- Lucas : Tests sur le module de détection du souffle
- Lucas : Programmation d’un premier fichier d’essai pour l’I2C ; compréhension de la forme d’un devicetree (overlay et config)

### Séance du 24/04

- Justine : Code pour me familiariser avec les threads + les FIFOs pour communiquer entre les threads
- Lucas : Code pour la lecture en I2C fonctionnel

### Séance du 02/05

- Lucas : Gestion des valeur obtenues en I2C et commencement de la lecture analogique (capteur de souffle)
- Justine : jouer un accord en MIDI Bluetooth sur téléphone avec applications adaptées

### Séance du 07/05

- Lucas : Mise en forme des données capteurs (courbes pour calibrer le souffle)
- Justine : logique de communication entre les différentes parties du projet

### Séance du 15/05

- Lucas : calibration du capteur de souffle et filtrage des données
- Justine : écriture du programme global intégrant toutes les parties du projet

### Séance du 22/05

- Lucas : modélisation d'un bec qui peut intégrer le capteur de souffle
- Lucas : ajout des foncions utilisées pour la détection de souffle et le choix des notes
- Justine : écriture du programme global intégrant toutes les parties du projet
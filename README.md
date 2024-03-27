# AirFlute

### Documents

- [Compte rendu des séances](SUIVI.md)
- [Convention de codage](documents/Conventions_Codage.md)

### TODO :
- [ ] Regarder le protocole MIDI
- [ ] Savoir s'annoncer comme MIDI Device par USB 
    * [ ] outil éventuel [librairie zephyr-usb-midi](https://github.com/stuffmatic/zephyr-usb-midi) sur git
- [ ] Regarder comment fonctionne le BLE (Bluetooth Low Energy)
- [ ] Fonctionnement MIDI par BLE
- [ ] Gros programme qui boucle, récupère l'état des capteurs, envoie le MIDI "corrrespondant"
    * [ ] sous-programme qui décide de la note à envoyer selon les capteurs
- [x] Detecter un capteur bouché
- [ ] Développer un capteur de pression
- [x] Zephyr RTOS
- [ ] Xiao BLE Sense
- [x] Faire le planning
- [ ] Circuit imprimé ?
    * [ ] outil éventuel [kicad](https://www.kicad.org/)
- [ ] Comment brancher les capteurs sur le bus I2C.
- [ ] Récupérer les trous bouchés sous forme de liste sur la XIAO
- [ ] Logiciel MIDI to Sound (Abbleton, MuseScore, etc.)
- [x] Conventions programmation.

### Planning :
| Date  | À faire avant la séance  | À faire pendant la séance |
| :- |:-| :-|
| 28 fevrier |Découverte du rôle (savoir où on va) : <ul> <li> [x] Paul</li> <li> [x] Justine</li><li> [x] Lucas</li> </ul> | <ul><li> [ ] Planning fini</li></ul> |
| 13 mars |  <ul> <li> [x] Zephyr correctement installé chez tout le monde</li> </ul> | <ul> Justine absente !!😥  <li> [x] Détecter le fait qu'un capteur est bouché</li></ul> |
| 20 mars | <ul> <li> [ ] Savoir présenter la carte come USB MIDI Device à un ordi </li> <li> [x] Regarder comment brancher les capteurs de doigts en utilisant un bus I2C </li> </ul> | <ul> <li> [x] Premier prototype pour le souffle imprimé </li> </ul> |
| 26 mars | <ul> <li> [ ] Savoir envoyer une note si un capteur est bouché (ON/OFF) </li> </ul> | <ul> <li> [ ] Création et envoi d'une note en MIDI en fonction des capteurs bouchés </li> </ul> |
| 2 avril |  | <ul> <li> évaluation intermédiaire? (être capable de jouer 1 note si un capteur est bouché ?) </li> </ul> |
| 9 avril |  | <ul> <li> évaluation intermédiaire? (être capable de jouer 1 note si un capteur est bouché ?) </li> </ul> |
| 24 avril | <ul> <li> [ ] Savoir gérer le cas d'une combinaison incorrecte de capteurs </li> </ul> |  | 
| 2 mai | | <ul><li> [ ] Récuperer via le bus I2C une combinaison de touches </li></ul> |
| 7 mai | <ul> <li> [ ] Combiner la récupération des capteurs et le code générateur de MIDI selon la combinaison </li> </ul>|<ul> <li> [ ] Prototype fonctionnel pour la détection du souffle (envoi de valeurs tout ou rien avec le souffle) </li></ul> |
| 15 mai |  | <ul> <li> [ ] Envoyer une note quand on détecte du souffle *et* selon les capteurs bouchés </li> </ul> | 
| 22 mai |  |  |
| 29 mai |  | <ul> <li> [ ] Prototype fonctionnel pour la détection du souffle (envoi de valeurs linéaires avec le souffle) </li></ul>  |
| 5 juin |  | <ul> <li> [ ] Réglage volume d'une note en fonction du souffle </li> </ul> |
| 12 juin |  |  <ul> <li> [ ] Modèle 3d complet </li></ul> |
| 24-28 juin | <ul> <li> [ ] Tous les modules sont prêts </li></ul>  | <ul> <li> [ ] Intégration des parties étudiées </li></ul> |

### Repartition

| Tache | Responsable | Condition de validation |
| :- |:-| :- |
| Savoir envoyer un flux MIDI en USB | Justine | Etre détecté comme un MIDI via USB |
| Savoir envoyer un flux MIDI en BLE | Justine | Etre détecté comme un MIDI via BLE |
| Jouer en fonction des capteurs bouchés | Justine/Paul | Etant donnée une configuration de True/False décrivant l'état des capteurs, pouvoir faire jouer une note à un ordinateur à volume sonore fixé |
| Jouer en fonction du souffle | Justine/Paul | Déclenchement de l'envoi d'une note si on souffle et si les capteurs sont bouchés, puis envoyer une note plus ou moins forte selon la "force" du souffle |
| Tester les capteurs | Lucas | Les capteurs envoient des données cohérentes (distance du doigt acceptable) |
| Zephyr RTOS | Paul | Tout le monde arrive à compiler le 'Hello World' et le mettre sur la XIAO |
| I2C | Lucas/Paul | Etre capable de récupérer un doigté pour flute via I2C (depuis la XIAO, pouvoir dire "tel capteur est recouvert/n'est pas recouvert") |
| Modèles 3D | Lucas | Avoir une pièce en 3d utilisable (tous les capteurs, circuits et cables passent) |
| Capteur Souffle | Lucas | Réussir à utiliser le capteur de distance pour savoir si on souffle (puis plus tard la puissance du souffle) |
| Capteurs pour les doigts | Paul | Detecter depuis la XIAO si le capteur est recouvert ou non |

### Remarques:

- On voit mal le lien entre les tâches et les activités concrètes des séances
- Il faut déterminer des dates limites et des livrables concrèts (logiciel, documents, ...) pour les tâches
- Est-ce que les tâches sont séquentielles ou vous pouvez paralleliser des choses ?
- Quelles sont les dépendances entre les tâches ?
- Est-ce qu'il y a des tâches optionnelles ? 
- Détaillez / découpez la partie MIDI BLE en plusieurs objectifs (jouer une note en boucle, jouer une note déclenchée par un bouton, ...)
- Rajoutez des objectifs optionnels (effets en utilisant les autres capteurs de la carte - micro / accéléromètre, ...)

# Test capteur


Ce test permet de montrer que le capteur fonctionne. 
Lorsque l'on pose le doigt sur le capteur, la LED s'allume, et on obtient un message en console. 
Lorsque l'on retire le doigt, la LED s'éteind et on affiche également un message. 

## Montage     

- La résistance coté photodiode : 1k
- La résistance coté phototransistor : 47k
- V_out branché sur le PIN 29 (D3)

(La configuration du GPIO se fait à la main, car je n'ai pas réussi à faire fonctionner
le programme en utilisant DeviceTree et le fichier en .overlay)



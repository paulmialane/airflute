# Convention de programmation

Nous ne savons à quel point nous allons travailler sur le code des autres et nous n'avons pas forcément les mêmes conventions. On a eu des problèmes sur la lecture du code lors d'ARTEFACT. Donc on se fixe les conventions suivantes:

1. Tous les noms de variables/fonctions/structures (etc.) sont en anglais. Tous les commentaires sont en anglais.

2. On utilise la convention snake_case (recommendée en C).

3. Variables: Les noms des variables décrivent ce que fait la variable. Si le nom est trop long, on se limite, mais on met un petit commentaire pour décrire la variable. Si la valeur de la variable a une unité, on met l'unité en fin de nom de variable (`int length_cm = 5;`).

4. Variables sur la pile: tout en minuscule.

5. Variables globales : on met le préfixe `g_` (ex. `int g_length;`).

6. Constantes : ALL CAPS.

7. Parenthèses et accolades :
```C
if (condition) {
    ...
} else {
    ...
}

while (condition) {
    ...
}
 ```

8. Pointeurs : On met l'astérisque devant le nom du pointeur (`int *pointer_to_int;` et pas `int* pointer_to_int;`)

9. Fonctions : On met un commentaire pour décrire ce que fait la fonction de la manière suivante :

```C
int sum(int a, int b) {
    /*
    This function returns the sum two ints.

    Inputs : 
        - int a : The first number
        - int b : The second number

    Output :
        - int : a + b
    */

    return a + b
}
```

En cas de doute : [C Coding Standard](https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html)
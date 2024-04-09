Pour créer et définir un thread : 

K_THREAD_DEFINE(name, stack_size, entry, p1, p2, p3, prio, options, delay)



        name – Name of the thread.

        stack_size – Stack size in bytes.

        entry – Thread entry function.

        p1 – 1st entry point parameter.

        p2 – 2nd entry point parameter.

        p3 – 3rd entry point parameter.

        prio – Thread priority.

        options – Thread options.

        delay – Scheduling delay (in milliseconds), zero for no delay.

Pour obtenir l'id du thread : 

``` 
 extern const k_tid_t <name>;
```


Il faut 
```
 #include <thread.h>
```

Pour transmettre des messages entre les threads, on peut utiliser des Message Queues : FIFO, options disponibles pour attendre un certain temps (timeout) ou jusqu'à ce qu'un message soit disponible à la lecture (K_FOREVER). On peut même lire le message sans l'enlever de la Queue.
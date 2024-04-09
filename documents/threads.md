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

Pour transmettre des messages entre les threads, on peut utiliser des Message Queues (fonctionnement en FIFO)

Pour define et initialize une Message Queue rapidement : 
```
K_MSGQ_DEFINE(q_name, q_msg_size, q_max_msgs, q_align)
```

Ensuite on y accède avec : 
```
extern struct k_msgq <name>;
```

Pour écrire : 
```
int k_msgq_put(struct k_msgq *msgq, const void *data, k_timeout_t timeout)
```
Renvoie 0 quand le message a bien été envoyé.

Pour lire : 
```
k_msgq_get(struct k_msgq *msgq, void *data, k_timeout_t timeout)
```
timeout K_FOREVER pour attendre indéfiniment tant qu'il n'y a pas de message 
Renvoie 0 quand le message a bien été lu.
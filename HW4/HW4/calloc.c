#include<pthread.h>
#include "malloc.h"


void *calloc(size_t nmemb, size_t size)
{
        pthread_mutex_lock(&mutex);
        size_t *new;
        pthread_mutex_unlock(&mutex);
        new = malloc(nmemb * size);
        pthread_mutex_lock(&mutex);
        if(new)
                memset(new,0,nmemb);
        pthread_mutex_unlock(&mutex);
        return(new);
}


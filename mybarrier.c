#include <stdlib.h>
#include "mybarrier.h"
#include <stdio.h>

mybarrier_t * mybarrier_init(unsigned int count) 
{
    if (count <= 0){
        return NULL;  
    }
    mybarrier_t * barrier = malloc(sizeof(*barrier));
    if (NULL == barrier){
        return NULL;
    } 
    barrier->count = count;
    barrier->original_count = count;
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
    return barrier;
}

int mybarrier_destroy(mybarrier_t * barrier) 
{
    if (barrier == NULL) {
        return -1;  
    }
    int ret = 0;  
    if (pthread_mutex_destroy(&barrier->mutex) != 0) {
        ret = -1;
    }
    if (pthread_cond_destroy(&barrier->cond) != 0) {
        ret = -1;
    }
    free(barrier);
    return ret;  
}

int mybarrier_wait(mybarrier_t * barrier) 
{
    int ret = 0; 
    if (barrier == NULL) {
        return -1;
    }
    ret = pthread_mutex_lock(&barrier->mutex);
    if (ret != 0) {
        return ret; 
    }
    barrier->count--;
    if (barrier->count > 0) {
        ret = pthread_cond_wait(&barrier->cond, &barrier->mutex);
    } else {
        barrier->count = barrier->original_count;
        ret = pthread_cond_broadcast(&barrier->cond);
    }
    int unlock_ret = pthread_mutex_unlock(&barrier->mutex);
    if (unlock_ret != 0) {
        return unlock_ret;
    }
    return ret;  
}

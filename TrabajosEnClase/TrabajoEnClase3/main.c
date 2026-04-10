#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

typedef enum{
    THINKING,
    HUNGRY,
    EATING
} State;

State state[N];

pthread_mutex_t mutex;
pthread_cond_t cond[N];

int LEFT(int i){
    return (i + N - 1) % N;
}

int RIGHT(int i){
    return (i + 1) % N;
}

void test(int i){

    if(state[i] == HUNGRY &&
       state[LEFT(i)] != EATING &&
       state[RIGHT(i)] != EATING){

        state[i] = EATING;
        pthread_cond_signal(&cond[i]);
    }
}

void get_forks(int i){

    pthread_mutex_lock(&mutex);

    state[i] = HUNGRY;

    test(i);

    while(state[i] != EATING){
        pthread_cond_wait(&cond[i], &mutex);
    }
    printf("Agarrando cubiertos desde el hilo: %i\n", i);
    pthread_mutex_unlock(&mutex);
}

void put_forks(int i){
    
    printf("Dejando cubiertos desde el hilo: %i\n", i);
    pthread_mutex_lock(&mutex);

    state[i] = THINKING;

    test(LEFT(i));
    test(RIGHT(i));

    pthread_mutex_unlock(&mutex);
}

void think(int i){
    printf("Pensando desde el hilo: %i\n", i);
    sleep(1);
}

void eat(int i){
    printf("comiendo desde el hilo: %i\n", i);
    sleep(2);
}

void* philosopher(void* num){

    int i = *(int*)num;

    while(1){
        think(i);
        get_forks(i);
        eat(i);
        put_forks(i);
    }
}

int main(){

    pthread_t thread[N];
    int id[N];

    pthread_mutex_init(&mutex,NULL);

    for(int i=0;i<N;i++){
        pthread_cond_init(&cond[i],NULL);
        state[i] = THINKING;
    }

    for(int i=0;i<N;i++){
        id[i] = i;
        pthread_create(&thread[i],NULL,philosopher,&id[i]);
    }

    for(int i=0;i<N;i++){
        pthread_join(thread[i],NULL);
    }

    return 0;
}

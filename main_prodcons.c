#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>
#include "process_sensor_prodcons.h"

#define NUM_PRODUTORAS 2
#define NUM_CONSUMIDORAS 2
#define TOTAL_DADOS 20

sem_t sem_prod;
sem_t sem_consum;

struct timeval start_time, end_time;

void medir_tempo_execucao() {
    gettimeofday(&end_time, NULL);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("\nTempo total de execução: %.6f segundos\n", elapsed_time);
}

int main() {
    gettimeofday(&start_time, NULL);

    sem_init(&sem_prod, 0, MAX_FILA);
    sem_init(&sem_consum, 0, 0);

    FilaDados fila;
    fila.inicio = fila.fim = 0;
    pthread_mutex_init(&fila.mutex, NULL);

    pthread_t produtores[NUM_PRODUTORAS];
    pthread_t consumidores[NUM_CONSUMIDORAS];
    ArgsProdutor args_produtores[NUM_PRODUTORAS];

    int dados_por_produtora = TOTAL_DADOS / NUM_PRODUTORAS;


    for (int i = 0; i < NUM_PRODUTORAS; i++) {
        args_produtores[i].fila = &fila;
        args_produtores[i].start_index = i * dados_por_produtora;
        args_produtores[i].total_gerar = dados_por_produtora;

        pthread_create(&produtores[i], NULL, produtor, &args_produtores[i]);
    }


    for (int i = 0; i < NUM_CONSUMIDORAS; i++) {
        pthread_create(&consumidores[i], NULL, consumidor, &fila);
    }

    for (int i = 0; i < NUM_PRODUTORAS; i++) pthread_join(produtores[i], NULL);
    for (int i = 0; i < NUM_CONSUMIDORAS; i++) pthread_join(consumidores[i], NULL);

    sem_destroy(&sem_prod);
    sem_destroy(&sem_consum);
    pthread_mutex_destroy(&fila.mutex);

    medir_tempo_execucao();
    return 0;
}

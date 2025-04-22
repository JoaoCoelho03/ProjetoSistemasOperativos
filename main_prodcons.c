#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h> // Para gettimeofday
#include "process_sensor_prodcons.h"

#define MAX_FILES 100

sem_t sem_prod;  // Semáforo para o produtor
sem_t sem_consum; // Semáforo para o consumidor

struct timeval start_time, end_time;  // Variáveis para medir o tempo de execução

// Função para medir o tempo total
void medir_tempo_execucao() {
    gettimeofday(&end_time, NULL);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("\nTempo total de execução: %.6f segundos\n", elapsed_time);
}

int main() {
    gettimeofday(&start_time, NULL);  // Começa o temporizador

    // Inicializa semáforos
    sem_init(&sem_prod, 0, MAX_FILA);  // Semáforo do produtor
    sem_init(&sem_consum, 0, 0);        // Semáforo do consumidor

    FilaDados fila;
    fila.inicio = fila.fim = 0;
    pthread_mutex_init(&fila.mutex, NULL);

    pthread_t t_produtor, t_consumidor;

    // Cria threads produtoras e consumidoras
    pthread_create(&t_produtor, NULL, produtor, &fila);
    pthread_create(&t_consumidor, NULL, consumidor, &fila);

    // Espera que ambas as threads terminem
    pthread_join(t_produtor, NULL);
    pthread_join(t_consumidor, NULL);

    // Finaliza semáforos e mutex
    sem_destroy(&sem_prod);
    sem_destroy(&sem_consum);
    pthread_mutex_destroy(&fila.mutex);

    // Mede o tempo total de execução
    medir_tempo_execucao();

    return 0;
}

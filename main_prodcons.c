#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "process_sensor_prodcons.h"
#include <semaphore.h>  // Para semáforos
#include <unistd.h>     // Para usleep()

sem_t sem_prod;   // Semáforo para o produtor
sem_t sem_consum; // Semáforo para o consumidor


int main() {
    // Inicializa semáforos
    sem_init(&sem_prod, 0, MAX_FILA);  // Inicializa o semáforo do produtor com capacidade máxima
    sem_init(&sem_consum, 0, 0);        // Inicializa o semáforo do consumidor como 0 (aguarda)

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

    return 0;
}

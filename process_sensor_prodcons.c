#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "process_sensor_prodcons.h"
#include <semaphore.h>  // Para semáforos
#include <unistd.h>     // Para usleep()

extern sem_t sem_prod;   // Declaração do semáforo para o produtor
extern sem_t sem_consum; // Declaração do semáforo para o consumidor


void adicionar_fila(FilaDados *fila, DadoSensor dado) {
    fila->fila[fila->fim] = dado;
    fila->fim = (fila->fim + 1) % MAX_FILA;
}

DadoSensor remover_fila(FilaDados *fila) {
    DadoSensor dado = fila->fila[fila->inicio];
    fila->inicio = (fila->inicio + 1) % MAX_FILA;
    return dado;
}

void *produtor(void *arg) {
    FilaDados *fila = (FilaDados *)arg;
    for (int i = 0; i < 10; i++) {  // Gerando 10 dados para simular sensores
        DadoSensor dado;
        snprintf(dado.nome_sensor, sizeof(dado.nome_sensor), "Sensor_%d", i);
        dado.media = rand() % 100; // Exemplo de dado aleatório
        dado.horas_fora = rand() % 10;

        sem_wait(&sem_prod);  // Aguarda espaço disponível na fila

        pthread_mutex_lock(&fila->mutex); // Protege acesso à fila
        adicionar_fila(fila, dado);
        pthread_mutex_unlock(&fila->mutex); // Libera acesso à fila

        sem_post(&sem_consum);  // Libera o consumidor para processar

        usleep(500000);  // Simula tempo de geração de dados
    }
    return NULL;
}

void *consumidor(void *arg) {
    FilaDados *fila = (FilaDados *)arg;
    for (int i = 0; i < 10; i++) {  // Consumindo 10 dados
        sem_wait(&sem_consum);  // Aguarda dados disponíveis na fila

        pthread_mutex_lock(&fila->mutex); // Protege acesso à fila
        DadoSensor dado = remover_fila(fila);
        pthread_mutex_unlock(&fila->mutex); // Libera acesso à fila

        // Simula o processamento dos dados
        printf("Consumidor processando: %s - Média: %.2f - Horas fora: %d\n",
            dado.nome_sensor, dado.media, dado.horas_fora);

        usleep(500000);  // Simula o tempo de processamento
    }
    return NULL;
}

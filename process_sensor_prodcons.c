#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "process_sensor_prodcons.h"
#include <unistd.h>

extern sem_t sem_prod;
extern sem_t sem_consum;

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
    ArgsProdutor *args = (ArgsProdutor *)arg;

    for (int i = 0; i < args->total_gerar; i++) {
        DadoSensor dado;
        snprintf(dado.nome_sensor, sizeof(dado.nome_sensor), "Sensor_%d", args->start_index + i);
        dado.media = rand() % 100;
        dado.horas_fora = rand() % 10;

        sem_wait(&sem_prod);
        pthread_mutex_lock(&args->fila->mutex);
        adicionar_fila(args->fila, dado);
        pthread_mutex_unlock(&args->fila->mutex);
        sem_post(&sem_consum);

        printf("Produtora gerou: %s\n", dado.nome_sensor);
        usleep(100000);
    }

    return NULL;
}


void *consumidor(void *arg) {
    FilaDados *fila = (FilaDados *)arg;

    FILE *saida = fopen("relatorio_prodcons.txt", "a");
    if (!saida) {
        perror("Erro ao abrir relatorio_prodcons.txt");
        pthread_exit(NULL);
    }

    for (int i = 0; i < TOTAL_DADOS / NUM_CONSUMIDORAS; i++) {
        sem_wait(&sem_consum);
        pthread_mutex_lock(&fila->mutex);
        DadoSensor dado = remover_fila(fila);
        pthread_mutex_unlock(&fila->mutex);
        sem_post(&sem_prod);

        printf("Consumidora processou: %s - Média: %.2f - Horas fora: %.2f\n",
               dado.nome_sensor, dado.media, dado.horas_fora);
        fprintf(saida, "%s;%.2f;%.2f\n", dado.nome_sensor, dado.media, dado.horas_fora);
        usleep(150000);
    }

    fclose(saida);
    return NULL;
}

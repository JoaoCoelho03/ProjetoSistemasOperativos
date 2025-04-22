#ifndef PROCESS_SENSOR_PRODCONS_H
#define PROCESS_SENSOR_PRODCONS_H

#define MAX_FILA 10  // Tamanho da fila de dados

typedef struct {
    char nome_sensor[128];
    double media;
    int horas_fora;
} DadoSensor;

typedef struct {
    DadoSensor fila[MAX_FILA];
    int inicio;
    int fim;
    pthread_mutex_t mutex;
} FilaDados;

void *produtor(void *arg);
void *consumidor(void *arg);
void adicionar_fila(FilaDados *fila, DadoSensor dado);
DadoSensor remover_fila(FilaDados *fila);

#endif

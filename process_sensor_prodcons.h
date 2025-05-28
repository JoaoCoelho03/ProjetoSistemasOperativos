#ifndef PROCESS_SENSOR_PRODCONS_H
#define PROCESS_SENSOR_PRODCONS_H

#define MAX_FILA 10
#define TOTAL_DADOS 20
#define NUM_CONSUMIDORAS 2
#define NUM_PRODUTORAS 2

typedef struct {
    char nome_sensor[128];
    double media;
    double horas_fora;
} DadoSensor;

typedef struct {
    DadoSensor fila[MAX_FILA];
    int inicio;
    int fim;
    pthread_mutex_t mutex;
} FilaDados;

typedef struct {
    FilaDados *fila;
    int start_index;
    int total_gerar;
} ArgsProdutor;


void *produtor(void *arg);
void *consumidor(void *arg);
void adicionar_fila(FilaDados *fila, DadoSensor dado);
DadoSensor remover_fila(FilaDados *fila);

#endif

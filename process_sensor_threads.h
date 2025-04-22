#ifndef PROCESS_SENSOR_THREADS_H
#define PROCESS_SENSOR_THREADS_H

typedef struct {
    int thread_id;
    char nome_sensor[128];
    double media;
    int horas_fora;
} ResultadoSensor;

typedef struct {
    char filepath[256];
    int index;
    ResultadoSensor *resultados;
} ThreadArgs;

void *processar_sensor_thread(void *args);

#endif

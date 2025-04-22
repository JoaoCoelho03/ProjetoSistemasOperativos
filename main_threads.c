#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "process_sensor_threads.h"

#define MAX_FILES 100

volatile int sensores_concluidos = 0;
bool progresso_ativo = true;

void *barra_progresso(void *arg) {
    int total = *(int *)arg;
    int anterior = -1;

    while (progresso_ativo) {
        int percentagem = (sensores_concluidos * 100) / total;
        if (percentagem != anterior) {
            anterior = percentagem;
            int barras = percentagem / 10;
            printf("\rProgresso: [");
            for (int i = 0; i < 10; i++) {
                if (i < barras) printf("=");
                else if (i == barras) printf(">");
                else printf(" ");
            }
            printf("] %d%%", percentagem);
            fflush(stdout);
        }
        sleep(1);
    }
    printf("\rProgresso: [==========>] 100%%\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <diretorio_dados>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    DIR *dp = opendir(argv[1]);
    if (!dp) {
        perror("Erro ao abrir diretório");
        exit(EXIT_FAILURE);
    }

    char *ficheiros[MAX_FILES];
    int num_ficheiros = 0;
    struct dirent *entry;

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_REG) {
            ficheiros[num_ficheiros] = strdup(entry->d_name);
            num_ficheiros++;
        }
    }
    closedir(dp);

    pthread_t threads[num_ficheiros];
    ResultadoSensor resultados[num_ficheiros];
    ThreadArgs args[num_ficheiros];

    // Inicia thread de progresso
    pthread_t progresso_thread;
    pthread_create(&progresso_thread, NULL, barra_progresso, &num_ficheiros);

    for (int i = 0; i < num_ficheiros; i++) {
        snprintf(args[i].filepath, sizeof(args[i].filepath), "%s/%s", argv[1], ficheiros[i]);
        args[i].index = i;
        args[i].resultados = resultados;
        pthread_create(&threads[i], NULL, processar_sensor_thread, &args[i]);
    }

    for (int i = 0; i < num_ficheiros; i++) {
        pthread_join(threads[i], NULL);
        free(ficheiros[i]);
    }

    progresso_ativo = false;
    pthread_join(progresso_thread, NULL);

    FILE *saida = fopen("relatorio_threads.txt", "w");
    for (int i = 0; i < num_ficheiros; i++) {
        fprintf(saida, "Thread %d;%s;%.2f;%d\n",
                resultados[i].thread_id,
                resultados[i].nome_sensor,
                resultados[i].media,
                resultados[i].horas_fora);
    }
    fclose(saida);

    printf("Relatório com threads gerado: relatorio_threads.txt\n");
    return 0;
}

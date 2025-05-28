#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "process_sensor.h"
#include "utils.h"

#define MAX_FILES 100

volatile int sensores_processados = 0;
int total_sensores = 0;
bool progresso_concluido = false;
int ficheiros_processados = 0;  // Variável global de ficheiros processados

pthread_mutex_t mutex_progresso = PTHREAD_MUTEX_INITIALIZER;  // Mutex para proteger 'ficheiros_processados'

// Estrutura para leitura de pipes
typedef struct {
    int (*pipes)[2];
    int num_sensores;
} LeituraArgs;

// Função da barra de progresso
void *barra_progresso(void *arg) {
    int ultima_percentagem = -1;

    while (!progresso_concluido){

        int percent = (ficheiros_processados * 100) / total_sensores;

        if (percent != ultima_percentagem) {
            ultima_percentagem = percent;

            int barras = percent / 10;

            printf("\rProgresso: [");
            for (int i = 0; i < 10; i++) {
                if (i < barras) printf("=");
                else if (i == barras) printf(">");
                else printf(" ");
            }
            printf("] %d%%", percent);
            fflush(stdout);
        }

    }

    printf("\rProgresso: [==========>] 100%%\n");
    return NULL;
}


void *ler_pipes(void *arg) {
    LeituraArgs *args = (LeituraArgs *)arg;
    FILE *saida = fopen("relatorio.txt", "w");
    if (!saida) {
        perror("Erro ao criar relatorio.txt");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    for (int i = 0; i < args->num_sensores; i++) {
        readn(args->pipes[i][0], buffer, sizeof(buffer));
        fprintf(saida, "%s\n", buffer);
        close(args->pipes[i][0]);

        pthread_mutex_lock(&mutex_progresso);
        ficheiros_processados++;
        pthread_mutex_unlock(&mutex_progresso);
    }

    fclose(saida);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <diretorio_dados>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *diretorio = argv[1];
    struct dirent *entry;
    DIR *dp = opendir(diretorio);
    if (!dp) {
        perror("Erro ao abrir diretório");
        exit(EXIT_FAILURE);
    }

    char *ficheiros[MAX_FILES];
    int num_ficheiros = 0;

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_REG) {
            ficheiros[num_ficheiros] = strdup(entry->d_name);
            num_ficheiros++;
        }
    }
    closedir(dp);
    total_sensores = num_ficheiros;

    int pipes[num_ficheiros][2];
    pid_t filhos[num_ficheiros];

    for (int i = 0; i < num_ficheiros; i++) {
        pipe(pipes[i]);

        pid_t pid = fork();
        if (pid == 0) {
            close(pipes[i][0]);
            char caminho[256];
            snprintf(caminho, sizeof(caminho), "%s/%s", diretorio, ficheiros[i]);
            process_sensor_file(caminho, pipes[i][1]);
            close(pipes[i][1]);
            exit(EXIT_SUCCESS);
        } else {
            filhos[i] = pid;
            close(pipes[i][1]);
        }
    }

    pthread_t thread_barra, thread_leitura;

    LeituraArgs args = {
        .pipes = pipes,
        .num_sensores = num_ficheiros
    };


    pthread_create(&thread_barra, NULL, barra_progresso, NULL);
    pthread_create(&thread_leitura, NULL, ler_pipes, &args);


    for (int i = 0; i < num_ficheiros; i++) {
        waitpid(filhos[i], NULL, 0);
        free(ficheiros[i]);
    }


    pthread_join(thread_leitura, NULL);
    progresso_concluido = true;
    pthread_join(thread_barra, NULL);

    printf("Relatório gerado com sucesso em 'relatorio.txt'\n");
    return 0;
}

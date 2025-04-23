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

typedef struct {
    int (*pipes)[2];
    int num_sensores;
} LeituraArgs;

void *barra_progresso(void *arg) {
    int ultima_percentagem = -1;

    // A barra de progresso será atualizada enquanto o processo não terminar
    while (!progresso_concluido) {
        // Calcular o progresso com base no número de sensores processados
        int percent = (sensores_processados * 100) / total_sensores;

        // Atualizar a barra de progresso quando o percentual mudar
        if (percent != ultima_percentagem) {
            ultima_percentagem = percent;

            // Calcular a quantidade de barras para mostrar na barra de progresso
            int barras = percent / 10;

            // Imprimir a barra de progresso no terminal
            printf("\rProgresso: [");
            for (int i = 0; i < 10; i++) {
                if (i < barras) printf("=");
                else if (i == barras) printf(">");
                else printf(" ");
            }
            printf("] %d%%", percent);
            fflush(stdout);  // Garantir que o terminal seja atualizado
        }

        // Aguardar 5 segundos para continuar atualizando a barra
        sleep(5);  // Atualiza a barra de progresso a cada 5 segundos
    }

    // Mostrar 100% quando o progresso for concluído
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
        sensores_processados++;
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

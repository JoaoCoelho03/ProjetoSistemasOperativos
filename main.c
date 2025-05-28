#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include "process_sensor.h"
#include "utils.h"

#define MAX_FILES 100

int total_sensores = 0;

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

    FILE *saida = fopen("relatorio.txt", "w");
    if (!saida) {
        perror("Erro ao criar relatorio.txt");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    int ficheiros_processados = 0;
    time_t ultimo_update = time(NULL);

    for (int i = 0; i < num_ficheiros; i++) {
        readn(pipes[i][0], buffer, sizeof(buffer));
        fprintf(saida, "%s\n", buffer);
        close(pipes[i][0]);

        ficheiros_processados++;

        time_t agora = time(NULL);
        if (difftime(agora, ultimo_update) >= 5 || ficheiros_processados == total_sensores) {
            int percent = (ficheiros_processados * 100) / total_sensores;
            int barras = percent / 10;

            printf("\rProgresso: [");
            for (int j = 0; j < 10; j++) {
                if (j < barras) printf("=");
                else if (j == barras) printf(">");
                else printf(" ");
            }
            printf("] %d%%", percent);
            fflush(stdout);

            ultimo_update = agora;
        }
    }

    fclose(saida);

    for (int i = 0; i < num_ficheiros; i++) {
        waitpid(filhos[i], NULL, 0);
        free(ficheiros[i]);
    }

    printf("\nRelatório gerado com sucesso em 'relatorio.txt'\n");
    return 0;
}

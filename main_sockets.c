#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/wait.h>
#include "process_sensor_sockets.h"
#include "utils.h"

#define MAX_FILES 100
#define SOCKET_PATH "/tmp/sensor_socket"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <diretorio_dados>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    unlink(SOCKET_PATH);
    int server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    bind(server_sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_sock, 10);

    DIR *dp = opendir(argv[1]);
    if (!dp) {
        perror("Erro ao abrir diretório");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    char *ficheiros[MAX_FILES];
    int num_ficheiros = 0;

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_REG) {
            ficheiros[num_ficheiros] = strdup(entry->d_name);
            num_ficheiros++;
        }
    }
    closedir(dp);

    for (int i = 0; i < num_ficheiros; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            char caminho[256];
            snprintf(caminho, sizeof(caminho), "%s/%s", argv[1], ficheiros[i]);
            process_sensor_socket(caminho);
            exit(0);
        }
    }

    FILE *saida = fopen("relatorio_socket.txt", "w");
    for (int i = 0; i < num_ficheiros; i++) {
        int client_sock = accept(server_sock, NULL, NULL);
        char buffer[256];
        readn(client_sock, buffer, sizeof(buffer));
        fprintf(saida, "%s\n", buffer);
        close(client_sock);
        free(ficheiros[i]);
    }

    fclose(saida);
    close(server_sock);
    unlink(SOCKET_PATH);

    for (int i = 0; i < num_ficheiros; i++) wait(NULL);

    printf("Relatório com sockets gerado: relatorio_socket.txt\n");
    return 0;
}

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "process_sensor_sockets.h"
#include "utils.h"

#define SOCKET_PATH "/tmp/sensor_socket"
#define TIME_FORMAT "%Y-%m-%dT%H:%M:%S"

time_t parse_timestamp(const char *timestamp) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(timestamp, TIME_FORMAT, &tm);
    return mktime(&tm);
}

int fora_do_intervalo(const char *sensor, double valor) {
    if (strstr(sensor, "temperatura")) return (valor < 18 || valor > 27);
    if (strstr(sensor, "humidade")) return (valor < 30 || valor > 70);
    if (strstr(sensor, "pm2.5") || strstr(sensor, "PM2.5")) return (valor >= 25);
    if (strstr(sensor, "pm10") || strstr(sensor, "PM10")) return (valor >= 50);
    if (strstr(sensor, "co2") || strstr(sensor, "CO2")) return (valor > 1000);
    return 0;
}

void process_sensor_socket(const char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir ficheiro");
        exit(EXIT_FAILURE);
    }

    struct stat sb;
    fstat(fd, &sb);
    char *data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    double soma = 0;
    int count = 0, fora = 0;
    double valor;
    char sensor_name[128], timestamp[64];
    time_t prev_time = 0, current_time;
    int primeira = 1;

    char *linha = data;
    char *fim = data;

    while (*fim != '\n' && fim < data + sb.st_size) fim++;
    linha = fim + 1;

    while (fim < data + sb.st_size) {
        if (*fim == '\n' || fim == data + sb.st_size - 1) {
            size_t len = fim - linha + 1;
            char linha_copia[len];
            memcpy(linha_copia, linha, len);
            linha_copia[len - 1] = '\0';

            if (sscanf(linha_copia, "%[^,],%lf,%s", sensor_name, &valor, timestamp) == 3) {
                soma += valor;
                count++;
                current_time = parse_timestamp(timestamp);
                if (fora_do_intervalo(filepath, valor)) {
                    if (!primeira) fora += difftime(current_time, prev_time) / 3600.0;
                    primeira = 0;
                } else primeira = 1;
                prev_time = current_time;
            }
            linha = fim + 1;
        }
        fim++;
    }
    munmap(data, sb.st_size);

    double media = count > 0 ? soma / count : 0;
    char mensagem[256];
    snprintf(mensagem, sizeof(mensagem), "%d;%s;%.2f;%d", getpid(), sensor_name, media, fora);

    // Enviar via socket
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    writen(sock, mensagem, strlen(mensagem) + 1);
    close(sock);
}

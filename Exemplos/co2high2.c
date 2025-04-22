#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define TIME_FORMAT "%Y-%m-%dT%H:%M:%S"

// Função para converter timestamp em segundos
time_t parse_timestamp(const char *timestamp) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(timestamp, TIME_FORMAT, &tm);
    return mktime(&tm);
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <ficheiro.csv> <limite CO2 em ppm>\n", argv[0]);
        return 1;
    }

    const char *file_path = argv[1];
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir o ficheiro");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Erro ao obter informações do ficheiro");
        close(fd);
        return 1;
    }

    char *file_data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("Erro ao mapear o ficheiro");
        close(fd);
        return 1;
    }
    
    close(fd); // Já não precisamos do descritor do ficheiro

    double co2_value , total_time = 0;
    int threshold = atoi(argv[2]);
    time_t prev_time = 0, current_time;
    char sensor_name[100];
    char timestamp[50] = "";
    int first = 1;

    char *start = file_data;
    char *end = file_data;
    
    // Percorre o ficheiro procurando quebras de linha
    while (end < file_data + sb.st_size) {
        if (*end == '\n' || end == file_data + sb.st_size - 1) {
            size_t len = end - start + 1;
            char line[len];
            memcpy(line, start, len);
            line[len - 1] = '\0'; // Garantir terminação correta da string

            
            if (sscanf(line, "%99[^,],%lf,%49s", sensor_name, &co2_value, timestamp) == 3) {
                current_time = parse_timestamp(timestamp);
                if (co2_value > threshold) {
                    if (!first) {
                        total_time += difftime(current_time, prev_time) / 3600.0; // Tempo em horas
                    }
                    first=0;
                }
                else
                    first = 1;
                prev_time = current_time;
            }

            start = end + 1; // Avançar para a próxima linha
        }
        end++;
    }

    munmap(file_data, sb.st_size);

    printf("Tempo total com CO2 acima de %d ppm: %.2f horas\n", threshold, total_time);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LINE_SIZE 256

// Função para converter string de timestamp para struct tm
time_t parse_timestamp(const char *timestamp) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(timestamp, "%Y-%m-%dT%H:%M:%S", &tm);
    return mktime(&tm);
}

// Função para processar uma linha do CSV
int parse_line(const char *line, double *co2_value, time_t *timestamp) {
    char sensor_name[100], timestamp_str[50];
    
    if (sscanf(line, "%99[^,],%lf,%49s", sensor_name, co2_value, timestamp_str) == 3) {
        *timestamp = parse_timestamp(timestamp_str);
        return 1;
    }
    return 0;
}

int main(int argc, char **argv, char** envp) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <ficheiro.csv>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Erro ao abrir o ficheiro");
        return 1;
    }

    char line[LINE_SIZE];
    double co2_value;
    time_t timestamp, prev_timestamp = 0;
    double total_time = 0;
    int first = 1;

    // Ignorar cabeçalho
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        if (parse_line(line, &co2_value, &timestamp)) {
            if (co2_value > 800) {
                if (!first) {
                    total_time += difftime(timestamp, prev_timestamp) / 3600.0; // Converter para horas
                }
                first = 0;
            }
            else
                first = 1;
            
            prev_timestamp = timestamp;
        }
    }

    fclose(file);

    printf("Tempo total com CO2 acima de 800: %.2f horas\n", total_time);
    return 0;
}


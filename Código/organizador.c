#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SENSORES 100
#define MAX_ID 32
#define MAX_LINHA 128
#define MAX_VALOR 64

typedef struct {
    long timestamp;
    char valor[MAX_VALOR];
} Leitura;

typedef struct {
    char id_sensor[MAX_ID];
    Leitura* leituras;
    int qtd;
    int capacidade;
} Sensor;


int comparar_por_timestamp(const void* a, const void* b) {
    Leitura* la = (Leitura*)a;
    Leitura* lb = (Leitura*)b;
    return (la->timestamp > lb->timestamp) - (la->timestamp < lb->timestamp);
}

int encontrar_sensor(Sensor sensores[], int total, const char* id) {
    for (int i = 0; i < total; i++) {
        if (strcmp(sensores[i].id_sensor, id) == 0) return i;
    }
    return -1;
}


void adicionar_leitura(Sensor* sensor, long timestamp, const char* valor) {
    if (sensor->qtd >= sensor->capacidade) {
        sensor->capacidade *= 2;
        sensor->leituras = realloc(sensor->leituras, sensor->capacidade * sizeof(Leitura));
    }
    sensor->leituras[sensor->qtd].timestamp = timestamp;
    strncpy(sensor->leituras[sensor->qtd].valor, valor, MAX_VALOR);
    sensor->qtd++;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE* entrada = fopen(argv[1], "r");
    if (!entrada) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    Sensor sensores[MAX_SENSORES];
    int total_sensores = 0;

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), entrada)) {
        long timestamp;
        char id_sensor[MAX_ID];
        char valor[MAX_VALOR];

        if (sscanf(linha, "%ld %s %s", &timestamp, id_sensor, valor) != 3) continue;

        int idx = encontrar_sensor(sensores, total_sensores, id_sensor);
        if (idx == -1) {
            // Novo sensor
            strncpy(sensores[total_sensores].id_sensor, id_sensor, MAX_ID);
            sensores[total_sensores].leituras = malloc(100 * sizeof(Leitura));
            sensores[total_sensores].qtd = 0;
            sensores[total_sensores].capacidade = 100;
            idx = total_sensores++;
        }

        adicionar_leitura(&sensores[idx], timestamp, valor);
    }

    fclose(entrada);

    for (int i = 0; i < total_sensores; i++) {
        Sensor* s = &sensores[i];
        qsort(s->leituras, s->qtd, sizeof(Leitura), comparar_por_timestamp);

        char nome_arquivo[64];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", s->id_sensor);

        FILE* saida = fopen(nome_arquivo, "w");
        for (int j = 0; j < s->qtd; j++) {
            fprintf(saida, "%ld %s %s\n", s->leituras[j].timestamp, s->id_sensor, s->leituras[j].valor);
        }
        fclose(saida);
        free(s->leituras);
    }

    printf("Organização concluída com sucesso.\n");
    return 0;
}

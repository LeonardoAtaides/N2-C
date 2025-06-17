#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h> 
#else
#include <sys/stat.h>
#endif

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
        if (!sensor->leituras) {
            perror("Erro de alocação");
            exit(EXIT_FAILURE);
        }
    }
    sensor->leituras[sensor->qtd].timestamp = timestamp;
    strncpy(sensor->leituras[sensor->qtd].valor, valor, MAX_VALOR);
    sensor->leituras[sensor->qtd].valor[MAX_VALOR - 1] = '\0';
    sensor->qtd++;
}

int main() {
    char caminho_csv[] = "./Arquivos_Gerados/arquivos_embaralhados.csv";

    FILE* entrada = fopen(caminho_csv, "r");
    if (!entrada) {
        perror("Erro ao abrir ./Arquivos_Gerados/arquivos_embaralhados.csv");
        printf("\033[1;33mCertifique-se de que a pasta 'Arquivos_Gerados' está criada, se não é necessário executar o programa 'gerador.c'.\033[0m\n");
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
            if (total_sensores >= MAX_SENSORES) {
                fprintf(stderr, "Limite maximo de sensores atingido.\n");
                break;
            }
            strncpy(sensores[total_sensores].id_sensor, id_sensor, MAX_ID);
            sensores[total_sensores].id_sensor[MAX_ID - 1] = '\0';
            sensores[total_sensores].leituras = malloc(100 * sizeof(Leitura));
            if (!sensores[total_sensores].leituras) {
                perror("Erro de alocação");
                fclose(entrada);
                return 1;
            }
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

        char nome_arquivo[128];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "./Arquivos_Gerados/%s.csv", s->id_sensor);

        FILE* saida = fopen(nome_arquivo, "w");
        if (!saida) {
            perror("Erro ao criar arquivo de saída");
            continue;
        }

        for (int j = 0; j < s->qtd; j++) {
            fprintf(saida, "%ld %s %s\n", s->leituras[j].timestamp, s->id_sensor, s->leituras[j].valor);
        }

        fclose(saida);
        free(s->leituras);
    }

    printf("\033[1;32mOrganização concluída com sucesso!\033[0m Arquivos salvos em Arquivos_Gerados/.\n");
    return 0;
}

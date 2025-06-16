#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>

#define MAX_LINHA 128
#define MAX_VALOR 64
#define MAX_ID 32

typedef struct {
    long timestamp;
    char valor[MAX_VALOR];
} Leitura;

long diferenca(long a, long b) {
    return labs(a - b);
}

int busca_binaria_proximo(Leitura* dados, int n, long alvo) {
    int inicio = 0, fim = n - 1;
    int mais_proximo = -1;
    long menor_dif = LONG_MAX;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        long dif_atual = diferenca(dados[meio].timestamp, alvo);
        if (dif_atual < menor_dif) {
            menor_dif = dif_atual;
            mais_proximo = meio;
        }

        if (dados[meio].timestamp == alvo) {
            return meio;
        } else if (dados[meio].timestamp > alvo) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    return mais_proximo;
}

int arquivo_existe(const char *caminho) {
    struct stat buffer;
    return (stat(caminho, &buffer) == 0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("\033[1;33mALERTA! PARA REALIZAR A CONSULTA DEVE SER NESTE FORMATO:\033[0m\n");
        printf("\033[1;32mExemplo:\033[0m .\\consulta <id_sensor> <timestamp>\n");
        return 1;
    }

    char* id_sensor = argv[1];
    char* fim_parse;
    long timestamp_consulta = strtol(argv[2], &fim_parse, 10);

    if (*fim_parse != '\0') {
        printf("\033[1;31mErro: Timestamp invalido. Use numero inteiro representando unix epoch.\033[0m\n");
        return 1;
    }

    printf("\033[1;34mINFO:\033[0m Este programa espera que os arquivos estejam ordenados por timestamp em ordem \033[1;36mdecrescente\033[0m.\n\n");

    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "./Arquivos_Gerados/%s.csv", id_sensor);

    if (!arquivo_existe(caminho_arquivo)) {
        printf("\033[1;31mErro: Sensor '%s' nao encontrado. Verifique o nome do sensor.\033[0m\n", id_sensor);
        return 1;
    }

    FILE* arquivo = fopen(caminho_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo do sensor");
        return 1;
    }

    Leitura* dados = NULL;
    int capacidade = 1000;
    int qtd = 0;

    dados = malloc(capacidade * sizeof(Leitura));
    if (!dados) {
        printf("\033[1;31mErro ao alocar memoria.\033[0m\n");
        fclose(arquivo);
        return 1;
    }

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), arquivo)) {
        long ts;
        char id_lido[MAX_ID], valor[MAX_VALOR];
        int campos_lidos = sscanf(linha, "%ld %s %s", &ts, id_lido, valor);
        if (campos_lidos != 3) {
            continue;
        }
        if (strcmp(id_lido, id_sensor) != 0) {
            continue;
        }

        if (qtd >= capacidade) {
            capacidade *= 2;
            Leitura* temp = realloc(dados, capacidade * sizeof(Leitura));
            if (!temp) {
                printf("\033[1;31mErro ao realocar memoria.\033[0m\n");
                free(dados);
                fclose(arquivo);
                return 1;
            }
            dados = temp;
        }

        dados[qtd].timestamp = ts;
        strncpy(dados[qtd].valor, valor, MAX_VALOR - 1);
        dados[qtd].valor[MAX_VALOR - 1] = '\0';
        qtd++;
    }

    fclose(arquivo);

    if (qtd == 0) {
        printf("\033[1;33mNenhum dado encontrado para o sensor '%s'.\033[0m\n", id_sensor);
        free(dados);
        return 1;
    }

    int indice = busca_binaria_proximo(dados, qtd, timestamp_consulta);
    if (indice == -1) {
        printf("\033[1;33mNenhuma leitura encontrada proxima ao timestamp fornecido.\033[0m\n");
        free(dados);
        return 1;
    }

    printf("Leitura mais próxima encontrada:\n");
    printf("Timestamp: %ld\n", dados[indice].timestamp);
    printf("Sensor: %s\n", id_sensor);
    printf("Valor: %s\n", dados[indice].valor);

    free(dados);
    return 0;
}

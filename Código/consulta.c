#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define MAX_LINHA 128
#define MAX_VALOR 64
#define MAX_ID 32

typedef struct {
    long timestamp;
    char valor[MAX_VALOR];
} Leitura;

// Função para calcular diferença absoluta entre timestamps
long diferenca(long a, long b) {
    return labs(a - b);
}

// Função de busca binária modificada para encontrar o timestamp mais próximo
int busca_binaria_proximo(Leitura* dados, int n, long alvo) {
    int inicio = 0, fim = n - 1;
    int mais_proximo = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        if (diferenca(dados[meio].timestamp, alvo) < diferenca(dados[mais_proximo].timestamp, alvo)) {
            mais_proximo = meio;
        }

        if (dados[meio].timestamp < alvo)
            inicio = meio + 1;
        else if (dados[meio].timestamp > alvo)
            fim = meio - 1;
        else
            return meio; // achou exatamente
    }

    return mais_proximo;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <id_sensor> <timestamp>\n", argv[0]);
        return 1;
    }

    char* id_sensor = argv[1];
    long timestamp_consulta = atol(argv[2]);

    char nome_arquivo[64];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", id_sensor);

    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo do sensor");
        return 1;
    }

    Leitura* dados = malloc(10000 * sizeof(Leitura)); // máximo esperado
    int qtd = 0;

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), arquivo)) {
        long ts;
        char id_lido[MAX_ID], valor[MAX_VALOR];
        if (sscanf(linha, "%ld %s %s", &ts, id_lido, valor) == 3) {
            dados[qtd].timestamp = ts;
            strcpy(dados[qtd].valor, valor);
            qtd++;
        }
    }

    fclose(arquivo);

    if (qtd == 0) {
        printf("Nenhum dado encontrado.\n");
        free(dados);
        return 1;
    }

    int indice = busca_binaria_proximo(dados, qtd, timestamp_consulta);
    printf("Leitura mais próxima:\n%ld %s %s\n", dados[indice].timestamp, id_sensor, dados[indice].valor);

    free(dados);
    return 0;
}

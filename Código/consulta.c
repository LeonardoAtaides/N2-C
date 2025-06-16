#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
    int mais_proximo = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        if (dados[meio].timestamp == alvo) {
            return meio;
        }


        if (diferenca(dados[meio].timestamp, alvo) < diferenca(dados[mais_proximo].timestamp, alvo)) {
            mais_proximo = meio;
        }

        if (dados[meio].timestamp < alvo)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }

    return mais_proximo;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("\033[1;33mALERTA! PARA REALIZAR A CONSULTA O ARQUIVO DEVE SER NESTE FORMATO:\033[0m\n");
        printf("\033[1;32mExemplo:\033[0m.\\consulta <id_sensor> <timestamp>\n", argv[0]);
        return 1;
    }

    char* id_sensor = argv[1];
    long timestamp_consulta = atol(argv[2]);

    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "./Arquivos_Gerados/%s.csv", id_sensor);

    FILE* arquivo = fopen(caminho_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo do sensor");
        return 1;
    }

    Leitura* dados = malloc(10000 * sizeof(Leitura));
    if (!dados) {
        printf("Erro ao alocar memória\n");
        fclose(arquivo);
        return 1;
    }

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
        printf("Nenhum dado encontrado para o sensor '%s'.\n", id_sensor);
        free(dados);
        return 1;
    }

    int indice = busca_binaria_proximo(dados, qtd, timestamp_consulta);

    printf("Leitura mais próxima:\n%ld %s %s\n", dados[indice].timestamp, id_sensor, dados[indice].valor);

    free(dados);
    return 0;
}

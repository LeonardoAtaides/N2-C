#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 128
#define MAX_VALOR 64
#define MAX_ID 32

typedef struct {
    long timestamp;
    char valor[MAX_VALOR];
} Leitura;

// Função para comparação usada no qsort
int comparar_timestamps(const void *a, const void *b) {
    Leitura *la = (Leitura *)a;
    Leitura *lb = (Leitura *)b;
    if (la->timestamp < lb->timestamp) return -1;
    else if (la->timestamp > lb->timestamp) return 1;
    else return 0;
}

// Função para calcular diferença absoluta
long diferenca(long a, long b) {
    long diff = a - b;
    return diff < 0 ? -diff : diff;
}

// Busca binária para encontrar índice do timestamp mais próximo
int busca_binaria_proximo(Leitura* dados, int n, long alvo) {
    int inicio = 0, fim = n - 1;
    int melhor = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        if (diferenca(dados[meio].timestamp, alvo) < diferenca(dados[melhor].timestamp, alvo)) {
            melhor = meio;
        }

        if (dados[meio].timestamp < alvo)
            inicio = meio + 1;
        else if (dados[meio].timestamp > alvo)
            fim = meio - 1;
        else
            return meio;  // timestamp exato encontrado
    }
    return melhor;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <id_sensor> <timestamp_unix>\n", argv[0]);
        return 1;
    }

    char *id_sensor = argv[1];
    long timestamp_consulta = atol(argv[2]);

    const char *caminho_arquivo = "./Arquivos_Gerados/leitura.txt";
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo de leituras");
        return 1;
    }

    Leitura *dados = malloc(100000 * sizeof(Leitura));  // espaço para até 100k leituras
    if (!dados) {
        printf("Erro de memória\n");
        fclose(arquivo);
        return 1;
    }
    int qtd = 0;

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), arquivo)) {
        long ts;
        char sensor_lido[MAX_ID];
        char valor[MAX_VALOR];

        if (sscanf(linha, "%ld %s %s", &ts, sensor_lido, valor) == 3) {
            if (strcmp(sensor_lido, id_sensor) == 0) {
                dados[qtd].timestamp = ts;
                strcpy(dados[qtd].valor, valor);
                qtd++;
            }
        }
    }
    fclose(arquivo);

    if (qtd == 0) {
        printf("Nenhuma leitura encontrada para o sensor '%s'\n", id_sensor);
        free(dados);
        return 1;
    }

    // Ordena os dados por timestamp para busca binária
    qsort(dados, qtd, sizeof(Leitura), comparar_timestamps);

    // Busca o índice da leitura mais próxima
    int indice = busca_binaria_proximo(dados, qtd, timestamp_consulta);

    printf("Leitura mais próxima:\n%ld %s %s\n", dados[indice].timestamp, id_sensor, dados[indice].valor);

    free(dados);
    return 0;
}

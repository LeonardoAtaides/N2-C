#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <time.h>

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

int busca_binaria_pelo_proximo(Leitura* dados, int n, long alvo) {
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
        printf("\033[1;33mUSO CORRETO:\033[0m\n");
        printf("\033[1;32mExemplo:\033[0m ./consulta_sensores temperatura 15/06/2025-14:30:00\n");
        return 1;
    }

    char* id_sensor = argv[1];
    int dia, mes, ano, hora, min, seg;
    if (sscanf(argv[2], "%2d/%2d/%4d-%2d:%2d:%2d", &dia, &mes, &ano, &hora, &min, &seg) != 6) {
        printf("\033[1;31mErro: Formato de data invalido. Use 'DD/MM/AAAA-HH:MM:SS'.\033[0m\n");
        return 1;
    }

    struct tm tm_data = {0};
    tm_data.tm_mday = dia;
    tm_data.tm_mon = mes - 1;
    tm_data.tm_year = ano - 1900;
    tm_data.tm_hour = hora;
    tm_data.tm_min = min;
    tm_data.tm_sec = seg;

    time_t timestamp_consulta = mktime(&tm_data);
    if (timestamp_consulta == -1) {
        printf("\033[1;31mErro ao converter data para timestamp.\033[0m\n");
        return 1;
    }

    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "./Arquivos_Gerados/sensores_organizados/%s.csv", id_sensor);

    if (!arquivo_existe(caminho_arquivo)) {
        printf("\033[1;31mErro: Sensor '%s' nao encontrado.\033[0m\n", id_sensor);
        return 1;
    }

    FILE* arquivo = fopen(caminho_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo do sensor");
        return 1;
    }

    Leitura* dados = malloc(1000 * sizeof(Leitura));
    int capacidade = 1000, qtd = 0;

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), arquivo)) {
        long ts;
        char id_lido[MAX_ID], valor[MAX_VALOR];
        if (sscanf(linha, "%ld %s %s", &ts, id_lido, valor) != 3) continue;
        if (strcmp(id_lido, id_sensor) != 0) continue;

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

    int indice = busca_binaria_pelo_proximo(dados, qtd, timestamp_consulta);
    if (indice == -1) {
        printf("\033[1;33mNenhuma leitura proxima a data informada.\033[0m\n");
        free(dados);
        return 1;
    }

    char buffer_data[64];
    time_t encontrado = dados[indice].timestamp;
    struct tm *info_data = localtime(&encontrado);
    strftime(buffer_data, sizeof(buffer_data), "%d/%m/%Y-%H:%M:%S", info_data);

    printf("\033[1;34mLeitura mais proxima encontrada:\033[0m\n");
    printf("Data e Hora: %s\n", buffer_data);
    printf("Sensor: %s\n", id_sensor);
    printf("Valor: %s\n", dados[indice].valor);

    free(dados);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#endif

#define MAX_SENSORES 50
#define MAX_NOME 32
#define MAX_LEITURAS 2000

typedef enum { TIPO_INT, TIPO_BOOL, TIPO_FLOAT, TIPO_STRING, TIPO_INVALIDO = -1 } TipoDado;

typedef struct {
    char nome[MAX_NOME];
    TipoDado tipo;
} Sensor;

typedef struct {
    time_t timestamp;
    char sensor_nome[MAX_NOME];
    char valor[32];
} Leitura;

time_t converter_para_timestamp(const char *data_str) {
    struct tm t;
    memset(&t, 0, sizeof(t));
    int campos = sscanf(data_str, "%d/%d/%d-%d:%d:%d",
        &t.tm_mday, &t.tm_mon, &t.tm_year,
        &t.tm_hour, &t.tm_min, &t.tm_sec);
    if (campos != 6) {
        printf("Formato de data/hora inválido.\n");
        return -1;
    }

    t.tm_year -= 1900;
    t.tm_mon -= 1;
    t.tm_isdst = -1;

    return mktime(&t);
}

char *gerar_valor_aleatorio(TipoDado tipo) {
    static char buffer[32];
    switch (tipo) {
        case TIPO_INT:
            sprintf(buffer, "%d", rand() % 1000);
            break;
        case TIPO_BOOL:
            sprintf(buffer, "%s", (rand() % 2) ? "true" : "false");
            break;
        case TIPO_FLOAT:
            sprintf(buffer, "%.2f", ((float)rand() / RAND_MAX) * 100.0);
            break;
        case TIPO_STRING: {
            int len = 4 + rand() % 13;
            for (int i = 0; i < len; i++) {
                buffer[i] = 'A' + rand() % 26;
            }
            buffer[len] = '\0';
            break;
        }
        default:
            strcpy(buffer, "NA");
            break;
    }
    return buffer;
}

TipoDado obter_tipo(const char *tipo_str) {
    if (strcmp(tipo_str, "int") == 0) return TIPO_INT;
    if (strcmp(tipo_str, "bool") == 0) return TIPO_BOOL;
    if (strcmp(tipo_str, "float") == 0) return TIPO_FLOAT;
    if (strcmp(tipo_str, "string") == 0) return TIPO_STRING;
    return TIPO_INVALIDO;
}

void embaralhar_leituras(Leitura *leituras, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Leitura temp = leituras[i];
        leituras[i] = leituras[j];
        leituras[j] = temp;
    }
}

int main(int argc, char *argv[]) {
    char *execName = argv[0];
    char *lastSlash = strrchr(argv[0], '\\');
    if (lastSlash != NULL) execName = lastSlash + 1;

    if (argc < 4) {
        printf("\033[1;33mALERTA! PARA GERAR O ARQUIVO DEVE SER NESTE FORMATO:\033[0m\n");
        printf("Uso: .\\%s <data_inicio> <data_fim> <sensor1:tipo> [sensor2:tipo] ...\n", execName);
        printf("\033[1;32mExemplo:\033[0m .\\%s 15/06/2025-00:00:00 15/06/2025-23:59:59 temp:int umidade:float\n\n", execName);
        return 1;
    }

    srand(time(NULL));

    time_t inicio = converter_para_timestamp(argv[1]);
    time_t fim = converter_para_timestamp(argv[2]);
    if (inicio == -1 || fim == -1 || fim < inicio) {
        printf("Intervalo de datas inválido.\n");
        return 1;
    }

    int qtd_sensores = argc - 3;
    if (qtd_sensores > MAX_SENSORES) {
        printf("Limite máximo de sensores (%d) ultrapassado.\n", MAX_SENSORES);
        return 1;
    }

    Sensor sensores[MAX_SENSORES];
    for (int i = 0; i < qtd_sensores; i++) {
        char *nome_tipo = argv[i + 3];
        char *tipo_str = strchr(nome_tipo, ':');
        if (!tipo_str) {
            printf("Erro no argumento: %s\n", nome_tipo);
            return 1;
        }

        *tipo_str = '\0';
        tipo_str++;

        sensores[i].tipo = obter_tipo(tipo_str);
        if (sensores[i].tipo == TIPO_INVALIDO) {
            printf("Tipo inválido para sensor %s\n", nome_tipo);
            return 1;
        }
        strcpy(sensores[i].nome, nome_tipo);
    }

    const char *pasta = "./Arquivos_Gerados";
#ifdef _WIN32
    mkdir(pasta);
#else
    mkdir(pasta, 0755);
#endif

    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s/leitura.csv", pasta);

    FILE *arquivo = fopen(caminho_arquivo, "w");
    if (!arquivo) {
        perror("Erro ao criar arquivo");
        return 1;
    }

    int total_leituras = qtd_sensores * MAX_LEITURAS;
    Leitura *leituras = malloc(sizeof(Leitura) * total_leituras);
    if (!leituras) {
        printf("Erro de memória.\n");
        fclose(arquivo);
        return 1;
    }

    int idx = 0;
    for (int i = 0; i < qtd_sensores; i++) {
        for (int j = 0; j < MAX_LEITURAS; j++) {
            leituras[idx].timestamp = inicio + rand() % (fim - inicio + 1);
            strcpy(leituras[idx].sensor_nome, sensores[i].nome);
            strcpy(leituras[idx].valor, gerar_valor_aleatorio(sensores[i].tipo));
            idx++;
        }
    }

    embaralhar_leituras(leituras, total_leituras);

    for (int i = 0; i < total_leituras; i++) {
        fprintf(arquivo, "%ld %s %s\n", leituras[i].timestamp, leituras[i].sensor_nome, leituras[i].valor);
    }

    free(leituras);
    fclose(arquivo);

    printf("\033[1;32mArquivo gerado com sucesso\033[0m em: %s\n", caminho_arquivo);

    return 0;
}
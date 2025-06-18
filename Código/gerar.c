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

    int dia, mes, ano, hora, min, seg;

    int campos = sscanf(data_str, "%d/%d/%d-%d:%d:%d", &dia, &mes, &ano, &hora, &min, &seg);
    if (campos != 6) {
        fprintf(stderr, "\033[1;31mFormato de data/hora invalido. Use: dd/mm/yyyy-hh:mm:ss\033[0m\n");
        return (time_t)-1;
    }

    if (dia < 1 || dia > 30) {
        fprintf(stderr, "\033[1;31mDia invalido: %d. Use entre 01 e 30.\033[0m\n", dia);
        return (time_t)-1;
    }

    if (mes < 1 || mes > 12) {
        fprintf(stderr, "\033[1;31mMes invalido: %d. Use entre 01 e 12.\033[0m\n", mes);
        return (time_t)-1;
    }

    if (ano < 1900) {
        fprintf(stderr, "\033[1;31mAno invalido: %d. Use ano maior ou igual a 1900.\033[0m\n", ano);
        return (time_t)-1;
    }

    if (hora < 0 || hora > 23 || min < 0 || min > 59 || seg < 0 || seg > 59) {
        fprintf(stderr, "\033[1;31mHora invalida: %02d:%02d:%02d. Use valores entre 00:00:00 e 23:59:59.\033[0m\n", hora, min, seg);
        return (time_t)-1;
    }

    t.tm_mday = dia;
    t.tm_mon = mes - 1;
    t.tm_year = ano - 1900;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;

    time_t timestamp = mktime(&t);
    if (timestamp == (time_t)-1) {
        fprintf(stderr, "\033[1;31mErro ao converter data/hora para timestamp.\033[0m\n");
    }

    return timestamp;
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
            sprintf(buffer, "%.2f", ((float)rand() / RAND_MAX) * 100.0f);
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
    if (strcmp(tipo_str, "CONJ_Z") == 0) return TIPO_INT;
    if (strcmp(tipo_str, "BINARIO") == 0) return TIPO_BOOL;
    if (strcmp(tipo_str, "CONJ_Q") == 0) return TIPO_FLOAT;
    if (strcmp(tipo_str, "TEXTO") == 0) return TIPO_STRING;
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
        printf("\033[1;32mExemplo:\033[0m .\\%s 15/06/2025-00:00:00 15/06/2025-23:59:59 temp:CONJ_Z umidade:CONJ_Q alarme:BINARIO nome_sensor:TEXTO\n\n", execName);
        return 1;
    }

    srand((unsigned int)time(NULL));

    time_t inicio = converter_para_timestamp(argv[1]);
    time_t fim = converter_para_timestamp(argv[2]);
    if (inicio == (time_t)-1 || fim == (time_t)-1 || fim < inicio) {
        fprintf(stderr, "\033[1;31mIntervalo de datas invalido.\033[0m\n");
        return 1;
    }

    int qtd_sensores = argc - 3;
    if (qtd_sensores > MAX_SENSORES) {
        fprintf(stderr, "\033[1;31mLimite maximo de sensores (%d) ultrapassado.\033[0m\n", MAX_SENSORES);
        return 1;
    }

    Sensor sensores[MAX_SENSORES];
    for (int i = 0; i < qtd_sensores; i++) {
        char nome_tipo_tmp[64];
        strncpy(nome_tipo_tmp, argv[i + 3], sizeof(nome_tipo_tmp) - 1);
        nome_tipo_tmp[sizeof(nome_tipo_tmp) - 1] = '\0';

        char *tipo_str = strchr(nome_tipo_tmp, ':');
        if (!tipo_str) {
            fprintf(stderr, "\033[1;31mErro no argumento: '%s'. Esperado formato sensor:TIPO.\033[0m\n", argv[i + 3]);
            return 1;
        }

        *tipo_str = '\0';
        tipo_str++;

        if (strlen(nome_tipo_tmp) == 0 || strlen(tipo_str) == 0) {
            fprintf(stderr, "\033[1;31mFormato invalido em '%s'. Esperado sensor:TIPO, ex: temperatura:CONJ_Z\033[0m\n", argv[i + 3]);
            return 1;
        }

        sensores[i].tipo = obter_tipo(tipo_str);
        if (sensores[i].tipo == TIPO_INVALIDO) {
            fprintf(stderr, "\033[1;31mTipo invalido para sensor '%s': '%s'. Tipos validos: CONJ_Z, BINARIO, CONJ_Q, TEXTO\033[0m\n", nome_tipo_tmp, tipo_str);
            return 1;
        }

        strncpy(sensores[i].nome, nome_tipo_tmp, MAX_NOME);
        sensores[i].nome[MAX_NOME - 1] = '\0';
    }

    const char *pasta = "./Arquivos_Gerados";
#ifdef _WIN32
    mkdir(pasta);
#else
    mkdir(pasta, 0755);
#endif

    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s/arquivos_embaralhados.csv", pasta);

    FILE *arquivo = fopen(caminho_arquivo, "w");
    if (!arquivo) {
        perror("Erro ao criar arquivo");
        return 1;
    }

    int total_leituras = qtd_sensores * MAX_LEITURAS;
    Leitura *leituras = malloc(sizeof(Leitura) * total_leituras);
    if (!leituras) {
        fprintf(stderr, "\033[1;31mErro de memoria.\033[0m\n");
        fclose(arquivo);
        return 1;
    }

    for (int i = 0, idx = 0; i < qtd_sensores; i++) {
        for (int j = 0; j < MAX_LEITURAS; j++, idx++) {
            unsigned long range = (unsigned long)(fim - inicio) + 1;
            unsigned long offset = (unsigned long)(rand()) % range;

            leituras[idx].timestamp = inicio + offset;
            strncpy(leituras[idx].sensor_nome, sensores[i].nome, MAX_NOME);
            leituras[idx].sensor_nome[MAX_NOME - 1] = '\0';
            strncpy(leituras[idx].valor, gerar_valor_aleatorio(sensores[i].tipo), sizeof(leituras[idx].valor));
            leituras[idx].valor[sizeof(leituras[idx].valor) - 1] = '\0';
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

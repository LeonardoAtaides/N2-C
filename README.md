
# Compilação dos Programas

**Antes de iniciar a execução, certifique-se se esta na pasta `Código`, em seguida compile os programas com os seguintes comandos:**

```bash
gcc gerar.c -o gerar
gcc organizador.c -o organizador
gcc consultar.c -o consultar
```

---

## Etapas de Execução

###  Etapa 1 – Gerador de Arquivos (Programa 3)
_A lógica do programa 3, está neste arquivo:_
```bash
gerar.c
```
Execute o gerador com:

```bash
./gerar.exe *ou*
./gerar
```

Ele fornecerá instruções de uso, como demonstrado na imagem abaixo:

 **Exemplo de uso do Gerador**:  
[![Print Gerador](https://drive.google.com/uc?id=1koSHgnGWup4cY_0piE6_mH1sFw8Fga8m)](https://drive.google.com/file/d/1koSHgnGWup4cY_0piE6_mH1sFw8Fga8m/view?usp=drive_link)

Ao usar o comando corretamente, o programa criará uma pasta chamada `Arquivos_Gerados/`, contendo o arquivo:

```
arquivos_embaralhados.csv
```

---

### Etapa 2 – Organizador de Leituras (Programa 1)
_A lógica do programa 1, está neste arquivo:_
```bash
organizador.c
```
Execute o organizador com:

```bash
./organizador.exe *ou*
./organizador
```

Este programa irá ler os dados embaralhados e gerar automaticamente arquivos organizados para cada sensor dentro da pasta `Arquivos_Gerados/` dentro dela os sensores organizados serão salvos na subpasta `sensores_organizados`..

---

### Etapa 3 – Consulta por Instante (Programa 2)

Execute o programa de consulta com:
_A lógica do programa 2, está neste arquivo:_
```bash
consultar.c
```
Execute o consultar com:
```bash
./consulta.exe <nome_sensor> <data-hora> *ou*
./consulta <nome_sensor> <data-hora> *ou*
```
Se executado sem parâmetros, ele exibe uma orientação de uso semelhante à imagem abaixo:

**Exemplo de uso da Consulta**:  
[![Print Consulta](https://drive.google.com/uc?id=10X7-gtDYoHcWzcpWjjvxiYxpHvdUA0cc)](https://drive.google.com/file/d/10X7-gtDYoHcWzcpWjjvxiYxpHvdUA0cc/view?usp=drive_link)


Desenvolvido por: **Leonardo Ataídes**  


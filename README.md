
# Compilação dos Programas

**Antes de iniciar a execução, certifique-se se esta na pasta `Código`, em seguida compile os programas com os seguintes comandos:**

```bash
gcc gerar.c -o gerar
gcc organizador.c -o organizador
gcc consultar.c -o consulta
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
[![Print Gerador](https://drive.google.com/uc?id=1YVaNP9SGPJztgaCKvXODr7e0MczsBJsE)](https://drive.google.com/file/d/1YVaNP9SGPJztgaCKvXODr7e0MczsBJsE/view?usp=drive_link)

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

Este programa irá ler os dados embaralhados e gerar automaticamente arquivos organizados para cada sensor dentro da pasta `Arquivos_Gerados/` dentro dela os sensores organizados ficaram dentro de um subpasta com nome `sensores_organizados`..

---

### Etapa 3 – Consulta por Instante (Programa 2)

Execute o programa de consulta com:
_A lógica do programa 2, está neste arquivo:_
```bash
consulta.c
```
Execute o consultar com:
```bash
./consulta <id_sensor> <timestamp>
```

Se executado sem parâmetros, ele exibe uma orientação de uso semelhante à imagem abaixo:

**Exemplo de uso da Consulta**:  
[![Print Consulta](https://drive.google.com/uc?id=10FJIAeuHvAMZSBekeallZXOMTmlGQ03F)](https://drive.google.com/file/d/10FJIAeuHvAMZSBekeallZXOMTmlGQ03F/view?usp=drive_link)


Desenvolvido por: **Leonardo Ataídes**  


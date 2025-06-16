
# Sistema de Leitura de Sensores Industriais

Este projeto consiste em três programas desenvolvidos em C para simular a geração, organização e consulta de leituras de sensores em uma planta industrial inteligente.

---

## 📦 Compilação dos Programas

Antes de iniciar a execução, compile os programas com os seguintes comandos:

```bash
gcc gerador.c -o gerador
gcc organizador.c -o organizador
gcc consulta.c -o consulta
```

---

## 🔧 Etapas de Execução

### ✅ Etapa 1 – Gerador de Arquivos (Programa 3)

Execute o gerador com:

```bash
./gerador
```

Ele fornecerá instruções de uso, como demonstrado na imagem abaixo:

📷 **Exemplo de uso do Gerador**:  
[![Print Gerador](https://drive.google.com/uc?id=1YVaNP9SGPJztgaCKvXODr7e0MczsBJsE)](https://drive.google.com/file/d/1YVaNP9SGPJztgaCKvXODr7e0MczsBJsE/view?usp=drive_link)

Ao usar o comando corretamente, o programa criará uma pasta chamada `Arquivos_Gerados/`, contendo o arquivo:

```
arquivos_embaralhados.txt
```

---

### ✅ Etapa 2 – Organizador de Leituras (Programa 1)

Execute o organizador com:

```bash
./organizador
```

Este programa irá ler os dados embaralhados e gerar automaticamente arquivos organizados para cada sensor dentro da pasta `Arquivos_Gerados/`.

---

### ✅ Etapa 3 – Consulta por Instante (Programa 2)

Execute o programa de consulta com:

```bash
./consulta <id_sensor> <timestamp>
```

Se executado sem parâmetros, ele exibe uma orientação de uso semelhante à imagem abaixo:

📷 **Exemplo de uso da Consulta**:  
[![Print Consulta](https://drive.google.com/uc?id=10FJIAeuHvAMZSBekeallZXOMTmlGQ03F)](https://drive.google.com/file/d/10FJIAeuHvAMZSBekeallZXOMTmlGQ03F/view?usp=drive_link)

---

## 🗂️ Estrutura de Pastas e Arquivos

```
├── gerador.c
├── organizador.c
├── consulta.c
├── Arquivos_Gerados/
│   ├── arquivos_embaralhados.txt
│   ├── temp.csv
│   ├── umidade.csv
│   └── ... (outros sensores)
```

---

## 🧠 Observações Finais

- Os arquivos `.csv` gerados estão ordenados por `timestamp`.
- A consulta é feita com busca binária para garantir performance.
- O sistema é modular e de fácil integração com sistemas industriais simulados.

---

Desenvolvido por: **Leonardo**  
Faculdade N1 – Sistemas Embarcados

# Trabalhos Práticos de Estrutura de Dados III

## 📋 Contexto

Este projeto faz parte da disciplina **SCC0607 - Estrutura de Dados III** do Instituto de Ciências Matemáticas e de Computação (ICMC) da Universidade de São Paulo. O objetivo é implementar um sistema de gerenciamento de arquivos binários que simula operações de um banco de dados relacional aplicado ao contexto de **redes sociais**.

O sistema gerencia dados de pessoas e seus relacionamentos (quem segue quem) através de dois arquivos principais:
- **Arquivo Pessoa**: armazena informações sobre usuários da rede social
- **Arquivo Segue**: armazena relacionamentos entre pessoas

## 🎯 Objetivos

Os trabalhos práticos visam desenvolver competências em:

1. **Organização de Arquivos**
   - Manipulação de arquivos binários
   - Implementação de registros de tamanho fixo e variável
   - Gerenciamento de índices primários

2. **Operações de Banco de Dados**
   - Criação e manutenção de arquivos de dados
   - Inserção, remoção, busca e atualização de registros
   - Operações de junção (JOIN)

3. **Estruturas de Dados Avançadas**
   - Implementação de grafos direcionados
   - Representação através de listas de adjacências
   - Algoritmos de busca em grafos (BFS, detecção de ciclos)

---

## 📚 Funcionalidades Implementadas

### **Funcionalidade [1] - Criar Arquivo de Índice Primário**

**Descrição**: Cria um arquivo de índice primário vazio para o arquivo de dados pessoa.

**Sintaxe**:
```
1 arquivoIndicePrimario.bin
```

**Saída**: 
- Exibe o conteúdo do arquivo de índice criado usando `binarioNaTela()`
- Mensagem de erro: "Falha no processamento do arquivo."

---

### **Funcionalidade [2] - Criar Arquivo de Dados Pessoa a partir de CSV**

**Descrição**: Lê registros de um arquivo CSV e grava no arquivo binário pessoa, atualizando simultaneamente o índice primário.

**Sintaxe**:
```
2 arquivoEntrada.csv arquivoSaida.bin arquivoIndicePrimario.bin
```

**Saída**:
- Exibe o conteúdo dos arquivos pessoa.bin e indexaPessoa.bin usando `binarioNaTela()`
- Mensagem de erro: "Falha no processamento do arquivo."

---

### **Funcionalidade [3] - Listar Todos os Registros do Arquivo Pessoa**

**Descrição**: Recupera e exibe todos os registros não removidos do arquivo pessoa de forma organizada.

**Sintaxe**:
```
3 arquivoEntrada.bin
```

**Formato de Saída**:
```
Dados da pessoa de código [idPessoa]
Nome: [nomePessoa]
Idade: [idadePessoa]
Usuario: [nomeUsuario]

[linha em branco entre registros]
```

**Mensagens**:
- "Registro inexistente." (se não houver registros)
- "Falha no processamento do arquivo."

---

### **Funcionalidade [4] - Buscar Registros por Critério**

**Descrição**: Busca registros que satisfaçam um critério específico. Se a busca for por `idPessoa`, utiliza o índice primário (busca indexada). Caso contrário, realiza busca sequencial.

**Sintaxe**:
```
4 arquivoEntrada.bin arquivoIndicePrimario.bin n
1 nomeCampo=valorCampo
2 nomeCampo=valorCampo
...
n nomeCampo=valorCampo
```

**Exemplo**:
```
4 pessoa.bin indexaPessoa.bin 2
1 idPessoa=25
2 idade=13
```

**Observações**:
- Strings devem ser especificadas entre aspas duplas: `nomePessoa="MARIA SILVA"`
- Apenas um campo por busca (sem combinações)

**Saída**: Mesma formatação da funcionalidade [3]

---

### **Funcionalidade [5] - Remover Registros Logicamente**

**Descrição**: Remove logicamente registros do arquivo pessoa que satisfaçam um critério de busca, utilizando abordagem estática de reaproveitamento de espaços.

**Sintaxe**:
```
5 arquivoEntrada.bin arquivoIndicePrimario.bin n
1 nomeCampo1=valorCampo1
2 nomeCampo2=valorCampo2
...
n nomeCampon=valorCampon
```

**Exemplo**:
```
5 pessoa.bin indexaPessoa.bin 2
1 idPessoa=25
2 idade=NULO
```

**Observações**:
- Valores nulos são identificados com `NULO`
- Registros são marcados como removidos, mas os dados permanecem no arquivo

**Saída**: Exibe arquivos pessoa.bin e indexaPessoa.bin atualizados usando `binarioNaTela()`

---

### **Funcionalidade [6] - Inserir Novos Registros**

**Descrição**: Insere novos registros no arquivo pessoa, reaproveitando espaços de registros removidos quando possível.

**Sintaxe**:
```
6 arquivoEntrada.bin arquivoIndicePrimario.bin n
1 valor_idPessoa1, valor_nomePessoa1, valor_idade1, valor_nomeUsuario1
2 valor_idPessoa2, valor_nomePessoa2, valor_idade2, valor_nomeUsuario2
...
n valor_idPessoan, valor_nomePessoan, valor_idaden, valor_nomeUsuarion
```

**Exemplo**:
```
6 pessoa.bin indexaPessoa.bin 2
1 25, "SAMANTHA PEREIRA SANTOS", 13, "SAMANTHAPS"
2 45, "VITORIA PRADO CAMPOS", NULO, "VIVICA"
```

**Observações**:
- Não serão inseridos valores nulos ou repetidos para `idPessoa` e `nomeUsuario`
- Campos separados por vírgula

**Saída**: Exibe arquivos atualizados usando `binarioNaTela()`

---

### **Funcionalidade [7] - Atualizar Registros**

**Descrição**: Atualiza registros existentes. Se o novo registro for maior, remove o antigo e insere o novo. Se for menor ou igual, atualiza in-place.

**Sintaxe**:
```
7 arquivoEntrada.bin arquivoIndicePrimario.bin n
1 nomeCampoBusca1=valorCampoBusca1 nomeCampoAtualiza1=valorCampoAtualiza1
2 nomeCampoBusca2=valorCampoBusca2 nomeCampoAtualiza2=valorCampoAtualiza2
...
n nomeCampoBuscan=valorCampoBuscan nomeCampoAtualizan=valorCampoAtualizan
```

**Exemplo**:
```
7 pessoa.bin indexaPessoa.bin 2
1 idPessoa=25 nomePessoa="MARIANA PEREIRA"
2 idade=13 idade=20
```

**Observações**:
- Campo de busca e campo de atualização podem ser diferentes
- Lixo remanescente é marcado com '$'

**Saída**: Exibe arquivos atualizados usando `binarioNaTela()`

---

### **Funcionalidade [8] - Criar Arquivo Segue a partir de CSV**

**Descrição**: Lê registros de um arquivo CSV e cria o arquivo binário segue com registros de tamanho fixo.

**Sintaxe**:
```
8 arquivoEntrada.csv arquivoSaida.bin
```

**Exemplo**:
```
8 segue.csv segue.bin
```

**Estrutura do Arquivo Segue**:
- Registro de cabeçalho: status (1 byte), quantidadePessoas (4 bytes), proxRRN (4 bytes)
- Registros de dados (30 bytes cada):
  - removido (1 byte)
  - idPessoaQueSegue (4 bytes)
  - idPessoaQueESeguida (4 bytes)
  - dataInicioQueSegue (10 bytes)
  - dataFimQueSegue (10 bytes)
  - grauAmizade (1 byte): '0' = celebridade, '1' = amiga de amiga, '2' = amiga

**Saída**: Exibe arquivo segue.bin usando `binarioNaTela()`

---

### **Funcionalidade [9] - Ordenar Arquivo Segue**

**Descrição**: Ordena o arquivo segue em memória RAM e grava em um novo arquivo ordenado.

**Critérios de Ordenação** (em ordem de prioridade):
1. idPessoaQueSegue (crescente)
2. idPessoaQueESeguida (crescente) - em caso de empate
3. dataInicioQueSegue (crescente) - em caso de empate
4. dataFimQueSegue (crescente) - em caso de empate

**Sintaxe**:
```
9 arquivoDesordenado.bin arquivoOrdenado.bin
```

**Exemplo**:
```
9 segue.bin segueOrdenado.bin
```

**Saída**: Exibe arquivo segueOrdenado.bin usando `binarioNaTela()`

---

### **Funcionalidade [10] - Junção (JOIN) entre Pessoa e Segue**

**Descrição**: Realiza junção entre os arquivos pessoa e segueOrdenado baseada na condição `idPessoa = idPessoaQueSegue`. Utiliza busca binária no arquivo ordenado.

**Sintaxe**:
```
10 arquivoEntrada.bin arquivoIndicePrimario.bin arquivoOrdenado.bin n
1 nomeCampo1=valorCampo1
2 nomeCampo2=valorCampo2
...
n nomeCampon=valorCampon
```

**Exemplo**:
```
10 pessoa.bin indexaPessoa.bin segueOrdenado.bin 1
1 idPessoa=25
```

**Formato de Saída**:
```
Dados da pessoa de codigo 25
Nome: SAMANTHA PEREIRA CAMPOS
Idade: 13
Usuario: SAMANTHAPS

Segue a pessoa de codigo: 45
Justificativa para seguir: celebridade
Começou a seguir em: 01/01/2020
Parou de seguir em: 10/05/2025

Segue a pessoa de codigo: 29
Justificativa para seguir: minha amiga
Começou a seguir em: 03/05/2023
Parou de seguir em: 31/12/2024
```

**Observações**:
- Valores nulos exibidos como "-"
- Pode retornar 0, 1 ou múltiplos registros da junção

---

### **Funcionalidade [11] - Construir Grafo Direcionado**

**Descrição**: Cria um grafo direcionado representando relacionamentos "quem segue quem" usando listas de adjacências.

**Sintaxe**:
```
11 arquivoPessoa.bin arquivoIndexaPessoa.bin arquivoSegueOrdenado.bin
```

**Estrutura do Grafo**:
- **Vértices**: nomeUsuario das pessoas que seguem (ordenados alfabeticamente)
- **Arestas**: apontam para pessoas seguidas, contendo:
  - nomeUsuario da pessoa seguida
  - dataInicioQueSegue
  - dataFimQueSegue
  - grauAmizade
- Listas de adjacência ordenadas por nomeUsuario da pessoa seguida (em caso de empate, por dataInicio)

**Formato de Saída**:
```
PIETRA6, JULIA5, 15/06/2025, NULO, 2
PIETRA6, LUCASCARDOSO, 11/04/2024, NULO, 2

SCONCEICAO, ARTHUROLIVEIRA, 28/09/2022, NULO, 1
SCONCEICAO, MARIA60, 11/08/2021, NULO, 1
SCONCEICAO, MAYA83, 23/04/2022, 04/03/2024, 2
```

**Mensagem de erro**: "Falha na execução da funcionalidade."

---

### **Funcionalidade [12] - Construir Grafo Transposto**

**Descrição**: Gera o grafo transposto G^T do grafo da funcionalidade [11], invertendo a direção de todas as arestas.

**Sintaxe**:
```
12 arquivoPessoa.bin arquivoIndexaPessoa.bin arquivoSegueOrdenado.bin
```

**Estrutura do Grafo Transposto**:
- **Vértices**: nomeUsuario das pessoas que são seguidas (ordenados alfabeticamente)
- **Arestas**: apontam para pessoas que as seguem
- Representa "quem é seguido por quem"

**Formato de Saída**:
```
ARTHUROLIVEIRA, CAUAMOURA, 29/11/2021, NULO, 2
ARTHUROLIVEIRA, RYANMENDES, 19/07/2025, NULO, 0
ARTHUROLIVEIRA, SCONCEICAO, 28/09/2022, NULO, 1

MARIA60, AMONTEIRO, 22/10/2022, NULO, 0
MARIA60, ENZO7, 08/10/2025, NULO, 1
```

**Mensagem de erro**: "Falha na execução da funcionalidade."

---

### **Funcionalidade [13] - Caminho Mais Curto até Celebridade (BFS)**

**Descrição**: Determina o caminho mais curto de cada pessoa até uma celebridade específica, exibindo apenas caminhos de pessoas que ainda seguem a celebridade (dataFimQueSegue = NULO).

**Sintaxe**:
```
13 arquivoPessoa.bin arquivoIndexaPessoa.bin arquivoSegueOrdenado.bin "nomeUsuarioDaCelebridade"
```

**Exemplo**:
```
13 arquivoPessoa.bin arquivoIndexaPessoa.bin segueOrdenado.bin "ARTHUROLIVEIRA"
```

**Critério de Desempate**: Durante a execução do algoritmo, decisões sobre ordem de inserção/análise seguem:
1. Ordem alfabética por nomeUsuario
2. Em caso de empate: data de início
3. Em caso de empate: data de fim

**Formato de Saída**:
```
EVELYN7, SCONCEICAO, 07/08/2020, NULO, 2
SCONCEICAO, ARTHUROLIVEIRA, 28/09/2022, NULO, 1

STEPHANY6, MARIAH8, 31/05/2021, NULO, NULO
MARIAH8, NINA18, 11/10/2013, NULO, NULO
NINA18, ARTHUROLIVEIRA, 03/02/2024, NULO, 1
```

**Observações**:
- Se a pessoa não segue a celebridade: "NAO SEGUE A CELEBRIDADE"
- Algoritmo: BFS (Busca em Largura)

**Mensagem de erro**: "Falha na execução da funcionalidade."

---

### **Funcionalidade [14] - Detecção de Ciclo (Fofoca)**

**Descrição**: Determina o comprimento do menor caminho para que uma fofoca gerada por uma pessoa retorne a ela mesma (detecção de ciclo).

**Sintaxe**:
```
14 arquivoPessoa.bin arquivoIndexaPessoa.bin arquivoSegueOrdenado.bin "nomeUsuarioQueGerouFofoca"
```

**Exemplo**:
```
14 arquivoPessoa.bin arquivoIndexaPessoa.bin segueOrdenado.bin "ARTHUROLIVEIRA"
```

**Formato de Saída**:
- Se existe ciclo: exibe o comprimento do caminho (número inteiro)
  ```
  5
  ```
- Se não existe ciclo:
  ```
  A FOFOCA NAO RETORNOU
  ```

**Observações**:
- Mesmo que existam múltiplos caminhos, exibe apenas o primeiro encontrado
- Critério de desempate: mesmos da funcionalidade [13]

**Mensagem de erro**: "Falha na execução da funcionalidade."

---

## 🛠️ Tutorial - Como Compilar e Executar

### **Pré-requisitos**

- Compilador GCC instalado
- Sistema operacional Linux ou ambiente compatível (WSL no Windows)
- Make instalado

### **Estrutura de Arquivos do Projeto**

```
projeto/
├── *.c              # Arquivos fonte em C
├── *.h              # Arquivos de cabeçalho
├── Makefile         # Script de compilação
├── pessoa.csv       # Arquivo CSV de entrada (pessoas)
└── segue.csv        # Arquivo CSV de entrada (relacionamentos)
```

---

### **Passo 1: Compilação**

O projeto utiliza um Makefile para automatizar a compilação. Execute no terminal:

```bash
make all
```

**O que acontece:**
- O Makefile identifica automaticamente todos os arquivos `.c`
- Compila cada arquivo `.c` em um arquivo objeto `.o`
- Liga todos os objetos para criar o executável `programaTrab`
- Flags de compilação: `-Wall -Wextra -std=c99` (avisos e padrão C99)

**Saída esperada:**
```
gcc -Wall -Wextra -Iinclude -std=c99 -c arquivo1.c -o arquivo1.o
gcc -Wall -Wextra -Iinclude -std=c99 -c arquivo2.c -o arquivo2.o
...
gcc -Wall -Wextra -Iinclude -std=c99 arquivo1.o arquivo2.o ... -o programaTrab
```

---

### **Passo 2: Execução**

O programa lê comandos da **entrada padrão**. Existem duas formas de executar:

#### **Opção 1: Executar diretamente com Make**

```bash
make run
```

Isso executa `./programaTrab` e aguarda entrada do usuário.

#### **Opção 2: Executar o binário diretamente**

```bash
./programaTrab
```

---

### **Passo 3: Fornecendo Comandos**

Após executar o programa, digite os comandos no terminal seguindo a sintaxe de cada funcionalidade.

**Exemplo completo de fluxo de trabalho:**

```bash
# 1. Compilar o programa
make all

# 2. Executar
./programaTrab

# 3. Digitar comandos (exemplo: criar índice)
1 indexaPessoa.bin

# 4. O programa processa e exibe a saída
```

---

### **Exemplos Práticos de Uso**

#### **Exemplo 1: Workflow Completo - Arquivo Pessoa**

```bash
./programaTrab
# Comando 1: Criar arquivo de índice
1 indexaPessoa.bin

./programaTrab
# Comando 2: Criar arquivo de dados a partir do CSV
2 pessoa.csv pessoa.bin indexaPessoa.bin

./programaTrab
# Comando 3: Listar todos os registros
3 pessoa.bin

./programaTrab
# Comando 4: Buscar por ID específico
4 pessoa.bin indexaPessoa.bin 1
1 idPessoa=25
```

#### **Exemplo 2: Inserção e Atualização**

```bash
./programaTrab
# Inserir 2 novos registros
6 pessoa.bin indexaPessoa.bin 2
1 100, "JOAO DA SILVA", 25, "JOAOSILVA"
2 101, "MARIA SANTOS", 30, "MARIASANTOS"

./programaTrab
# Atualizar idade de uma pessoa
7 pessoa.bin indexaPessoa.bin 1
1 idPessoa=100 idade=26
```

#### **Exemplo 3: Workflow Completo - Arquivo Segue e Grafos**

```bash
./programaTrab
# Criar arquivo segue
8 segue.csv segue.bin

./programaTrab
# Ordenar arquivo segue
9 segue.bin segueOrdenado.bin

./programaTrab
# Criar grafo direcionado
11 pessoa.bin indexaPessoa.bin segueOrdenado.bin

./programaTrab
# Encontrar caminho até celebridade
13 pessoa.bin indexaPessoa.bin segueOrdenado.bin "ARTHUROLIVEIRA"
```

#### **Exemplo 4: Usando Redirecionamento de Entrada**

Para facilitar testes, você pode criar um arquivo de texto com os comandos:

**arquivo `comandos.txt`:**
```
1 indexaPessoa.bin
```

**Executar:**
```bash
./programaTrab < comandos.txt
```

Ou usar pipes:
```bash
echo '3 pessoa.bin' | ./programaTrab
```

---

### **Passo 4: Limpeza de Arquivos Compilados**

Para remover arquivos objeto e executável:

```bash
make clean
```

Ou manualmente:
```bash
rm -f programaTrab *.o
```

---

### **Estrutura do Makefile Explicada**

```makefile
# Variáveis de configuração
CC = gcc                           # Compilador a ser usado
CFLAGS = -Wall -Wextra -Iinclude -std=c99  # Flags de compilação
TARGET = programaTrab              # Nome do executável final
SRCS = $(wildcard *.c)            # Lista todos os .c automaticamente
OBJS = $(SRCS:.c=.o)              # Converte .c em .o
HEADERS = $(wildcard *.h)         # Lista todos os .h automaticamente

# Regra padrão (executada com 'make' ou 'make all')
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Regra para compilar cada .c em .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para executar o programa
run: $(TARGET)
	./$(TARGET)

# Marca 'all' e 'run' como alvos falsos (não são arquivos)
.PHONY: all run clean
```

---

### **Dicas e Boas Práticas**

1. **Sempre compile antes de executar**:
   ```bash
   make all && ./programaTrab
   ```

2. **Verificar erros de compilação**: O Makefile usa `-Wall -Wextra` para mostrar todos os avisos. Corrija-os!

3. **Teste incremental**: Teste cada funcionalidade individualmente antes de integrar

4. **Arquivos de entrada**: Certifique-se de que `pessoa.csv` e `segue.csv` estejam no mesmo diretório

5. **Debugging**: Use `gdb` para depurar:
   ```bash
   gcc -g *.c -o programaTrab
   gdb ./programaTrab
   ```

6. **Verificar arquivos gerados**: Após executar comandos que criam arquivos `.bin`, verifique se foram criados:
   ```bash
   ls -lh *.bin
   ```

---

## 📝 Observações Importantes

### **Restrições Gerais**

1. **Modo de Arquivo**: Todos os arquivos devem ser manipulados em modo binário
2. **Ordem dos Campos**: Deve seguir estritamente as especificações
3. **Valores Nulos**:
   - Inteiros: representados por `-1`
   - Strings de tamanho fixo: preenchidas com `$`
   - Strings de tamanho variável: indicador de tamanho = 0
4. **Lixo**: Sempre identificado pelo caractere `$`
5. **Escrita de Dados**: Deve ser feita campo a campo (não registro completo)

### **Arquivo Pessoa**

- **Registro de Cabeçalho**: 17 bytes
  - status (1 byte)
  - quantidadePessoas (4 bytes)
  - quantidadeRemovidos (4 bytes)
  - proxByteOffset (8 bytes)

- **Registros de Dados**: Tamanho variável
  - Campos fixos: removido (1), tamanhoRegistro (4), idPessoa (4), idadePessoa (4)
  - Campos variáveis: nomePessoa e nomeUsuario (com indicadores de tamanho de 4 bytes cada)

### **Arquivo Segue**

- **Registro de Cabeçalho**: 9 bytes
  - status (1 byte)
  - quantidadePessoas (4 bytes)
  - proxRRN (4 bytes)

- **Registros de Dados**: 30 bytes (tamanho fixo)
  - removido (1), idPessoaQueSegue (4), idPessoaQueESeguida (4)
  - dataInicioQueSegue (10), dataFimQueSegue (10), grauAmizade (1)

### **Arquivo de Índice Primário**

- **Registro de Cabeçalho**: 12 bytes (1 byte de status + 11 bytes de lixo '$')
- **Registros de Dados**: 12 bytes cada
  - idPessoa (4 bytes)
  - byteOffset (8 bytes)
- **Ordenação**: Crescente por idPessoa

---

## 👥 Autores

**Disciplina**: SCC0607 - Estrutura de Dados III  
**Instituição**: ICMC - USP  

**Desenvolvedores**:
- Matheus Soares Falango
- Murilo Gonzales Vieira

---

**Bom Trabalho!** 🚀

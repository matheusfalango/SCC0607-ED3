# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99 -ggdb

# Diretórios
APP_DIR = apps
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
MAKE_DIR = make
HEADERS_DIR = include

# Nome do zip
ZIPNAME = programaTrab

# Arquivos
TARGET = $(APP_DIR)/programaTrab
SRCS = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(HEADERS_DIR)/*.h)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
MAKEFILE = $(MAKE_DIR)/Makefile

# Arquivos de dados a serem limpos
DATA_FILES = pessoa.bin indexaPessoa.bin segue.bin ordenado.bin

# Regra padrão
all: $(TARGET)

# Cria o executável
$(TARGET): $(OBJS) | $(APP_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compila arquivos .c em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Cria diretórios se não existirem
$(APP_DIR):
	mkdir -p $(APP_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpa arquivos compilados e dados
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(TARGET)
	rm -f $(DATA_FILES)

# Cria um arquivo .zip para envio
zip:
	rm -f $(ZIPNAME).zip
	zip -j $(ZIPNAME).zip $(SRC_DIR)/*.c $(HEADERS_DIR)/*.h $(MAKEFILE)

# Executa o programa
run: $(TARGET)
	./$(TARGET)

gdb: $(TARGET)
	gdb $(TARGET)

.PHONY: all clean run zip
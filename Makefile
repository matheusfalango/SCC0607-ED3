# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99

# Diretórios
APP_DIR = apps
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = include

# Arquivos
TARGET = $(APP_DIR)/programaTrab
SRCS = $(SRC_DIR)/programaTrab.c $(SRC_DIR)/utilidades.c $(SRC_DIR)/ArvAVL.c
OBJS = $(OBJ_DIR)/programaTrab.o $(OBJ_DIR)/utilidades.o $(OBJ_DIR)/ArvAVL.o
HEADERS = $(INCLUDE_DIR)/ArvAVL.h $(INCLUDE_DIR)/programaTrab.h $(INCLUDE_DIR)/utilidades.h

# Arquivos de dados a serem limpos
DATA_FILES = pessoa.bin indexaPessoa.bin

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
	if not exist $(APP_DIR) mkdir $(APP_DIR)

$(BIN_DIR):
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)

$(OBJ_DIR):
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

# Limpa arquivos compilados e dados
clean:
	if exist $(OBJ_DIR)\*.o del -f $(OBJ_DIR)\*.o
	if exist $(TARGET) del -f $(TARGET)
	if exist *.bin del -f -q *.bin

# Executa o programa
run: $(TARGET) | $(BIN_DIR)
	./$(TARGET)

.PHONY: all clean run
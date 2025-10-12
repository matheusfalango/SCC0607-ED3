# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99

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
SRCS = $(SRC_DIR)/programaTrab.c $(SRC_DIR)/utilidades.c $(SRC_DIR)/ArvAVL.c
OBJS = $(OBJ_DIR)/programaTrab.o $(OBJ_DIR)/utilidades.o $(OBJ_DIR)/ArvAVL.o
HEADERS = $(HEADERS_DIR)/ArvAVL.h $(HEADERS_DIR)/programaTrab.h $(HEADERS_DIR)/utilidades.h
MAKEFILE = $(MAKE_DIR)/Makefile

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

# Cria um arquivo .zip para enviar o trabalho
zip:
	if exist $(ZIPNAME).zip del $(ZIPNAME).zip
	powershell -NoProfile -Command "& {Compress-Archive -Path '$(SRC_DIR)\*.c','$(HEADERS_DIR)\*.h','$(MAKEFILE)' -DestinationPath '$(ZIPNAME).zip' -Force}"

# Executa o programa
run: $(TARGET) | $(BIN_DIR)
	./$(TARGET)

.PHONY: all clean run zip
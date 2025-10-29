# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99

# Arquivos
TARGET = programaTrab
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
HEADERS = $(wildcard *.h)

# Regra padrão
all: $(TARGET)

# Cria o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compila arquivos .c em .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Executa o programa
run: $(TARGET)
	./$(TARGET)

.PHONY: all run
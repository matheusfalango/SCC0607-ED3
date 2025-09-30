CC = gcc
CFLAGS = -Wall -Wextra
TARGET = programaTrab

SRCS = programaTrab.c utilidades.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) pessoa.bin indexaPessoa.bin

.PHONY: all clean

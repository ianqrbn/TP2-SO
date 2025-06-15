CC = gcc

CFLAGS = -g -Wall -Wextra -std=c99

TARGET = tp2virtual

SOURCES_C = tp2virtual.c

HEADERS = tpi.h substituicao.h estruturas.h

all: $(TARGET)

$(TARGET): $(SOURCES_C) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES_C)

clean:
	rm -f $(TARGET)

run_test: $(TARGET)
	./$(TARGET) segunda_chance compressor.log 4 128
TARGET = RC4
CC = gcc
CXX = g++
RM = rm -f
SRC = RC4.c

STREAM_BYTE_LEN = 100

ARG1 = $(STREAM_BYTE_LEN)

.PHONY: all clean run
all: $(SRC)
	@$(CC) -o $(TARGET) $(SRC)

clean: 
	@$(RM) $(OBJ)

run:
	@./$(TARGET) $(ARG1)

rerun: clean all run
TARGET = SimpleSubCipher
CC = gcc
CXX = g++
RM = rm -f
SRC = simple_sub_cipher.c

BIGRAM_FILE = english_bigrams_1.txt
CIPHER_TEXT = ciphertext.txt
TIME_OUT = 5000

ARG1 = $(BIGRAM_FILE)
ARG2 = $(CIPHER_TEXT)
ARG3 = $(TIME_OUT)

.PHONY: all clean run
all: $(SRC)
	@$(CC) -o $(TARGET) $(SRC)

clean: 
	@$(RM) $(OBJ)

run:
	@./$(TARGET) $(ARG1) $(ARG2) $(ARG3)

rerun: clean all run
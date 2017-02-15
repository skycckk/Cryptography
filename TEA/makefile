TARGET = TEA
CC = gcc
CXX = g++
RM = rm -f
SRC = TEA.c

IN_IMAGE = alice.bmp
OUT_IMAGE = alice_out.bmp

ARG1 = $(IN_IMAGE)
ARG2 = $(OUT_IMAGE)

.PHONY: all clean run
all: $(SRC)
	@$(CC) -o $(TARGET) $(SRC)

clean: 
	@$(RM) $(OBJ)

run:
	@./$(TARGET) $(ARG1) $(ARG2)

rerun: clean all run
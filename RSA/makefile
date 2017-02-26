TARGET = RSA_CRT
CC = gcc
CXX = g++
LIBS = -L$(LIB) -lgmp
LIB = /usr/local/lib
INCLUDE = /usr/local/include
CFLAGS = -I$(INCLUDE)
RM = rm -f
SRC = RSA_CRT.c

.PHONY: all clean run
all: $(SRC)
	@$(CC) -o $(TARGET) $(SRC) $(CFLAGS) $(LIBS)

clean:
	@$(RM) $(OBJ)

run:
	@./$(TARGET)

rerun: clean all run

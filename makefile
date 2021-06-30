CC=gcc
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
# LIB=$(addprefix lib,$(OBJ:.o=.a))

all: $(OBJ) main clean

main: $(OBJ)
	$(CC) -g $^ -o $@ -lglfw -lGL -lGLEW

$(OBJ): %.o: %.c
	$(CC) -g -c $< -lglfw -lGL -lGLEW

clean: $(OBJ)
	rm $^
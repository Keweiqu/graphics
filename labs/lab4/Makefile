ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/opt/ports/lib -framework OpenGL -lglfw -lGLEW -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglut -lGLEW -lglfw -lm
endif

CC = gcc
CFLAGS=-g -I/opt/ports/include 

INIT_SHADER = initshader.o

example1: example1.o $(INIT_SHADER) common.h
	$(CC) $@.o $(INIT_SHADER) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 


clean: 
	-rm -r example1 *.o core *.dSYM

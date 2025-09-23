makefile

CC = gcc 
CFLAGS = -Wall -g

all: shell

shell; shell.o  p0.o
      $(CC) $(CLAG) -o shell shell.o p0.o
      
      
p0.o: p0.c
    $(CC) $(CFLAGS) -c p0.c
    
    
shell.o: shell.c commands.h p0.h
      $(CC) $(CFLAGS) -c shell.c
      
run: shell 
      ./shell
      
valgrind: shell
    valgrind -leak-check-=full ./shell


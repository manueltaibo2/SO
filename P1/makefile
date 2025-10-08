
#Antón Vázquez López anton.vazquez.lopez@udc.es
#Manuel Taibo González manuel.taibo2@udc.es


CC = gcc
CFLAGS = -Wall -g
SRCS = p1.c comandos.c list.c
OBJS = $(SRCS:.c=.o)
TARGET = a.out

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

clean_obj:
	rm -f $(OBJS)

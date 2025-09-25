#Antón Vázquez López anton.vazquez.lopez@udc.es
#Manuel Taibo González manuel.taibo2@udc.es


CC = gcc
CFLAGS = -Wall -g
SRCS = p0.c list.c
OBJS = $(SRCS:.c=.o)
TARGET = shell

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

clean_obj:
	rm -f $(OBJS)

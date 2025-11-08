CC = gcc
CFLAGS = -Wall

all: serveur client

serveur: serveur.c
	$(CC) $(CFLAGS) -o serveur serveur.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f serveur client
	rm -f /tmp/fifo1 /tmp/fifo2

.PHONY: all clean
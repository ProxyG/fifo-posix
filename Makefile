CC = gcc
CFLAGS = -Wall
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)

all: serveur client gui

serveur: serveur.c
	$(CC) $(CFLAGS) -o serveur serveur.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

gui: gui.c
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -o gui gui.c $(GTK_LIBS)

clean:
	rm -f serveur client gui
	rm -f /tmp/fifo1 /tmp/fifo2

.PHONY: all clean
CC=gcc
CFLAGS=-c -Wall

objects = Producer.o Client.o SOIUtility.o main.o
 
all: $(objects)
	$(CC) $(objects) -o app

$(objects): %.o: %.c
	$(CC)  $(CFLAGS)  $< -o $@

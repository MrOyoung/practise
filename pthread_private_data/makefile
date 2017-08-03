CC=gcc

CSOURCE:=
CSOURCE+=main.c
CSOURCE+=thread1.c
CSOURCE+=thread2.c

CFLAGS:=-g -Wall

LIBS:=-pthread

target=local_variable

all:
	$(CC) $(CSOURCE) -o $(target) $(CFLAGS) $(LIBS)

clean:
	rm $(target)

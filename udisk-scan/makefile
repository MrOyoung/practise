#2017/08/08

CC=gcc

target=udisk_scan

CSOURCE:=
CSOURCE+=main.c
CSOURCE+=usb.c

CFLAGS:=-Wall

LIBS:=

all:$(CSOURCE)
	$(CC) -o $(target) $^ $(CFLAGS) $(LIBS)

clean:
	rm $(target)

CC=gcc

CSOURCE:=
CSOURCE+=dbus_low_level.c

CFLAGS:=
CFLAGS+=-I/usr/local/include/dbus-1.0
CFLAGS+=-I/usr/local/lib/dbus-1.0/include
CFLAGS+=-O2

LIBS:=
LIBS+=-ldbus-1


target1=client
target2=server


all:
	$(CC) -o $(target1) $(target1).c $(CSOURCE)  $(CFLAGS) $(LIBS)
	$(CC) -o $(target2) $(target2).c $(CSOURCE) $(CFLAGS) $(LIBS)

clean:
	rm $(target1) $(target2)

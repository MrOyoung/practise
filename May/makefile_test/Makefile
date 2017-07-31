CC=/zeos_sdk/toolchain/usr/bin/arm-linux-gcc
CPP=/zeos_sdk/toolchain/usr/bin/arm-linux-g++
TARGET=testapp

SRC_C:=main.c app.c dir1/app1.c
SRC_CC:=app_b.cc dir2/app2.cc

OBJECTS_C:=$(SRC_C:.c=.o)
OBJECTS_CC:=$(SRC_CC:.cc=.o)

all:$(OBJECTS_C) $(OBJECTS_CC)
	$(CC) $(OBJECTS_C) $(OBJECTS_CC) -o $(TARGET)

$(OBJECTS_C):%.o:%.c
	$(CC) -c $< -o $@
$(OBJECTS_CC):%.o:%.cc
	$(CPP) -c $< -o $@

vpath %.c ../../util

#参加编译的文件
#如有新文件添加请在此添加
objects:=main.o
objects+=uart.o
objects+=aio_read.o

#最终生成的应用程序
target:=ipc_uart

#编译器
CC:=arm-poky-linux-gnueabi-gcc

#编译选项
CFLAGS:=
CFLAGS+=-mfloat-abi=hard -mfpu=neon

CFLAGS+=-mtune=cortex-a9

CFLAGS+=-I../../include
CFLAGS+=-O2
CFLAGS+=-lrt


#需要链接的库
LIBS:=
LIBS+=
###################################################################################################

#生成应用程序
$(target):$(objects)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	@if [ ! -z $(BINDIR) ] ; then \
		mv -f $@ $(BINDIR)/$@; \
	fi

#包含依赖文件
include $(objects:.o=.d)

#生成相关的依赖文件
%.d:%.c
	@echo create depend file $@...
	@$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ : ]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

#清除所有的中间文件
.PHONY:clean
clean:
	rm -fr $(target) $(objects) $(objects:.o=.d)

CC=gcc
target=uart_aio_read

SOURCE := aio_read_man.c
SOURCE += aio_read_man.h

CFLAG := -lrt

DFLAG := -D_AIO_MAN

$(target):$(SOURCE)
	$(CC) $^ -o $@ $(CFLAG) $(DFLAG)

clean:
	rm -rf $(target)

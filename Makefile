CC = arm-none-linux-gnueabi-gcc
CFLAGS = -Wall -O2
LDFLAGS = -linkview

all: fbpad
%.o: %.c config.h
	$(CC) -c $(CFLAGS) $<
fbpad: fbpad.o term.o pad.o draw.o font.o isdw.o scrsnap.o
	$(CC) -o $@ $^ $(LDFLAGS)
clean:
	rm -f *.o fbpad

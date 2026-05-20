CC ?= gcc
CFLAGS = -Wall -O2

all: rfid_app

rfid_app: main.o ssd1306_i2c.o
	$(CC) -o rfid_app main.o ssd1306_i2c.o

main.o: main.c ssd1306_i2c.h
	$(CC) $(CFLAGS) -c main.c

ssd1306_i2c.o: ssd1306_i2c.c ssd1306_i2c.h
	$(CC) $(CFLAGS) -c ssd1306_i2c.c

clean:
	rm -f *.o rfid_app

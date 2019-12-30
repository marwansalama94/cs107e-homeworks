
ARMGNU ?= aarch64-none-elf

AOPS = --warn --fatal-warnings
COPS = -g -Wall -Og -std=gnu99 -nostartfiles -ffreestanding

gcc : app.bin

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.srec
	rm -f *.elf
	rm -f *.list
	rm -f *.img

start.o : start.s
	$(ARMGNU)-as $(AOPS) start.s -o start.o

app.o : app.c
	$(ARMGNU)-gcc $(COPS) -c app.c -o app.o

gpio.o : libs/gpio.c 
	$(ARMGNU)-gcc $(COPS) -c libs/gpio.c -o libs/gpio.o

cstart.o : cstart.c
	$(ARMGNU)-gcc $(COPS) -c cstart.c -o cstart.o

timer.o : libs/timer.c
	$(ARMGNU)-gcc $(COPS) -c libs/timer.c -o libs/timer.o

printf.o : libs/printf.c
	$(ARMGNU)-gcc $(COPS) -c libs/printf.c -o libs/printf.o

strings.o : libs/strings.c
	$(ARMGNU)-gcc $(COPS) -c libs/strings.c -o libs/strings.o

uart.o : libs/uart.c
	$(ARMGNU)-gcc $(COPS) -c libs/uart.c -o libs/uart.o

backtrace.o : backtrace.c
	$(ARMGNU)-gcc $(COPS) -c backtrace.c -o backtrace.o

malloc.o : malloc.c
	$(ARMGNU)-gcc $(COPS) -c malloc.c -o malloc.o


app.bin : memmap malloc.o start.o gpio.o app.o cstart.o timer.o backtrace.o strings.o printf.o uart.o
	$(ARMGNU)-ld start.o cstart.o libs/gpio.o libs/timer.o libs/strings.o libs/printf.o backtrace.o malloc.o libs/uart.o app.o -T memmap -o app.elf
	$(ARMGNU)-objdump -D app.elf > app.list
	$(ARMGNU)-objcopy app.elf -O binary app.bin



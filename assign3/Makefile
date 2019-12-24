
ARMGNU ?= aarch64-none-elf

AOPS = --warn --fatal-warnings
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

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

gpio.o : gpio.c 
	$(ARMGNU)-gcc $(COPS) -c gpio.c -o gpio.o

cstart.o : cstart.c
	$(ARMGNU)-gcc $(COPS) -c cstart.c -o cstart.o

timer.o : timer.c
	$(ARMGNU)-gcc $(COPS) -c timer.c -o timer.o

printf.o : printf.c
	$(ARMGNU)-gcc $(COPS) -c printf.c -o printf.o

strings.o : strings.c
	$(ARMGNU)-gcc $(COPS) -c strings.c -o strings.o

uart.o : uart.c
	$(ARMGNU)-gcc $(COPS) -c uart.c -o uart.o

app.bin : memmap start.o gpio.o app.o cstart.o timer.o strings.o printf.o uart.o
	$(ARMGNU)-ld start.o cstart.o gpio.o timer.o strings.o printf.o uart.o app.o -T memmap -o app.elf
	$(ARMGNU)-objdump -D app.elf > app.list
	$(ARMGNU)-objcopy app.elf -O binary app.bin



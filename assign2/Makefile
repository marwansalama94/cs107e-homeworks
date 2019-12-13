
ARMGNU ?= aarch64-none-elf

AOPS = --warn --fatal-warnings
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : clock.bin

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

clock.o : apps/clock.c
	$(ARMGNU)-gcc $(COPS) -c apps/clock.c -o clock.o

gpio.o : gpio.c 
	$(ARMGNU)-gcc $(COPS) -c gpio.c -o gpio.o

cstart.o : cstart.c
	$(ARMGNU)-gcc $(COPS) -c cstart.c -o cstart.o
timer.o : timer.c
	$(ARMGNU)-gcc $(COPS) -c timer.c -o timer.o

clock.bin : memmap start.o gpio.o clock.o cstart.o timer.o
	$(ARMGNU)-ld start.o cstart.o gpio.o timer.o clock.o -T memmap -o clock.elf
	$(ARMGNU)-objdump -D clock.elf > clock.list
	$(ARMGNU)-objcopy clock.elf -O binary clock.bin



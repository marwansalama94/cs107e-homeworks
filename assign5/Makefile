
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
	rm -f libs/*.o

start.o : start.s
	$(ARMGNU)-as $(AOPS) start.s -o start.o

app.o : scan.c
	$(ARMGNU)-gcc $(COPS) -c scan.c -o app.o

keyboard.o : keyboard.c
	$(ARMGNU)-gcc $(COPS) -c keyboard.c -o keyboard.o

gpio.o : libs/gpio.c 
	$(ARMGNU)-gcc $(COPS) -c libs/gpio.c -o libs/gpio.o

ps2.o : libs/ps2.c 
	$(ARMGNU)-gcc $(COPS) -c libs/ps2.c -o libs/ps2.o

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

malloc.o : libs/malloc.c
	$(ARMGNU)-gcc $(COPS) -c libs/malloc.c -o libs/malloc.o

pi.o : pi.c
	$(ARMGNU)-gcc $(COPS) -c pi.c -o pi.o

shell.o : shell.c
	$(ARMGNU)-gcc $(COPS) -c shell.c -o shell.o

shell_commands.o : shell_commands.c
	$(ARMGNU)-gcc $(COPS) -c shell_commands.c -o shell_commands.o


app.bin : memmap start.o gpio.o app.o cstart.o timer.o strings.o shell_commands.o shell.o printf.o uart.o malloc.o pi.o keyboard.o ps2.o
	$(ARMGNU)-ld start.o pi.o cstart.o libs/gpio.o shell_commands.o libs/malloc.o keyboard.o libs/ps2.o libs/timer.o libs/strings.o shell.o libs/printf.o libs/uart.o app.o -T memmap -o app.elf
	$(ARMGNU)-objdump -D app.elf > app.list
	$(ARMGNU)-objcopy app.elf -O binary app.bin



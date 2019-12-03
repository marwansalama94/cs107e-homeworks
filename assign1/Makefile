
ARMGNU ?= aarch64-none-elf

AOPS = --warn --fatal-warnings
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : larson.bin

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.list

larson.o : larson.s
	$(ARMGNU)-as $(AOPS) larson.s -o larson.o


larson.bin : larson.o 
	$(ARMGNU)-objcopy larson.o -O binary larson.bin



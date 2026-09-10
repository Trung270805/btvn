TARGET = main

CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CFLAGS = -mcpu=cortex-m3 \
         -mthumb \
         -O0 \
         -Wall \
         -Wextra \
         -ffreestanding \
         -fno-builtin

LDFLAGS = -T linker.ld \
          -nostdlib \
          -Wl,--gc-sections

all: $(TARGET).bin

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

startup.o: startup.s
	$(AS) $(CFLAGS) -c startup.s -o startup.o

$(TARGET).elf: startup.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) startup.o main.o -o $(TARGET).elf

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin
	$(SIZE) $(TARGET).elf

clean:
	rm -f *.o *.elf *.bin

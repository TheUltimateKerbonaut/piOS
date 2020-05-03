#SRCS = $(wildcard *.c)
#OBJS = $(SRCS:.c=.o)
SRCS := $(wildcard *.c io/*.c gfx/*.c)
OBJS := $(SRCS:.c=.o)
CFLAGS = -Wall -O2 -ffreestanding -nostdlib -nostartfiles

all: kernel8.img

boot.o: boot.s
	aarch64-elf-gcc $(CFLAGS) -c boot.s -o boot.o

%.o: %.c
	aarch64-elf-gcc $(CFLAGS) -c $< -o $@

kernel8.img: boot.o $(OBJS)

	aarch64-elf-ld -nostdlib -nostartfiles boot.o $(OBJS) -T linker.ld -o kernel8.elf
	aarch64-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf kernel8.img *.o io/*.o gfx/*.o >/dev/null 2>/dev/null || true

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial stdio

transfer:
	../raspbootin/raspbootcom/raspbootcom /dev/ttyUSB0 ~/piOS/kernel8.img 
	#java -jar ../Raspbootin64Client-master/Raspbootin64Client.jar /dev/ttyUSB0 ../piOS/kernel8.img

emulate:
	qemu-system-aarch64 -M raspi3 -serial stdio -kernel kernel8.img
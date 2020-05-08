#SRCS = $(wildcard *.c)
#OBJS = $(SRCS:.c=.o)
SRCS := $(wildcard *.c io/*.c gfx/*.c multitask/*.c memory/*.c)
OBJS := $(SRCS:.c=.o)
CFLAGS = -Wall -O2 -ffreestanding -nostdlib -nostartfiles -fno-common

all: kernel8.img

boot.o: boot.S
	aarch64-elf-gcc $(CFLAGS) -c boot.S -o boot.o

io/exceptionLevelA.o: io/exceptionLevel.S
	aarch64-elf-gcc $(CFLAGS) -c io/exceptionLevel.S -o io/exceptionLevelA.o


%.o: %.c
	aarch64-elf-gcc $(CFLAGS) -c $< -o $@

kernel8.img: boot.o io/exceptionLevelA.o $(OBJS)

	aarch64-elf-ld -Map=output.map  -z max-page-size=0x1000 -nostdlib -nostartfiles boot.o $(OBJS) io/exceptionLevelA.o -T linker.ld -o kernel8.elf
	aarch64-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf kernel8.img *.o io/*.o gfx/*.o multitask/*.o memory/*.o >/dev/null 2>/dev/null || true
	rm output.map

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial stdio

transfer: kernel8.img
	../raspbootin/raspbootcom/raspbootcom /dev/ttyUSB0 ~/piOS/kernel8.img 
	#java -jar ../Raspbootin64Client-master/Raspbootin64Client.jar /dev/ttyUSB0 ../piOS/kernel8.img

emulate: kernel8.img
	qemu-system-aarch64 -M raspi3 -serial stdio -kernel kernel8.img #-d int
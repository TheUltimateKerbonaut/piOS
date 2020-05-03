aarch64-elf-as -c boot.s -o boot.o
aarch64-elf-gcc -std=gnu99 -ffreestanding -c kernel.c -o kernel.o -O2 -Wall -Wextra

aarch64-elf-gcc -T linker.ld -o myos.elf -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc 
aarch64-elf-objcopy myos.elf -O binary kernel8.img

#echo "Transfering..."
#java -jar ../Raspbootin64Client-master/Raspbootin64Client.jar /dev/ttyUSB0 ../piOS/kernel8.img
#java -jar ../Raspbootin64Client-master/Raspbootin64Client.jar gui

#minicom -b 115200 -o -D /dev/ttyUSB0
qemu-system-aarch64 -M raspi3 -serial stdio -kernel kernel8.img
#cu -l /dev/ttyUSB0 -s 115200
sleep 2
echo "Transfering..."
#../raspbootin/raspbootcom/raspbootcom /dev/ttyUSB0 ~/piOS/kernel8.img 

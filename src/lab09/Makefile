uart.bin: start.o main.o 
	arm-linux-ld -Ttext 0xD0020010 -o uart.elf $^
	arm-linux-objcopy -O binary uart.elf $@
	arm-linux-objdump -D uart.elf > uart.dis

%.o: %.c
	arm-linux-gcc -c $< -o $@ -nostdlib -std=c99

%.o: %.S
	arm-linux-gcc -c $< -o $@ -nostdlib

clean:
	rm *.o *.elf *.bin *.dis

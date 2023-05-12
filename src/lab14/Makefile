key.bin:start.o key.o
	arm-linux-ld -Ttext 0xd0020010 -o key.elf $^
	arm-linux-objcopy -O binary key.elf key.bin
	arm-linux-objdump -D key.elf > key.dis
key.o:key.c
	arm-linux-gcc -nostdlib -c $< -o $@
start.o:start.S
	arm-linux-gcc -nostdlib -c $< -o $@
clean:
	rm *.o *.elf *.bin *.dis




key.bin:start.o main.o
	arm-linux-ld -Ttext 0xd0020010 -o key.elf $^
	arm-linux-objcopy -O binary key.elf key.bin
	arm-linux-objdump -D key.elf > key.dis
%.o:%.c
	arm-linux-gcc -c $< -o $@ -nostdlib
%.o:%.S
	arm-linux-gcc -c $< -o $@ -nostdlib
clean:
	rm *.o *.elf *.bin *.dis




pwm.bin: start.o main.o 
	arm-linux-ld -Ttext 0xD0020010 -o pwm.elf $^
	arm-linux-objcopy -O binary pwm.elf $@
	arm-linux-objdump -D pwm.elf > pwm.dis
     
%.o:%.c
	arm-linux-gcc -c $< -o $@ -nostdlib
%.o:%.S
	arm-linux-gcc -c $< -o $@ -nostdlib

clean:
	rm *.o *.elf *.bin *.dis

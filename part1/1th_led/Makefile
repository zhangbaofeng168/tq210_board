all : crt0.o led_c.o
	arm-linux-ld -Tled.lds $^ -o led_c.elf
	arm-linux-objcopy -O binary -S led_c.elf led_c.bin
	gcc -o mkv210 mkv210.c
%.o : %.S
	arm-linux-gcc -g -c -o $@ $^
%.o : %.c
	arm-linux-gcc -g -c -o $@ $^
clean:
	rm -f *.dis *.bin *.elf *.o
	
	
all : crt0.o ddr_init.o uart.o nand.o lib.o main.o
	arm-linux-ld -Tzbf.lds $^ -o zbf.elf
	arm-linux-objcopy -O binary -S zbf.elf zbf.bin
	gcc -o mkv210 mkv210.c
%.o : %.S
	arm-linux-gcc  -g -c -o $@ $^
%.o : %.c
	arm-linux-gcc  -g -c -o $@ $^
clean:
	rm -f *.dis *.bin *.elf *.o
	
	
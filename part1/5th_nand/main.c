#define	CPC0CON	(*(volatile unsigned long *)0xE0200060)
#define	CPC0DAT	(*(volatile unsigned long *)0xE0200064)

#include "uart.h"
#include "lib.h"
#include "nand.h"
void bzero(unsigned char *s, int size)
{
	int i = 0;
	for (; i < size; i++)
		s[i] = 0;
}
int main()
{
	char flag;
	int i;
	unsigned char buf[2048];
	
	CPC0CON = 0x00011000;//设置CPC0_4,CPC0_3为输出
	uart_init();//串口初始化
	nand_init();//nandflash初始化
	
	bzero(buf, 2048);
	nand_read_id(buf);
	for (i = 0; i < 5; i++)
	{
		putchar_hex(buf[i]);
		putchar(' ');
	}
	bzero(buf, 2048);
	for (i = 0; i < 2048; i++)
	{
		buf[i] = i % 255;
	}
	nand_erase(0x80000);
	write_nand(0x80000,buf);
	bzero(buf, 2048);
	read_nand(0x80000,buf);
	for (i = 0; i < 1024; i++)
	{
		putchar_hex(buf[i]);
		putchar(' ');
	}
	
	while(1)
	{
		put_string("\r\n/****welcome to zbf_bootloader****/\r\n");
		put_string("\rEnter 0 or 1 to test led:");
		flag = getc();
		putc(flag);putc('\n');
		if(flag == '0')
			CPC0DAT = 0x10;
		if(flag == '1')
			CPC0DAT = 0x08;
	}
	return 0;
}

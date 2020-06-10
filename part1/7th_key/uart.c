#define	CLK_SRC4	(*(volatile unsigned long *)0xE0100210)
#define	CLK_DIV4	(*(volatile unsigned long *)0xE0100310)
#define	GPA0CON		(*(volatile unsigned long *)0xE0200000)
#define	ULCON0		(*(volatile unsigned long *)0xE2900000 )
#define	UCON0		(*(volatile unsigned long *)0xE2900004 )
#define	UFCON0		(*(volatile unsigned long *)0xE2900008 )
#define	UBRDIV0		(*(volatile unsigned long *)0xE2900028)
#define	UDIVSLOT0	(*(volatile unsigned long *)0xE290002C)
#define	URXH0		(*(volatile unsigned long *)0xE2900024 )
#define	UTRSTAT0	(*(volatile unsigned long *)0xE2900010)
#define	UTXH0		(*(volatile unsigned long *)0xE2900020)


void uart_init()
{

	////1.引脚配置
	GPA0CON  |= (0x2<<0)|(0x2<<4);
	////2.uart控制寄存器
	ULCON0    |=0x3;
	UFCON0	  |=0x0;
	UCON0	  |=0x5;//IV_VAL1 = 35
	UBRDIV0   |=0x22;
	UDIVSLOT0 |=0xDDDD;
}

// 接收一个字符  
char getc(void) 
{ 
	 while(!(UTRSTAT0 & 1));    // 如果RX FIFO 空，等待 
		return URXH0;                     // 取数据 
} 
 
// 发送一个字符 
void putc(unsigned char c) 
{ 
	while(!(UTRSTAT0 & (1 << 2)));     // 如果TX FIFO 满，等待 
		UTXH0 = c;                        // 写数据 
} 


//发送字符串
void put_string(unsigned char *str)
{
	char *p=str;
	while(*p)
		putc(*p++);
}

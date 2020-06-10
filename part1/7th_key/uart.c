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

	////1.��������
	GPA0CON  |= (0x2<<0)|(0x2<<4);
	////2.uart���ƼĴ���
	ULCON0    |=0x3;
	UFCON0	  |=0x0;
	UCON0	  |=0x5;//IV_VAL1 = 35
	UBRDIV0   |=0x22;
	UDIVSLOT0 |=0xDDDD;
}

// ����һ���ַ�  
char getc(void) 
{ 
	 while(!(UTRSTAT0 & 1));    // ���RX FIFO �գ��ȴ� 
		return URXH0;                     // ȡ���� 
} 
 
// ����һ���ַ� 
void putc(unsigned char c) 
{ 
	while(!(UTRSTAT0 & (1 << 2)));     // ���TX FIFO �����ȴ� 
		UTXH0 = c;                        // д���� 
} 


//�����ַ���
void put_string(unsigned char *str)
{
	char *p=str;
	while(*p)
		putc(*p++);
}

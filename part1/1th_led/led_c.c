#define	CPC0CON	(*(volatile unsigned long *)0xE0200060)
#define	CPC0DAT	(*(volatile unsigned long *)0xE0200064)

void delay()
{
	unsigned long j;
	for(j=0;j<300000;j++){}
}
int main()
{
	int i;
	CPC0CON = 0x00011000;//����CPC0_4,CPC0_3Ϊ���
	while(1)
	{
		for(i=0;i<4;i++)
		{
			CPC0DAT = ((i/2)<<4)|((i%2)<<3);//2���Ʊ仯
			delay();
		}
	}
	return 0;
}

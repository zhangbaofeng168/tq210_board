//**nandflash 初始化**//
#define NFCONF   (*(volatile unsigned long *)0xB0E00000)
#define NFCONT   (*(volatile unsigned long *)0xB0E00004)
#define MP0_3CON (*(volatile unsigned long *)0xE0200320)
#define NFCMMD   (*(volatile unsigned long *)0xB0E00008)
#define NFADDR   (*(volatile unsigned long *)0xB0E0000C)
#define NFDATA   (*(volatile unsigned long *)0xB0E00010)
#define	NFSTAT   (*(volatile unsigned long *)0xB0E00028)

#define PAGE_SIZE 2048
#define READ_SIZE_ONE 2048
//等待操作
void s_delay()
{
	int i;
	for(i=0;i<10;i++);
}
void wait_busy()
{
	while((NFSTAT&0x1) == 0);
	s_delay();//短暂延时等待信号传输
}
//选中片选
void nand_select_chip(void)
{
	NFCONT &= 0xfffffffd;
	s_delay();
}
//取消片选
void nand_deselect_chip(void)
{
	NFCONT |= (1<<1);
	s_delay();	
}
//写命令
void write_cmd(int cmd)
{
	NFCMMD = cmd;
}
//复位nandflash	
static void nand_reset()
{
 	nand_select_chip();
 	write_cmd(0xff);  
 	wait_busy();
 	nand_deselect_chip();
}
//读数据
unsigned char read_data()
{
	return NFDATA;
}

//读size byte数据 */
static void nand_read_buf(unsigned char *buf, int size)
{
	int i = 0;
	for (; i < size; i++)
		buf[i] =  NFDATA;
}

//写数据
void write_data(unsigned char data)
{
	NFDATA = data;
}
// 写size byte数据 */
static void nand_write_buf(unsigned char *buf, int size)
{
	int i = 0;
	for (; i < size; i++)
		NFDATA = buf[i];
}
//写地址
void write_addr(unsigned int addr)
{
	unsigned int col = addr % PAGE_SIZE;
	unsigned int row = addr / PAGE_SIZE;
	NFADDR = col&0xff;
	NFADDR = (col>>8)&0xf;
	NFADDR = (row)&0xff;
	NFADDR = (row>>8)&0xff;
	NFADDR = (row>>16)&0x7;
}
void nand_init()
{
	/*1.周期配置[1]=1,1 = 5 address cycle 
	[15:12] Duration =  HCLK x TACLS  */
	NFCONF |= (1<<1)|(1<<12)|(2<<8)|(1<<4)|(1 << 23);
	//2.nandflash控制寄存器
	NFCONT |= (1<<0)|(1<<1);
	//3.相应的引脚配置
	MP0_3CON |= 0x22222222;
	
	nand_reset();//复位nandflash	
}

/* 读NAND ID */
void nand_read_id(unsigned char id[])
{
	int i;
	nand_select_chip();
	write_cmd(0x90);
	NFADDR = 0x00;
	for (i = 0; i < 5; i++)
		id[i] = read_data();

	nand_deselect_chip();
}

void nand_erase(unsigned int block_add)
{
	unsigned int row = block_add / PAGE_SIZE;
	nand_select_chip();
	write_cmd(0x60);
	//写块地址
	NFDATA = row&0xff;
	NFDATA = (row>>8)&0xff;
	NFDATA = (row>>16)&0x7;
	write_cmd(0xD0);
	wait_busy();
	nand_deselect_chip();
}
//写一页数据到nandflash
void write_nand(unsigned int write_add,unsigned char *buf)
{
	nand_select_chip();
	write_cmd(0x80);
	write_addr(write_add);	
	wait_busy();
	
	nand_write_buf(buf, PAGE_SIZE);
	
	write_cmd(0x10);
	wait_busy();
	nand_deselect_chip();
}

//读一页的函数
void read_nand(unsigned int start_add,unsigned char *buf)
{
	int i;
	nand_select_chip();
	
	write_cmd(0x00);
	write_addr(start_add);
	write_cmd(0x30);
	wait_busy();	
	
	nand_read_buf(buf, PAGE_SIZE);
	
	nand_deselect_chip();
}


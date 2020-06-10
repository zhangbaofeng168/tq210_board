
.global _start
_start:
	bl 	disable_watch_dog
	bl 	clock_init
        ldr     sp, =0xD0030000	   
        bl	ddr_init                                      
	bl      main
halt_loop:
	b halt_loop;
@关看门狗	
disable_watch_dog:
	ldr r0, =0xE2700000  
	mov r1, #0x0
	str r1, [r0]	
	mov pc, lr

@时钟初始化
@ARMCLK = 1000MHZ HCLK_MSYS = 200MHZ PCLK_MSYS = 100MHZ
	
#define CLK_DIV0  0xE0100300
#define CLK_SRC0  0xE0100200
#define APLL_CON0 0xE0100100
#define MPLL_CON  0xE0100108

#define DIV0_VAL       ((0x0<<0)|(4<<4)|(4<<8)|(1<<12)|(3<<16)|(1<<20)|(4<<24)|(1<<28))
#define APLL_CON0_VAL (3<<8)|(125<<16)|(1<<0)|(1<<31) @1000MHZ
#define MPLL_CON_VAL  (12<<8)|(667<<16)|(1<<0)|(1<<31)@667MHZ
clock_init:
	
	ldr r0, =APLL_CON0  @APLL_CON0
	ldr r1, =APLL_CON0_VAL
	str r1, [r0]
	
	ldr r0, =MPLL_CON  @MPLL_CON
	ldr r1, =MPLL_CON_VAL
	str r1, [r0]
	
	ldr r0, =CLK_DIV0  @CLK_DIV0
	ldr r1, =DIV0_VAL
	str r1, [r0]

	ldr r0, =CLK_SRC0  @CLK_SRC0
	ldr r1, =0x1111
	str r1, [r0]
	
	mov pc, lr
	

 


.global _start
_start:
            ldr     sp, =0xD0030000	                                         
            b       main                @ 调用C程序中的main函数


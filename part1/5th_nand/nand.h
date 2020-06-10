void nand_init();
void nand_read_id(unsigned char id[]);
void nand_erase(unsigned long block_add);
void write_nand(unsigned long write_add,unsigned char *buf);
void read_nand(unsigned long start_add,unsigned char *buf);
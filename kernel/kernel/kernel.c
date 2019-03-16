#include <stdio.h>

#include <kernel/tty.h>
#include <multiboot.h>
#include <kernel/kdebug.h>


void kernel_main(void);

void kernel_main(void){
	terminal_initialize();
	init_debug();

	printf("Hello, kernel World!\n");

	print_cur_status();

	panic("test");
}
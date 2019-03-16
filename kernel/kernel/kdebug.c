#include <kernel/kdebug.h>
#include <stdio.h>

static void print_stack_trace();
static elf_t kernel_elf;

void init_debug()
{
    // 从 GRUB 提供的信息中获取到内核符号表和代码地址信息
    kernel_elf = elf_from_multiboot(glb_mboot_ptr);
    printf("%x\n", glb_mboot_ptr);
}

void print_cur_status()
{
    //printf("%d>\n", kernel_elf.symtabsz);
    static int round = 0;
    uint16_t reg1, reg2, reg3, reg4;

    asm volatile (  "mov %%cs, %0;"
            "mov %%ds, %1;"
            "mov %%es, %2;"
            "mov %%ss, %3;"
            : "=m"(reg1), "=m"(reg2), "=m"(reg3), "=m"(reg4));

    // 打印当前的运行级别
    printf("%d: @ring %d\n", round, reg1 & 0x3);
    printf("%d:  cs = %x\n", round, reg1);
    printf("%d:  ds = %x\n", round, reg2);
    printf("%d:  es = %x\n", round, reg3);
    printf("%d:  ss = %x\n", round, reg4);
    ++round;
}

void panic(const char *msg)
{
    printf("*** System panic: %s\n", msg);
    print_stack_trace();
    printf("***\n");
    
    // 致命错误发生后打印栈信息后停止在这里
    while(1);
}

void print_stack_trace()
{
    uint32_t *ebp, *eip;
    printf("In pst\n");
    asm volatile ("mov %%ebp, %0" : "=m" (ebp));
    printf("%x\n", ebp);
    while (ebp) {
        eip = ebp + 1;
        printf("   [0x%x] %s\n", *eip, elf_lookup_symbol(*eip, &kernel_elf));
        ebp = (uint32_t*)*ebp;
    }
}
#ifndef INCLUDE_KDEBUG_H_
#define INCLUDE_KDEBUG_H_

#include <elf.h>

#define assert(x, info)							\
    do {										\
        if (!(x)) {								\
            panic(info);						\
        }										\
    } while (0)
    
// 编译期间静态检测
#define static_assert(x)						\
    switch (x) { case 0: case (x): ; }

// 初始化 Debug 信息
void init_debug();

// 打印当前的函数调用栈信息
void panic(const char *msg);

// 打印当前的段存器值
void print_cur_status();



#endif  // INCLUDE_DEBUG_H_
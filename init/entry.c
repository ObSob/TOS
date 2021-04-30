#include "console.h"
#include "debug.h"
#include "gdt.h"

int kern_entry()
{
    console_clear();

    init_debug();
    init_gdt();
    printk_color(rc_black, rc_light_blue, "Hello, OS Kernel!\n");
    panic("panic test");

	return 0;
}


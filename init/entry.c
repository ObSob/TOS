#include "console.h"
#include "debug.h"

int kern_entry()
{
	console_clear();
    printk_color(rc_black, rc_light_blue, "Hello, OS Kernel!\n", rc_black, rc_green);

	return 0;
}


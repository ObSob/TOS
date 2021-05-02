#include "console.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"

int kern_entry()
{
    init_debug();
    init_gdt();
    init_idt();

    console_clear();
    printk_color(rc_black, rc_light_blue, "Hello, OS Kernel!\n\n");

    init_timer(200);

//    sti();

    show_kernel_mem_info();
    show_memory_map();
    init_pmm();

    uint32_t allc_addr = NULL;
    printk("Test Physical Memory Alloc :\n");
    allc_addr = pmm_alloc_page();
    printk("\tAlloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmm_alloc_page();
    printk("\tAlloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmm_alloc_page();
    printk("\tAlloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmm_alloc_page();
    printk("\tAlloc Physical Addr: 0x%08X\n", allc_addr);



//    panic("panic test");

	return 0;
}


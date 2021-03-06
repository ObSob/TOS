#ifndef TOS_PMM_H
#define TOS_PMM_H

#include "multiboot.h"

#define STACK_SIZE      8192
#define PMM_MAX_SIZE    0x20000000      // 512M support
#define PMM_PAGE_SIZE   0x1000

#define PAGE_MAX_SIZE   (PMM_MAX_SIZE/PMM_PAGE_SIZE)

#define PHY_PAGE_MASH   0xfffff000

extern uint8_t kern_start[];
extern uint8_t kern_end[];

extern uint32_t phy_page_count;

void    show_kernel_mem_info();
void    show_memory_map();
void    init_pmm();
uint32_t pmm_alloc_page();
void    pmm_free_page(uint32_t p);

#endif //TOS_PMM_H

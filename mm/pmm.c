#include "multiboot.h"
#include "common.h"
#include "debug.h"
#include "pmm.h"

static uint32_t pmm_stack[PAGE_MAX_SIZE + 1];
static uint32_t pmm_stack_top;
uint32_t phy_page_count;

void
init_pmm()
{
    printk("init memory, ");
    mmap_entry_t *mmap_start_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr;
    mmap_entry_t *mmap_end_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr + glb_mboot_ptr->mmap_length;
    mmap_entry_t *map_entry;

    for (map_entry = mmap_start_addr; map_entry < mmap_end_addr; map_entry++) {
        if (map_entry->type == 1 && map_entry->base_addr_low == 0x100000) {
            // 把内核结束位置到结束位置的内存段，按页存储到页管理栈里
            // 最多支持512MB的物理内存
            uint32_t page_addr = map_entry->base_addr_low + (uint32_t)(kern_end - kern_start);
            uint32_t length = map_entry->base_addr_low + map_entry->length_low;

            while (page_addr < length && page_addr <= PMM_MAX_SIZE) {
                pmm_free_page(page_addr);
                page_addr += PMM_PAGE_SIZE;
                phy_page_count++;
            }
        }
    }
    printk("%u physical pages in total\n", phy_page_count);
}

uint32_t
pmm_alloc_page()
{
    assert(pmm_stack_top != 0, "out of memory");
    uint32_t page = pmm_stack[pmm_stack_top--];
    return page;
}

void
pmm_free_page(uint32_t p)
{
    assert(pmm_stack_top != PAGE_MAX_SIZE, "out of pmm_stack stack");
    pmm_stack[++pmm_stack_top] = p;
}

void
show_kernel_mem_info()
{
    printk("kernel info in memory: \n");
    printk("\tkernel in memory start: 0x%08X\n", kern_start);
    printk("\tkernel in memory end:   0x%08X\n", kern_end);
    printk("\tkernel in memory used:  %d KB\n\n", (kern_end - kern_start + 1023) / 1024);
}

void
show_memory_map()
{
    uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
    uint32_t mmap_length = glb_mboot_ptr->mmap_length;

    printk("Memory map: \n");
    printk("\tbase_addr\tlength\t\ttype\n");
    for (mmap_entry_t *mmap = (mmap_entry_t *)mmap_addr; (uint32_t)mmap < mmap_addr + mmap_length; mmap++) {
        printk("\t0x%X%08X\t0x%X%08X\t0x%X\n",
               (uint32_t)mmap->base_addr_high, (uint32_t)mmap->base_addr_low,
               (uint32_t)mmap->length_high, (uint32_t)mmap->length_low,
               (uint32_t)mmap->type);
    }
    printk("\n");
}
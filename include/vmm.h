#ifndef TOS_VMM_H
#define TOS_VMM_H

#include "types.h"
#include "idt.h"
#include "vmm.h"

#define PAGE_OFFSET 	0xC0000000

#define PAGE_PRESENT    0x1     // valid
#define PAGE_WRITE      0x2     // writable
#define PAGE_USER 	    0x4     // user lavel

#define PAGE_SIZE       4096
#define PAGE_MASK       0xFFFFF000

#define PGD_INDEX(x)    (((x) >> 22) & 0x3FF)       // page directory entry
#define PTE_INDEX(x)    (((x) >> 12) & 0x3FF)       // page table entry
#define OFFSET_INDEX(x) ((x) & 0xFFF)               // offset

typedef uint32_t pgd_t;
typedef uint32_t pte_t;

#define PGD_SIZE    (PAGE_SIZE/sizeof(pte_t))
#define PTE_SIZE    (PAGE_SIZE/sizeof(uint32_t))

#define PTE_COUNT   128   // 映射512M内存需要的页表数

extern pgd_t pgd_kern[PGD_SIZE];

static inline void
flushTLB(uint32_t va)
{
    asm volatile ("invlpg (%0)" : : "a" (va));
}

void        init_vmm();
void        switch_pde(uint32_t pd);
void        map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags);
void        unmap(pgd_t *pgd_now, uint32_t va);
uint32_t    get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa);
void        page_fault(pt_regs *regs);


#endif //TOS_VMM_H

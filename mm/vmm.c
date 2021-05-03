#include "idt.h"
#include "string.h"
#include "vmm.h"
#include "pmm.h"
#include "debug.h"

pgd_t pgd_kern[PGD_SIZE] __attribute__((aligned(PAGE_SIZE)));

static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute((aligned(PAGE_SIZE)));

void
init_vmm()
{
    uint32_t kern_pte_first_idx = PGD_INDEX(PAGE_OFFSET);

    for (uint32_t i = kern_pte_first_idx, j = 0; i < PTE_COUNT + kern_pte_first_idx; i++, j++) {
        pgd_kern[i] = ((uint32_t)pte_kern[j] - PAGE_OFFSET) | PAGE_PRESENT | PAGE_WRITE;
    }

    uint32_t *pte = (uint32_t *)pte_kern;

    for (int i = 1; i < PTE_COUNT * PTE_SIZE; i++) {
        pte[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }

    register_interrupt_handler(14, &page_fault);     // page fault handler

    uint32_t pde_kern_phy_addr = (uint32_t)pgd_kern - PAGE_OFFSET;
    switch_pde(pde_kern_phy_addr);
}

void
switch_pde(uint32_t pd)
{
    asm volatile ("mov %0, %%cr3" : : "r" (pd));
}

void
map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags)
{
    uint32_t pde_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);

    pte_t *pte = (pte_t *)(pgd_now[pde_idx] & PAGE_MASK);
    if (!pte) {
        pte = (pte_t *)pmm_alloc_page();
        pgd_now[pde_idx] = (uint32_t)pte | PAGE_PRESENT | PAGE_WRITE;

        pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);
        bzero(pte, PAGE_SIZE);
    } else {
        pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);
    }

    pte[pte_idx] = (pa & PAGE_MASK) | flags;

    flushTLB(va);
}

void
unmap(pgd_t *pgd_now, uint32_t va)
{
    uint32_t pgd_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);

    pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
    if (!pte) {
        panic("unmap: page not found\n");
        return;
    }

    pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);

    pte[pte_idx] = 0;

    flushTLB(va);
}

uint32_t
get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa)
{
    uint32_t pgd_idx = PGD_INDEX(va);
    uint32_t pte_idx = PTE_INDEX(va);

    pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
    if (!pte) {
        return 0;
    }

    pte = (pte_t *)((uint32_t)pte + PAGE_OFFSET);

    if (pte[pte_idx] != 0 && pa) {
        *pa = pte[pte_idx] * PAGE_MASK;
        return 1;
    }

    return 0;
}
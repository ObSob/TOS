#include "gdt.h"
#include "string.h"
#include "debug.h"

#define GDT_LENGTH 5

gdt_entry_t gdt_entries[GDT_LENGTH];
gdt_ptr_t   gdt_ptr;

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

extern uint32_t stack;

void
init_gdt()
{
    gdt_ptr.limit = sizeof(gdt_entry_t) * GDT_LENGTH - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);             	    // 按照 Intel 文档要求，第一个描述符必须全 0
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); 	// 指令段
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); 	// 数据段
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); 	// 用户模式代码段
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); 	// 用户模式数据段

    // 加载全局描述符表地址到 GPTR 寄存器
    printk("loading GDT...\n");
    gdt_flush((uint32_t)&gdt_ptr);
}

static void
gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low     = (base & 0xFFFF);
    gdt_entries[num].base_middle  = (base >> 16) & 0xFF;
    gdt_entries[num].base_high    = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low    = (limit & 0xFFFF);
    gdt_entries[num].granularity  = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access       = access;
}

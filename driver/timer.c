#include "timer.h"
#include "debug.h"
#include "common.h"
#include "idt.h"

void
timer_callback(pt_regs *regs)
{
    static uint32_t tick = 0;
    printk_color(rc_black, rc_red, "Tick: %d\n", tick++);
}

void
init_timer(uint32_t frequency)
{
    register_interrupt_handler(IRQ0, timer_callback);

    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    // 分别写入低字节和高字节
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}
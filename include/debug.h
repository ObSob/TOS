//
// Created by wbh on 2021/4/4.
//

#ifndef TOS_DEBUG_H
#define TOS_DEBUG_H

#include "console.h"
#include "vargs.h"

void printk(const char *format, ...);
void printk_color(real_color_t bg, real_color_t fore, const char* format, ...);

#endif //TOS_DEBUG_H

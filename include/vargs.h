//
// Created by wbh on 2021/4/4.
//

#ifndef TOS_VARGS_H
#define TOS_VARGS_H

typedef __builtin_va_list va_list;

#define va_start(ap, last)  (__builtin_va_start(ap, last))
#define va_arg(ap, type)    (__builtin_va_arg(ap, type))
#define va_end(ap)

#endif //TOS_VARGS_H

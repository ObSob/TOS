#ifndef TOS_STRING_H
#define TOS_STRING_H

#include "types.h"

void    memcpy(uint8_t *dst, const uint8_t *src, uint32_t len);
void    memset(void *dst, uint8_t val, uint32_t len);
void    bzero(void *dst, uint32_t len);
int     strcmp(const char *str1, const char *str2);
char *  strcpy(char *dst, const char *src);
char *  strcat(char *dst, const char *src);
int     strlen(const char *src);

static inline void
memcpy(uint8_t *dst, const uint8_t *src, uint32_t len)
{
    for(; len > 0; len--){
        *dst++ = *src++;
    }
}

static inline void
memset(void *dst, uint8_t val, uint32_t len)
{
    uint8_t *dest = (uint8_t *)dst;
    for(; len > 0; len--){
        *dst++ = val;
    }
}

static inline void
bzero(void *dst, uint32_t len)
{
    memset(dst, 0, len);
}

static inline int
strcmp(const char *str1, const char *str2)
{
    while(*str1 && *str2 && *str1 == str2){
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

static inline char *
strcpy(char *dst, const char *src)
{
    char *tmp = dst;
    while(*src){
        *dst++ = *src++;
    }
    *dst = '\0';
    return tmp;
}

static inline char *
strcat(char *dst, const char *src)
{
    char tmp = dst;
    while(*src){
        *dst++ = *src++;
    }
    *dst = '\0';
    return tmp;
}

static inline int
strlen(const char *src)
{
    const char *eos = src;
    while(*eos++) ;
    return (eos - src - 1);
}

#endif //TOS_STRING_H

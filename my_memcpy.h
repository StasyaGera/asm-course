#ifndef ASM_COURSE_MY_MEMCPY_H
#define ASM_COURSE_MY_MEMCPY_H

#include <emmintrin.h>

void naive_memcpy(void* dst, void const* src, size_t size) {
    for (size_t i = 0; i < size; i++) {
        *((char*)dst + i) = *((char*)src + i);
    }
}

void my_memcpy(void* dst, void const* src, size_t size) {
    size_t offset = (size_t)src % 16;
    if (offset != 0) {
        offset = 16 - offset;
        naive_memcpy(dst, src, offset);
        size -= offset;
    }

    __m128i tmp;
    void* src_cp = (char*)src + offset;
    void* dst_cp = (char*)dst + offset;

    if (size >= 16) {
        __asm__ volatile(
        "cpy:\n"
                "movdqa     (%1), %3\n"
                "movntdq    %3, (%0)\n"
                "add        $16, %0\n"
                "add        $16, %1\n"
                "sub        $16, %2\n"
                "cmp        $16, %2\n"
                "jnl        cpy\n"
        : "=r" (dst_cp), "=r" (src_cp), "=r" (size), "=x" (tmp)
        : "0" (dst_cp), "1" (src_cp), "2" (size)
        : "memory", "cc"
        );
    }

    naive_memcpy(dst_cp, src_cp, size);
}

#endif //ASM_COURSE_MY_MEMCPY_H
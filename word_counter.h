#ifndef ASM_COURSE_WORD_COUNTER_H
#define ASM_COURSE_WORD_COUNTER_H

#include <emmintrin.h>

const char ws = ' ';
bool ws_seq;

size_t naive_count(const char * text, size_t size) {
    size_t result = 0;
    for (size_t i = 0; i < size; i++) {
        if (text[i] == ws) {
            ws_seq = true;
        } else if (ws_seq) {
            ws_seq = false;
            result++;
        }
    }
    return result;
}

size_t naive_count(std::string input) {
    ws_seq = false;
    return naive_count(input.c_str(), input.size()) + ((input.size() == 0 || input[0] == ws) ? 0 : 1);
}

size_t count(std::string input) {
    ws_seq = false;
    const char *text = input.c_str();
    size_t size = input.size();

    int result = (size == 0 || text[0] == ws) ? 0 : 1;
    size_t offset = (size_t)text % 16;
    if (offset != 0) {
        offset = 16 - offset;
        result += naive_count(text, offset);
        text += offset;
        size -= offset;
    }

    __m128i spaces = _mm_set_epi8(ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws);
    __m128i curr, next, tmp;
    __asm__ volatile(
            "movdqa     (%0), %1\n"
            "pcmpeqb    %2, %1\n"
    : "=r" (text),"=x" (next), "=x" (spaces)
    : "0" (text), "1" (next), "2" (spaces)
    : "memory", "cc"
    );

    for (size; size >= 32; size -= 16) {
        int32_t a;
        __asm__ volatile(
                "add        $16,  %0\n"
                "movdqa     %3, %1\n"
                "movdqa     (%0), %2\n"

                "pcmpeqb    %4, %2\n"
                "movdqa     %2, %3\n"

                "palignr    $1, %1, %2\n"
                "pandn      %1, %2\n"
                "pmovmskb   %2, %5\n"
        : "=r" (text), "=x" (curr), "=x" (tmp), "=x" (next), "=x" (spaces), "=r" (a)
        : "0" (text), "1" (curr), "2" (tmp), "3" (next), "4" (spaces), "5" (a)
        : "memory", "cc"
        );


        result += __builtin_popcount(a);
    }

    ws_seq = false;
    return result + naive_count(text, size);
}

#endif //ASM_COURSE_WORD_COUNTER_H

#ifndef ASM_COURSE_WORD_COUNTER_H
#define ASM_COURSE_WORD_COUNTER_H

#include <emmintrin.h>
#include <chrono>

const char ws = ' ';
bool ws_seq = false;

int naive_count(const char * text, size_t size) {
    int result = 0;
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

int naive_count(std::string input) {
    return naive_count(input.c_str(), input.size());
}

int count(std::string input) {
    const char *text = input.c_str();
    size_t size = input.size();

    int result = text[0] == ws ? 0 : 1;
    size_t offset = (size_t)text % 16;
    if (offset != 0) {
        offset = 16 - offset;
        result += naive_count(text, offset);
        text += offset;
        size -= offset;
    }

    __m128i spaces = _mm_set_epi8(ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws, ws);
    for (size; size >= 16; size -= 16) {
        __m128i curr, next;
        int32_t a;

        __asm__ volatile(
                "movdqa     (%0), %1\n"
                "add        $16,  %0\n"
                "movdqa     (%0), %2\n"
                "pcmpeqb    %3, %1\n"
                "pcmpeqb    %3, %2\n"
                "palignr    $1, %1, %2\n"
                "pandn      %1, %2\n"
                "pmovmskb   %2, %4\n"

        : "=r" (text), "=x" (curr), "=x" (next), "=x" (spaces), "=r" (a)
        : "0" (text), "1" (curr), "2" (next), "3" (spaces), "4" (a)
        : "memory", "cc"
        );

        result += __builtin_popcount(a);
        ws_seq = (text[-1] == ws);
    }

    return result + naive_count(text, size);
}

#endif //ASM_COURSE_WORD_COUNTER_H

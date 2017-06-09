#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

#include "my_memcpy.h"
#include "word_counter.h"

#define word_counter

void printTime(std::string name, clock_t begin, clock_t end, int divisor) {
    std::cout << "> " << name << " done in " << double(end - begin) / CLOCKS_PER_SEC / divisor << " <\n";
}

int main(int argc, char* argv[])
{
#ifdef memcpy
    size_t const N = 100000000;
    size_t const M = 100;
    std::vector<char> a(N), b(N);
    clock_t begin, end;

    std::cout << "Started memcpy testing...\n";

    begin = clock();
    for (size_t i = 0; i != M; ++i)
        my_memcpy(b.data(), a.data(), N);
    end = clock();
    printTime("memcpy", begin, end, M);

    begin = clock();
    for (size_t i = 0; i != 10; ++i) {
        naive_memcpy(b.data(), a.data(), N);
    }
    end = clock();
    printTime("naive", begin, end, 10);

    std::cout << "Finished memcpy testing\n";
#endif

#ifdef word_counter
    std::string text = " one two   three four    five  six  seven eight     nine ten eleven              twelve";
    std::cout << count(text);
#endif
    return 0;
}

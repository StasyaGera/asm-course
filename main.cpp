#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

#include "my_memcpy.h"
#include "word_counter.h"

//#define memcpy
#define word_counter

void printTime(std::string name, clock_t begin, clock_t end, int divisor) {
    printf("> %s done in %f <\n", name.c_str(), double(end - begin) / CLOCKS_PER_SEC / divisor);
}

clock_t begin, end;
std::string start = "Started %s testing...\n";
std::string finish = "Finished %s testing.\n";

int main(int argc, char* argv[])
{
#ifdef memcpy
    size_t const N = 100000000;
    size_t const M = 100;
    std::vector<char> a(N), b(N);

    printf(start.c_str(), "memcpy");

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

    printf(finish.c_str(), "memcpy");
#endif

#ifdef word_counter
    std::srand((uint)std::time(nullptr));
    int n = std::rand() % 1000000 + 1000000;
    std::string text = "";
    for (int i = 1; i <= n; i++) {
        text.append(std::string((uint)std::rand() % 10 + 1, ' '));
        text.append(std::to_string(i));
    }

    printf(start.c_str(), "word counter");

    printf("Counting %d words\n", n);
    int m;

    begin = clock();
    m = count(text);
    end = clock();
    if (m != n) {
        printf("error counting");
        return 1;
    }
    printTime("count", begin, end, 1);

    begin = clock();
    m = naive_count(text);
    end = clock();
    if (m != n) {
        printf("error counting");
        return 1;
    }
    printTime("naive", begin, end, 1);

    printf(finish.c_str(), "word counter");
#endif

    return 0;
}

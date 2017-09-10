#ifndef ASM_COURSE_ALLOCATOR_H
#define ASM_COURSE_ALLOCATOR_H

#include <sys/mman.h>

struct allocator {
    static void* malloc() {
        if (node == nullptr) {
            node = (char**)mmap(0, 4096, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        }
        void* p = (void*)node;
        node = (char**)*node;
        return p;
    }
    static void free(void* ptr) {
        *(void**)ptr = (void*)node;
        node = (char**)ptr;
    }

    static char** node;
};

char** allocator::node = nullptr;

#endif //ASM_COURSE_ALLOCATOR_H

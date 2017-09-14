#ifndef ASM_COURSE_ALLOCATOR_H
#define ASM_COURSE_ALLOCATOR_H

#include <sys/mman.h>

struct allocator {
    allocator() {
        start = mmap(nullptr, PAGE_SZ, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        tail = start;
        for (char* i = (char*)start + NODE_SZ; i < (char*)start + PAGE_SZ; i += NODE_SZ) {
            *(void**)(i - NODE_SZ) = i;
        }
    }

    ~allocator()
    {
        munmap(start, PAGE_SZ);
    }

    static allocator& get_instance()
    {
        static allocator static_instance = allocator();
        return static_instance;
    }

    static void* malloc() {
        void* ptr = tail;
        tail = *(void**)tail;
        return ptr;
    }

    static void free(void* ptr) {
        *(void**)ptr = tail;
        tail = (void**)ptr;
    }

private:
    static const size_t NODE_SZ = 256;
    static const size_t PAGE_SZ = 4096;

    static void* tail;
    static void* start;
};

void* allocator::tail = nullptr;
void* allocator::start = nullptr;

#endif //ASM_COURSE_ALLOCATOR_H

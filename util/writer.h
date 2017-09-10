#ifndef ASM_COURSE_WRITER_H
#define ASM_COURSE_WRITER_H

struct writer {
    writer(void* ptr)
        : ptr((char*)ptr)
    {}

    void write(const char* instr) {
        for (const char* i = instr; *i != '\0'; i++, ptr++) {
            *ptr = *i;
        }
    }

    void write(const char* instr, void* obj) {
        write(instr);
        *(void**)ptr = obj;
        ptr += 8;
    }

    void write(const char* instr, int32_t imm) {
        write(instr);
        *(int32_t*)ptr = imm;
        ptr += 4;
    }

    char* get_ptr() {
        return ptr;
    }

    void skip(int size = 1) {
        ptr += size;
    }

private:
    char* ptr;
};

#endif //ASM_COURSE_WRITER_H

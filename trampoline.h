#ifndef ASM_COURSE_TRAMPOLINE_H
#define ASM_COURSE_TRAMPOLINE_H

#include "util/counter.h"
#include "util/writer.h"
#include "util/allocator.h"

const char* reg_shifts[5] = {
        "\x48\x89\xFE",                             //      mov   rsi, rdi
        "\x48\x89\xF2",                             //      mov   rdx, rsi
        "\x48\x89\xD1",                             //      mov   rcx, rdx
        "\x49\x89\xC8",                             //      mov   r8,  rcx
        "\x4D\x89\xC1"                              //      mov   r9,  r8
};

template <typename T>
struct trampoline;

template <typename R, typename ... Args>
struct trampoline<R(Args ...)> {
    template <typename F>
    trampoline(F const& f)
        : obj(new F(f)), caller(&call<F>), deleter(&del<F>)
    {
        id = allocator::malloc();
        writer w(id);

        if (counter<Args...>::INTEGER < 6) {
            for (int i = counter<Args...>::INTEGER - 1; i >= 0; i--) {
                w.write(reg_shifts[i]);
            }
            w.write("\x48\xBF", obj);               //      mov   rdi obj
            w.write("\x48\xB8", (void*)caller);     //      mov   rax caller
            w.write("\xFF\xE0");                    //      jmp   rax
        } else {
            int on_stack = (counter<Args...>::INTEGER - 6 + counter<Args...>::SSE) * 8;
            w.write("\x4C\x8B\x1C\x24");            //      mov   r11, [rsp]
            w.write("\x48\x89\xE0");                //      mov   rax, rsp
            w.write("\x48\x05", on_stack);          //      add   rax, on_stack
            w.write("\x41\x51");                    //      push  r9
            w.write("\x48\x81\xC4", 8);             //      add   rsp, $8
            char* shift = w.get_ptr();              // shift:
            w.write("\x48\x39\xE0");                //      cmp   rax, rsp
            w.write("\x74");                        //      je    continue
            char* je = w.get_ptr(); w.skip();
            w.write("\x48\x81\xC4", 8);             //      add   rsp, $8
            w.write("\x4C\x8B\x0C\x24");            //      mov   r9,  [rsp]
            w.write("\x4C\x89\x4C\x24\xF8");        //      mov   [rsp - 8], r9
            w.write("\xEB");                        //      jmp   shift
            *w.get_ptr() = (char)(shift - w.get_ptr()); w.skip();
            *je = (char)(w.get_ptr() - je - 1);     // continue:
            w.write("\x4C\x89\x1C\x24");            //      mov   [rsp], r11
            w.write("\x48\x81\xEC", on_stack + 8);  //      sub   rsp, on_stack + 8
            for (int i = 4; i >= 0; i--) {
                w.write(reg_shifts[i]);
            }
            w.write("\x48\xBF", obj);               //      mov   rdi obj
            w.write("\x48\xB8", (void*)caller);     //      mov   rax caller
            w.write("\xFF\xD0");                    //      call  rax
            w.write("\x41\x59");                    //      pop   r9
            w.write("\x4C\x8B\x9C\x24", on_stack);  //      mov   r11, [rsp + on_stack]
            w.write("\x4C\x89\x1C\x24");            //      mov   [rsp], r11
            w.write("\xC3");                        //      ret
        }
    }

    trampoline(trampoline&& other)
        : obj(other.obj), id(other.id),
          deleter(other.deleter)
    {
        other.obj = nullptr;
    }

    ~trampoline()
    {
        if (obj != nullptr) {
            deleter(obj);
        }
        allocator::free(id);
    }

    trampoline& operator=(trampoline&& other) {
        obj = other.obj;
        id = other.id;
        deleter = other.deleter;
        other.obj = nullptr;
        return *this;
    }

    R (*get() const)(Args... args)
    {
        return (R (*)(Args...))id;
    }

private:
    template <typename F>
    static R call(void* obj, Args... args) {
        return (*(F*)obj)(args...);
    }

    template <typename F>
    static void del(void* obj) {
        delete (F*)obj;
    }

    void* obj, *id;
    R (*caller)(void* obj, Args... args);
    void (*deleter)(void* obj);
};

#endif //ASM_COURSE_TRAMPOLINE_H

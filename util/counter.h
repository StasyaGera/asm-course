#ifndef ASM_COURSE_COUNTER_H
#define ASM_COURSE_COUNTER_H

template <typename ... Args>
struct counter;

template <>
struct counter<> {
    static const int INTEGER = 0;
    static const int SSE = 0;
};

template <typename ... Args>
struct counter<float, Args ...> {
    static const int INTEGER = counter<Args ...>::INTEGER;
    static const int SSE = counter<Args ...>::SSE + 1;
};

template <typename ... Args>
struct counter<double, Args ...> {
    static const int INTEGER = counter<Args ...>::INTEGER;
    static const int SSE = counter<Args ...>::SSE + 1;
};

template <typename T, typename ... Args>
struct counter<T, Args ...> {
    static const int INTEGER = counter<Args ...>::INTEGER + 1;
    static const int SSE = counter<Args ...>::SSE;
};

#endif //ASM_COURSE_COUNTER_H

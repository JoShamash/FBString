#ifndef FBSTRING_CORE_H
#define FBSTRING_CORE_H

#include <atomic>
#include <cstring>
#include <memory>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <stdexcept>

#ifdef USE_JEMALLOC
#include <jemalloc.h>
#endif

class FBStringCore {
public:
    // 类型定义
    typedef char value_type;
    typedef char &reference;
    typedef const char &const_reference;
    typedef char *iterator;
    typedef const char *const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t size_type;
    static const size_type npos = static_cast<size_type>(-1);

    /**
     * 默认构造函数
     * 初始化一个空的字符串对象。
     */
    FBStringCore();

    /**
     * 用 C 风格字符串初始化
     * @param s 指向字符数组的指针
     */
    FBStringCore(const char *s);

    /**
     * 用 n 个字符 c 初始化
     * @param n 字符个数
     * @param c 初始化字符
     */
    FBStringCore(size_type n, char c);

    /**
     * 使用 C 风格字符串和大小构造
     * @param str 指向字符数组的指针
     * @param size 字符串的长度
     */
    FBStringCore(const char *str, size_type size);

    /**
     * 拷贝构造函数
     * @param other 要复制的 FBStringCore 对象
     */
    FBStringCore(const FBStringCore &other);

    /**
     * 拷贝赋值运算符
     * @param other 要复制的 FBStringCore 对象
     * @return 当前对象的引用
     */
    FBStringCore &operator=(const FBStringCore &other);

    /**
     * 移动构造函数
     * @param other 要移动的 FBStringCore 对象
     */
    FBStringCore(FBStringCore&& other) noexcept;

    /**
     * 移动赋值运算符
     * @param other 要移动的 FBStringCore 对象
     * @return 当前对象的引用
     */
    FBStringCore& operator=(FBStringCore&& other) noexcept;

    /**
     * 从 C 字符串赋值
     * @param s 要赋值的 C 字符串
     * @return 当前对象的引用
     */
    FBStringCore &operator=(const char *s);

    /**
     * 析构函数
     * 释放所有分配的资源。
     */
    ~FBStringCore();

    /**
     * 返回当前字符串中第 n 个字符的位置
     * @param n 要访问的字符位置
     * @return 当前字符串中第 n 个字符的引用
     */
    reference operator[](size_type n);

    /**
     * 返回当前字符串中第 n 个字符的位置
     * @param n 要访问的字符位置
     * @return 当前字符串中第 n 个字符的常量引用
     */
    const_reference operator[](size_type n) const;

    /**
     * 返回当前字符串中第 n 个字符的位置并进行范围检查
     * @param n 要访问的字符位置
     * @return 当前字符串中第 n 个字符的引用
     * @throws out_of_range 如果 n 超出范围
     */
    reference at(size_type n);

    /**
     * 返回当前字符串中第 n 个字符的位置并进行范围检查
     * @param n 要访问的字符位置
     * @return 当前字符串中第 n 个字符的常量引用
     * @throws out_of_range 如果 n 超出范围
     */
    const_reference at(size_type n) const;

    /**
     * 返回一个非 null 终止的 C 字符数组
     * @return 指向内部字符数组的指针
     */
    const char *data() const;

    /**
     * 返回一个以 null 终止的 C 字符串
     * @return 指向内部字符数组的指针
     */
    const char *c_str() const;

    /**
     * 清空字符串
     * 将字符串重置为空。
     */
    void clear();

    /**
     * 预留存储空间
     * @param newCapacity 要预留的容量大小
     */
    void reserve(size_type newCapacity);

    /**
     * 调整字符串大小
     * @param newSize 新的字符串大小
     */
    void resize(size_type newSize);

    /**
     * 用字符 c 调整字符串大小
     * @param newSize 新的字符串大小
     * @param c 用于填充的字符
     */
    void resize(size_type newSize, char c);

    /**
     * 追加字符串
     * @param s 要追加的字符串
     * @return 当前对象的引用
     */
    FBStringCore &operator+=(const FBStringCore &s);

    /**
     * 追加 C 风格字符串
     * @param s 要追加的 C 字符串
     * @return 当前对象的引用
     */
    FBStringCore &append(const char *s);

    /**
     * 追加 C 风格字符串的前 n 个字符
     * @param s 要追加的 C 字符串
     * @param n 要追加的字符数
     * @return 当前对象的引用
     */
    FBStringCore &append(const char *s, size_type n);

    /**
     * 追加 FBStringCore 对象
     * @param s 要追加的 FBStringCore 对象
     * @return 当前对象的引用
     */
    FBStringCore &append(const FBStringCore &s);

    /**
     * 追加 FBStringCore 对象中的部分字符串
     * @param s 要追加的 FBStringCore 对象
     * @param pos 起始位置
     * @param n 要追加的字符数
     * @return 当前对象的引用
     */
    FBStringCore &append(const FBStringCore &s, size_type pos, size_type n);

    /**
     * 追加 n 个字符 c
     * @param n 要追加的字符数
     * @param c 要追加的字符
     * @return 当前对象的引用
     */
    FBStringCore &append(size_type n, char c);

    /**
     * 追加迭代器范围内的字符
     * @param first 起始迭代器
     * @param last 结束迭代器
     * @return 当前对象的引用
     */
    FBStringCore &append(const_iterator first, const_iterator last);

    /**
     * 用 C 风格字符串赋值
     * @param s 要赋值的 C 字符串
     * @return 当前对象的引用
     */
    FBStringCore &assign(const char *s);

    /**
     * 用 C 风格字符串的前 n 个字符赋值
     * @param s 要赋值的 C 字符串
     * @param n 要赋值的字符数
     * @return 当前对象的引用
     */
    FBStringCore &assign(const char *s, size_type n);

    /**
     * 用 FBStringCore 对象赋值
     * @param s 要赋值的 FBStringCore 对象
     * @return 当前对象的引用
     */
    FBStringCore &assign(const FBStringCore &s);

    /**
     * 用 n 个字符 c 赋值
     * @param n 要赋值的字符数
     * @param c 要赋值的字符
     * @return 当前对象的引用
     */
    FBStringCore &assign(size_type n, char c);

    /**
     * 用 FBStringCore 对象中的部分字符串赋值
     * @param s 要赋值的 FBStringCore 对象
     * @param start 起始位置
     * @param n 要赋值的字符数
     * @return 当前对象的引用
     */
    FBStringCore &assign(const FBStringCore &s, size_type start, size_type n);

    /**
     * 用迭代器范围内的字符赋值
     * @param first 起始迭代器
     * @param last 结束迭代器
     * @return 当前对象的引用
     */
    FBStringCore &assign(const_iterator first, const_iterator last);

    /**
     * 在指定位置插入 C 风格字符串
     * @param pos 插入位置
     * @param s 要插入的 C 字符串
     * @return 当前对象的引用
     */
    FBStringCore &insert(size_type pos, const char *s);

    /**
     * 在指定位置插入 C 风格字符串的前 n 个字符
     * @param pos 插入位置
     * @param s 要插入的 C 字符串
     * @param n 要插入的字符数
     * @return 当前对象的引用
     */
    FBStringCore &insert(size_type pos, const char *s, size_type n);

    /**
     * 在指定位置插入 FBStringCore 对象
     * @param pos 插入位置
     * @param s 要插入的 FBStringCore 对象
     * @return 当前对象的引用
     */
    FBStringCore &insert(size_type pos, const FBStringCore &s);

    /**
     * 在指定位置插入 FBStringCore 对象中的部分字符
     * @param pos 插入位置
     * @param s 要插入的 FBStringCore 对象
     * @param pos2 要插入的起始位置
     * @param n 要插入的字符数
     * @return 当前对象的引用
     */
    FBStringCore &insert(size_type pos, const FBStringCore &s, size_type pos2, size_type n);

    /**
     * 在指定位置插入 n 个字符 c
     * @param pos 插入位置
     * @param n 要插入的字符数
     * @param c 要插入的字符
     * @return 当前对象的引用
     */
    FBStringCore &insert(size_type pos, size_type n, char c);

    /**
     * 在迭代器位置插入字符 c
     * @param it 插入位置的迭代器
     * @param c 要插入的字符
     * @return 插入后的位置迭代器
     */
    iterator insert(iterator it, char c);

    /**
     * 在迭代器位置插入 n 个字符 c
     * @param it 插入位置的迭代器
     * @param n 要插入的字符数
     * @param c 要插入的字符
     */
    void insert(iterator it, size_type n, char c);

    /**
     * 在迭代器位置插入迭代器范围内的字符
     * @param it 插入位置的迭代器
     * @param first 要插入的起始迭代器
     * @param last 要插入的结束迭代器
     */
    void insert(iterator it, const_iterator first, const_iterator last);

    /**
     * 删除指定位置的 n 个字符
     * @param pos 删除的起始位置
     * @param n 要删除的字符数
     * @return 当前对象的引用
     */
    FBStringCore &erase(size_type pos = 0, size_type n = npos);

    /**
     * 删除迭代器位置的字符
     * @param pos 要删除的字符位置的迭代器
     * @return 删除后的位置迭代器
     */
    iterator erase(iterator pos);

    /**
     * 删除迭代器范围内的字符
     * @param first 要删除的起始迭代器
     * @param last 要删除的结束迭代器
     * @return 删除后的位置迭代器
     */
    iterator erase(iterator first, iterator last);

    /**
     * 替换指定位置的 n 个字符为 C 风格字符串
     * @param p0 替换的起始位置
     * @param n0 要替换的字符数
     * @param s 替换为的 C 字符串
     * @return 当前对象的引用
     */
    FBStringCore &replace(size_type p0, size_type n0, const char *s);

    /**
     * 替换指定位置的 n0 个字符为 C 风格字符串的前 n 个字符
     * @param p0 替换的起始位置
     * @param n0 要替换的字符数
     * @param s 替换为的 C 字符串
     * @param n 替换为的字符数
     * @return 当前对象的引用
     */
    FBStringCore &replace(size_type p0, size_type n0, const char *s, size_type n);

    /**
     * 替换指定位置的 n 个字符为 FBStringCore 对象
     * @param p0 替换的起始位置
     * @param n0 要替换的字符数
     * @param s 替换为的 FBStringCore 对象
     * @return 当前对象的引用
     */
    FBStringCore &replace(size_type p0, size_type n0, const FBStringCore &s);

    /**
     * 替换指定位置的 n0 个字符为 FBStringCore 对象中的部分字符
     * @param p0 替换的起始位置
     * @param n0 要替换的字符数
     * @param s 替换为的 FBStringCore 对象
     * @param pos 替换为的起始位置
     * @param n 替换为的字符数
     * @return 当前对象的引用
     */
    FBStringCore &replace(size_type p0, size_type n0, const FBStringCore &s, size_type pos, size_type n);

    /**
     * 替换指定位置的 n0 个字符为 n 个字符 c
     * @param p0 替换的起始位置
     * @param n0 要替换的字符数
     * @param n 替换为的字符数
     * @param c 替换为的字符
     * @return 当前对象的引用
     */
    FBStringCore &replace(size_type p0, size_type n0, size_type n, char c);

    /**
     * 替换迭代器范围内的字符为 C 风格字符串
     * @param first0 替换的起始迭代器
     * @param last0 替换的结束迭代器
     * @param s 替换为的 C 字符串
     * @return 当前对象的引用
     */
    FBStringCore &replace(iterator first0, iterator last0, const char *s);

    /**
     * 替换迭代器范围内的字符为 C 风格字符串的前 n 个字符
     * @param first0 替换的起始迭代器
     * @param last0 替换的结束迭代器
     * @param s 替换为的 C 字符串
     * @param n 替换为的字符数
     * @return 当前对象的引用
     */
    FBStringCore &replace(iterator first0, iterator last0, const char *s, size_type n);

    /**
     * 替换迭代器范围内的字符为 FBStringCore 对象
     * @param first0 替换的起始迭代器
     * @param last0 替换的结束迭代器
     * @param s 替换为的 FBStringCore 对象
     * @return 当前对象的引用
     */
    FBStringCore &replace(iterator first0, iterator last0, const FBStringCore &s);

    /**
     * 替换迭代器范围内的字符为 n 个字符 c
     * @param first0 替换的起始迭代器
     * @param last0 替换的结束迭代器
     * @param n 替换为的字符数
     * @param c 替换为的字符
     * @return 当前对象的引用
     */
    FBStringCore &replace(iterator first0, iterator last0, size_type n, char c);

    /**
     * 替换迭代器范围内的字符为另一个迭代器范围内的字符
     * @param first0 替换的起始迭代器
     * @param last0 替换的结束迭代器
     * @param first 替换为的起始迭代器
     * @param last 替换为的结束迭代器
     * @return 当前对象的引用
     */
    FBStringCore &replace(iterator first0, iterator last0, const_iterator first, const_iterator last);

    /**
     * 交换当前字符串与另一个字符串的值
     * @param s2 要交换的字符串
     */
    void swap(FBStringCore &s2);

    /**
     * 拷贝字符串中的字符到字符数组
     * @param s 目标字符数组
     * @param n 要拷贝的字符数
     * @param pos 开始拷贝的位置
     * @return 实际拷贝的字符数
     */
    size_type copy(char *s, size_type n, size_type pos = 0) const;

    /**
     * 返回子字符串
     * @param pos 子字符串的起始位置
     * @param n 子字符串的长度
     * @return 子字符串对象
     */
    FBStringCore substr(size_type pos = 0, size_type n = npos) const;

    /**
     * 比较两个字符串是否相等
     * @param other 要比较的 FBStringCore 对象
     * @return 是否相等
     */
    bool operator==(const FBStringCore& other) const;

    /**
     * 比较两个字符串是否不相等
     * @param other 要比较的 FBStringCore 对象
     * @return 是否不相等
     */
    bool operator!=(const FBStringCore& other) const;

    /**
     * 比较两个字符串大小
     * @param other 要比较的 FBStringCore 对象
     * @return 比较结果
     */
    bool operator<(const FBStringCore& other) const;
    bool operator<=(const FBStringCore& other) const;
    bool operator>(const FBStringCore& other) const;
    bool operator>=(const FBStringCore& other) const;

    /**
     * 比较当前字符串和另一个字符串的大小
     * @param s 要比较的字符串
     * @return 比较结果
     */
    int compare(const FBStringCore& s) const;

    /**
     * 比较当前字符串的子串和另一个字符串的大小
     * @param pos 子串的起始位置
     * @param n 子串的长度
     * @param s 要比较的c字符串
     * @param pos2 另一个子串的起始位置
     * @return 比较结果
     */
    int compare(size_type pos, size_type n, const char *s, size_type pos2) const;

    /**
     * 比较当前字符串的子串和另一个字符串的大小
     * @param pos 子串的起始位置
     * @param n 子串的长度
     * @param s 要比较的字符串
     * @return 比较结果
     */
    int compare(size_type pos, size_type n, const FBStringCore& s) const;

    /**
     * 比较当前字符串的子串和另一个字符串的子串的大小
     * @param pos 子串的起始位置
     * @param n 子串的长度
     * @param s 要比较的字符串
     * @param pos2 另一个子串的起始位置
     * @param n2 另一个子串的长度
     * @return 比较结果
     */
    int compare(size_type pos, size_type n, const FBStringCore& s, size_type pos2, size_type n2) const;

    /**
     * 比较当前字符串和 C 风格字符串的大小
     * @param s 要比较的 C 字符串
     * @return 比较结果
     */
    int compare(const char* s) const;

    /**
     * 比较当前字符串的子串和 C 风格字符串的大小
     * @param pos 子串的起始位置
     * @param n 子串的长度
     * @param s 要比较的 C 字符串
     * @return 比较结果
     */
    int compare(size_type pos, size_type n, const char* s) const;

    /**
     * 查找字符在字符串中的位置
     * @param c 要查找的字符
     * @param pos 开始查找的位置
     * @return 字符的位置或 npos
     */
    size_type find(char c, size_type pos = 0) const;

    /**
     * 查找 C 风格字符串在字符串中的位置
     * @param s 要查找的 C 字符串
     * @param pos 开始查找的位置
     * @return 字符串的位置或 npos
     */
    size_type find(const char* s, size_type pos = 0) const;

    /**
     * 查找 C 风格字符串的前 n 个字符在字符串中的位置
     * @param s 要查找的 C 字符串
     * @param pos 开始查找的位置
     * @param n 要查找的字符数
     * @return 字符串的位置或 npos
     */
    size_type find(const char* s, size_type pos, size_type n) const;

    /**
     * 查找 FBStringCore 对象在字符串中的位置
     * @param s 要查找的 FBStringCore 对象
     * @param pos 开始查找的位置
     * @return 字符串的位置或 npos
     */
    size_type find(const FBStringCore& s, size_type pos = 0) const;

    /**
     * 从后向前查找字符在字符串中的位置
     * @param c 要查找的字符
     * @param pos 开始查找的位置
     * @return 字符的位置或 npos
     */
    size_type rfind(char c, size_type pos = npos) const;

    /**
     * 从后向前查找 C 风格字符串在字符串中的位置
     * @param s 要查找的 C 字符串
     * @param pos 开始查找的位置
     * @return 字符串的位置或 npos
     */
    size_type rfind(const char* s, size_type pos = npos) const;

    /**
     * 从后向前查找 C 风格字符串的前 n 个字符在字符串中的位置
     * @param s 要查找的 C 字符串
     * @param pos 开始查找的位置
     * @param n 要查找的字符数
     * @return 字符串的位置或 npos
     */
    size_type rfind(const char* s, size_type pos, size_type n) const;

    /**
     * 从后向前查找 FBStringCore 对象在字符串中的位置
     * @param s 要查找的 FBStringCore 对象
     * @param pos 开始查找的位置
     * @return 字符串的位置或 npos
     */
    size_type rfind(const FBStringCore& s, size_type pos = npos) const;

    /**
     * 查找字符串中第一个出现的字符
     * @param c 要查找的字符
     * @param pos 开始查找的位置
     * @return 字符的位置或 npos
     */
    size_type find_first_of(char c, size_type pos = 0) const;

    /**
     * 查找字符串中第一个出现的 C 风格字符串
     * @param s 要查找的 C 字符串
     * @param pos 开始查找的位置
     * @return 字符串的位置或 npos
     */
    size_type find_first_of(const char* s, size_type pos = 0) const;

    /**
     * 查找字符串中第一个出现的 C 风格字符串的前 n 个字符
     * @param s 要查找的 C 字符串
     * @param pos 开始查找的位置
     * @param n 要查找的字符数
     * @return 字符串的位置或 npos
     */
    size_type find_first_of(const char* s, size_type pos, size_type n) const;

    /**
     * 查找字符串中第一个出现的 FBStringCore 对象
     * @param s 要查找的 FBStringCore 对象
     * @param pos 开始查找的位置
     * @return 字符串的位置或 npos
     */
    size_type find_first_of(const FBStringCore& s, size_type pos = 0) const;

    /**
     * 查找字符串中第一个不在指定字符集中的字符
     * @param c 要排除的字符
     * @param pos 开始查找的位置
     * @return 字符的位置或 npos
     */
    size_type find_first_not_of(char c, size_type pos = 0) const;

    /**
     * 查找字符串中第一个不在指定 C 风格字符串中的字符
     * @param s 要排除的 C 字符串
     * @param pos 开始查找的位置
     * @return 字符的位置或 npos
     */
    size_type find_first_not_of(const char* s, size_type pos = 0) const;

    /**
     * 查找字符串中第一个不在指定 C 风格字符串的前 n 个字符中的字符
     * @param s 要排除的 C 字符串
     * @param pos 开始查找的位置
     * @param n 要排除的字符数
     * @return 字符的位置或 npos
     */
    size_type find_first_not_of(const char* s, size_type pos, size_type n) const;

    /**
     * 查找字符串中第一个不在指定 FBStringCore 对象中的字符
     * @param s 要排除的 FBStringCore 对象
     * @param pos 开始查找的位置
     * @return 字符的位置或 npos
     */
    size_type find_first_not_of(const FBStringCore& s, size_type pos = 0) const;

    /**
     * 从后向前查找字符串中最后一个出现的字符
     * @param c 要查找的字符
     * @param pos 开始查找的位置
     * @return 字符的位置或 npos
     */
    size_type find_last_of(char c, size_type pos = npos) const;

    /**
     * 从后向前查找字符串中最后一个出现的 C 风格字符串
     * @param s 要查找的 C 字符串
     * @param pos 开始查找的位置
     * @return 字符串的位置或 npos
     */
    size_type find_last_of(const char* s, size_type pos = npos) const;

    /**
     * 从后向前查找字符串中最后一个出现的 C 风格字符串的前 n 个字符
     * @param s 要查找的 C 字符串
     * @param pos 开始查找的位置
     * @param n 要查找的字符数
     * @return 字符串的位置或 npos
     */
    size_type find_last_of(const char* s, size_type pos, size_type n) const;

    /**
     * 从后向前查找字符串中最后一个出现的 FBStringCore 对象
     * @param s 要查找的 FBStringCore 对象
     * @param pos 开始查找的位置
     * @return 字符串的位置或 npos
     */
    size_type find_last_of(const FBStringCore& s, size_type pos = npos) const;

    /**
     * 从后向前查找字符串中最后一个不在指定字符集中的字符
     * @param c 要排除的字符
     * @param pos 开始查找的位置
     * @return 字符的位置或 npos
     */
    size_type find_last_not_of(char c, size_type pos = npos) const;

    /**
     * 从后向前查找字符串中最后一个不在指定 C 风格字符串中的字符
     * @param s 要排除的 C 字符串
     * @param pos 开始查找的位置
     * @return 字符的位置或 npos
     */
    size_type find_last_not_of(const char* s, size_type pos = npos) const;

    /**
     * 从后向前查找字符串中最后一个不在指定 C 风格字符串的前 n 个字符中的字符
     * @param s 要排除的 C 字符串
     * @param pos 开始查找的位置
     * @param n 要排除的字符数
     * @return 字符的位置或 npos
     */
    size_type find_last_not_of(const char* s, size_type pos, size_type n) const;

    /**
     * 从后向前查找字符串中最后一个不在指定 FBStringCore 对象中的字符
     * @param s 要排除的 FBStringCore 对象
     * @param pos 开始查找的位置
     * @return 字符的位置或 npos
     */
    size_type find_last_not_of(const FBStringCore& s, size_type pos = npos) const;

    /**
     * 返回字符串的大小
     * @return 字符串的长度
     */
    size_type size() const;

    /**
     * 返回字符串的长度
     * @return 字符串的长度
     */
    size_type length() const;

    /**
     * 判断字符串是否为空
     * @return 是否为空
     */
    bool empty() const;

    /**
     * 返回当前容量
     * @return 当前容量
     */
    size_type capacity() const;

    /**
     * 返回可存放的最大字符串长度
     * @return 最大字符串长度
     */
    size_type max_size() const;

    /**
     * 返回字符串的起始位置迭代器
     * @return 起始位置迭代器
     */
    iterator begin();

    /**
     * 返回字符串的起始位置常量迭代器
     * @return 起始位置常量迭代器
     */
    const_iterator begin() const;

    /**
     * 返回字符串的结束位置迭代器
     * @return 结束位置迭代器
     */
    iterator end();

    /**
     * 返回字符串的结束位置常量迭代器
     * @return 结束位置常量迭代器
     */
    const_iterator end() const;

    /**
     * 返回字符串的最后一个字符位置的反向迭代器
     * @return 最后一个字符位置的反向迭代器
     */
    reverse_iterator rbegin();

    /**
     * 返回字符串的最后一个字符位置的常量反向迭代器
     * @return 最后一个字符位置的常量反向迭代器
     */
    const_reverse_iterator rbegin() const;

    /**
     * 返回字符串第一个字符位置的前面的反向迭代器
     * @return 第一个字符位置的前面的反向迭代器
     */
    reverse_iterator rend();

    /**
     * 返回字符串第一个字符位置的前面的常量反向迭代器
     * @return 第一个字符位置的前面的常量反向迭代器
     */
    const_reverse_iterator rend() const;

    /**
     * 输入操作符重载
     * @param in 输入流
     * @param s 字符串对象
     * @return 输入流
     */
    friend std::istream& operator>>(std::istream& in, FBStringCore& s);

    /**
     * 输出操作符重载
     * @param out 输出流
     * @param s 字符串对象
     * @return 输出流
     */
    friend std::ostream& operator<<(std::ostream& out, const FBStringCore& s);

    /**
     * 从输入流中读取字符串
     * @param in 输入流
     * @param s 字符串对象
     * @param delim 分隔符
     * @return 输入流
     */
    friend std::istream& getline(std::istream& in, FBStringCore& s, char delim);

private:
    /** 存储字符串类型枚举 */
    enum class StorageType {
        Small,
        Medium,
        Large
    };

    /** 中大型存储结构 */
    struct MediumLarge {
        char* data_;
        size_type size_;
        size_type capacity_;
        std::atomic<size_type>* refCount_;

        /** 返回容量 */
        size_type capacity() const;

        /** 设置容量和存储类型 */
        void setCapacity(size_type cap, StorageType type);
    };

    /** 存储联合体 */
    union Storage {
        char small_[24];
        MediumLarge ml_;
    };

    Storage storage_; /**< 存储联合体实例 */
    StorageType type_; /**< 当前存储类型 */

    /** 初始化小型存储 */
    void initSmall(const char* str, size_type size);

    /** 初始化中型存储 */
    void initMedium(const char* str, size_type size);

    /** 初始化大型存储 */
    void initLarge(const char* str, size_type size);

    /** 从另一个实例复制 */
    void copyFrom(const FBStringCore& other);

    /** 销毁当前存储 */
    void destroy();

    /** 解除共享 */
    void unshare();

    /** 分配内存 */
    char* allocate(size_type size);

    /** 释放内存 */
    void deallocate(char* ptr);

    /** 确定存储类型 */
    StorageType determineType(size_type size) const;

    /** 设置小型存储的大小 */
    void setSmallSize(size_type s);

    /** 返回小型存储的大小 */
    size_type smallSize() const;

    /** 返回容量 */
    size_type capacityImpl() const;

    /** 重新分配内存 */
    void realloc(size_type newCapacity);

    /** 判断是否为小端 */
    static bool isLittleEndian();

    /** 对齐大小 */
    static size_type alignSize(size_type size);

    /** 假设不可达 */
    static void assumeUnreachable();


};

#endif // FBSTRING_CORE_H

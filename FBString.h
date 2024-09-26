#ifndef FBSTRING_H
#define FBSTRING_H

#include "FBStringCore.h"
#include <ostream>
#include <string>

// FBString 类用于封装 FBStringCore 并提供更高级的字符串操作接口
class FBString {
public:
    /**
     * 默认构造函数
     * 初始化一个空的字符串对象。
     */
    FBString();

    /**
     * 使用 C 风格字符串构造
     * @param str 指向字符数组的指针
     */
    FBString(const char* str);

    /**
     * 拷贝构造函数
     * @param other 要复制的 FBString 对象
     */
    FBString(const FBString& other);

    /**
     * 拷贝赋值运算符
     * @param other 要复制的 FBString 对象
     * @return 当前对象的引用
     */
    FBString& operator=(const FBString& other);

    /**
     * 析构函数
     * 释放所有分配的资源。
     */
    ~FBString();

    /**
     * 返回 C 风格字符串
     * @return 指向内部字符数组的指针
     */
    const char* c_str() const;

    /**
     * 返回字符串大小
     * @return 字符串的长度
     */
    size_t size() const;

    /**
     * 清空字符串
     * 将字符串重置为空。
     */
    void clear();

    /**
     * 预留存储空间
     * @param newCapacity 要预留的容量大小
     */
    void reserve(size_t newCapacity);

    /**
     * 调整字符串大小
     * @param newSize 新的字符串大小
     */
    void resize(size_t newSize);

    /**
     * 追加字符串
     * @param str 要追加的字符串
     */
    void append(const char* str);

    /**
     * 插入字符串
     * @param pos 插入位置
     * @param str 要插入的字符串
     */
    void insert(size_t pos, const char* str);

    /**
     * 删除字符串的一部分
     * @param pos 删除的起始位置
     * @param len 要删除的长度
     */
    void erase(size_t pos, size_t len);

    /**
     * 查找子字符串
     * @param str 要查找的子字符串
     * @param pos 开始查找的位置
     * @return 子字符串的起始位置，如果未找到则返回 std::string::npos
     */
    size_t find(const char* str, size_t pos = 0) const;

    /**
     * 比较运算符
     * @param other 要比较的 FBString 对象
     * @return 比较结果
     */
    bool operator==(const FBString& other) const;
    bool operator!=(const FBString& other) const;
    bool operator<(const FBString& other) const;
    bool operator<=(const FBString& other) const;
    bool operator>(const FBString& other) const;
    bool operator>=(const FBString& other) const;

    /**
     * 输出运算符重载
     * @param os 输出流对象
     * @param str FBString 对象
     * @return 输出流对象
     */
    friend std::ostream& operator<<(std::ostream& os, const FBString& str);

    /**
     * 与 std::string 互相转换
     * @param str std::string 对象
     */
    FBString(const std::string& str);
    FBString& operator=(const std::string& str);
    operator std::string() const;

private:
    FBStringCore core_; /**< 核心存储对象 */
};

#endif // FBSTRING_H

#include "FBString.h"

// 默认构造函数
FBString::FBString() : core_() {}

// 使用 C 风格字符串构造
FBString::FBString(const char* str) : core_(str, std::strlen(str)) {}

// 拷贝构造函数
FBString::FBString(const FBString& other) : core_(other.core_) {}

// 拷贝赋值运算符
FBString& FBString::operator=(const FBString& other) {
    if (this != &other) {
        core_ = other.core_;
    }
    return *this;
}

// 析构函数
FBString::~FBString() {}

// 返回 C 风格字符串
const char* FBString::c_str() const {
    return core_.c_str();
}

// 返回字符串大小
size_t FBString::size() const {
    return core_.size();
}

// 清空字符串
void FBString::clear() {
    core_.clear();
}

// 预留存储空间
void FBString::reserve(size_t newCapacity) {
    core_.reserve(newCapacity);
}

// 调整字符串大小
void FBString::resize(size_t newSize) {
    core_.resize(newSize);
}

// 追加字符串
void FBString::append(const char* str) {
    core_.append(str, std::strlen(str));
}

// 插入字符串
void FBString::insert(size_t pos, const char* str) {
    core_.insert(pos, str, std::strlen(str));
}

// 删除字符串的一部分
void FBString::erase(size_t pos, size_t len) {
    core_.erase(pos, len);
}

// 查找子字符串
size_t FBString::find(const char* str, size_t pos) const {
    return core_.find(str, pos);
}

// 比较运算符
bool FBString::operator==(const FBString& other) const {
    return core_ == other.core_;
}

bool FBString::operator!=(const FBString& other) const {
    return core_ != other.core_;
}

bool FBString::operator<(const FBString& other) const {
    return core_ < other.core_;
}

bool FBString::operator<=(const FBString& other) const {
    return core_ <= other.core_;
}

bool FBString::operator>(const FBString& other) const {
    return core_ > other.core_;
}

bool FBString::operator>=(const FBString& other) const {
    return core_ >= other.core_;
}

// 输出运算符重载
std::ostream& operator<<(std::ostream& os, const FBString& str) {
    return os << str.c_str();
}

// 与 std::string 互相转换
FBString::FBString(const std::string& str) : core_(str.c_str(), str.size()) {}

FBString& FBString::operator=(const std::string& str) {
    core_ = FBStringCore(str.c_str(), str.size());
    return *this;
}

FBString::operator std::string() const {
    return std::string(core_.c_str(), core_.size());
}

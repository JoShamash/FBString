#include <limits>
#include "FBStringCore.h"

// 默认构造函数
FBStringCore::FBStringCore() {
    initSmall("", 0);
}

// 用 C 风格字符串初始化
FBStringCore::FBStringCore(const char* s) {
    size_type len = std::strlen(s);
    switch (determineType(len)) {
        case StorageType::Small:
            initSmall(s, len);
            break;
        case StorageType::Medium:
            initMedium(s, len);
            break;
        case StorageType::Large:
            initLarge(s, len);
            break;
    }
}

// 用 n 个字符 c 初始化
FBStringCore::FBStringCore(size_type n, char c) {
    std::string temp(n, c);
    switch (determineType(n)) {
        case StorageType::Small:
            initSmall(temp.c_str(), n);
            break;
        case StorageType::Medium:
            initMedium(temp.c_str(), n);
            break;
        case StorageType::Large:
            initLarge(temp.c_str(), n);
            break;
    }
}

// 使用 C 风格字符串和大小构造
FBStringCore::FBStringCore(const char* str, size_type size) {
    switch (determineType(size)) {
        case StorageType::Small:
            initSmall(str, size);
            break;
        case StorageType::Medium:
            initMedium(str, size);
            break;
        case StorageType::Large:
            initLarge(str, size);
            break;
    }
}

// 拷贝构造函数
FBStringCore::FBStringCore(const FBStringCore& other) {
    copyFrom(other);
}

// 移动构造函数
FBStringCore::FBStringCore(FBStringCore&& other) noexcept {
    storage_ = other.storage_;
    type_ = other.type_;
    other.type_ = StorageType::Small;
    other.initSmall("", 0);
}

// 拷贝赋值运算符
FBStringCore& FBStringCore::operator=(const FBStringCore& other) {
    if (this != &other) {
        destroy();
        copyFrom(other);
    }
    return *this;
}

// 移动赋值运算符
FBStringCore& FBStringCore::operator=(FBStringCore&& other) noexcept {
    if (this != &other) {
        destroy();
        storage_ = other.storage_;
        type_ = other.type_;
        other.type_ = StorageType::Small;
        other.initSmall("", 0);
    }
    return *this;
}

// 从 C 字符串赋值
FBStringCore& FBStringCore::operator=(const char* s) {
    size_type len = std::strlen(s);
    destroy();
    switch (determineType(len)) {
        case StorageType::Small:
            initSmall(s, len);
            break;
        case StorageType::Medium:
            initMedium(s, len);
            break;
        case StorageType::Large:
            initLarge(s, len);
            break;
    }
    return *this;
}

// 析构函数
FBStringCore::~FBStringCore() {
    destroy();
}

// 返回当前字符串中第 n 个字符的位置
FBStringCore::reference FBStringCore::operator[](size_type n) {
    return const_cast<reference>(static_cast<const FBStringCore&>(*this)[n]);
}

// 返回当前字符串中第 n 个字符的位置
FBStringCore::const_reference FBStringCore::operator[](size_type n) const {
    return type_ == StorageType::Small ? storage_.small_[n] : storage_.ml_.data_[n];
}

// 返回当前字符串中第 n 个字符的位置并进行范围检查
FBStringCore::reference FBStringCore::at(size_type n) {
    if (n >= size()) throw std::out_of_range("Index out of range");
    return (*this)[n];
}

// 返回当前字符串中第 n 个字符的位置并进行范围检查
FBStringCore::const_reference FBStringCore::at(size_type n) const {
    if (n >= size()) throw std::out_of_range("Index out of range");
    return (*this)[n];
}

// 返回一个非 null 终止的 C 字符数组
const char* FBStringCore::data() const {
    return c_str();
}

// 返回一个以 null 终止的 C 字符串
const char* FBStringCore::c_str() const {
    return type_ == StorageType::Small ? storage_.small_ : storage_.ml_.data_;
}

// 清空字符串
void FBStringCore::clear() {
    destroy();
    initSmall("", 0);
}

// 预留存储空间
void FBStringCore::reserve(size_type newCapacity) {
    if (newCapacity > capacity()) {
        realloc(newCapacity);
    }
}

// 调整字符串大小
void FBStringCore::resize(size_type newSize) {
    resize(newSize, '\0');
}

// 用字符 c 调整字符串大小
void FBStringCore::resize(size_type newSize, char c) {
    size_type currentSize = size();
    if (newSize > currentSize) {
        reserve(newSize);
        if (type_ == StorageType::Small) {
            std::fill(storage_.small_ + currentSize, storage_.small_ + newSize, c);
            setSmallSize(newSize);
        } else {
            std::fill(storage_.ml_.data_ + currentSize, storage_.ml_.data_ + newSize, c);
            storage_.ml_.size_ = newSize;
        }
    } else {
        if (type_ == StorageType::Small) {
            setSmallSize(newSize);
        } else {
            storage_.ml_.size_ = newSize;
        }
    }
}

// 追加字符串
FBStringCore& FBStringCore::operator+=(const FBStringCore& s) {
    return append(s);
}

// 追加 C 风格字符串
FBStringCore& FBStringCore::append(const char* s) {
    return append(s, std::strlen(s));
}

// 追加 C 风格字符串的前 n 个字符
FBStringCore& FBStringCore::append(const char* s, size_type n) {
    reserve(size() + n);
    if (type_ == StorageType::Small) {
        std::memcpy(storage_.small_ + smallSize(), s, n);
        setSmallSize(smallSize() + n);
    } else {
        std::memcpy(storage_.ml_.data_ + storage_.ml_.size_, s, n);
        storage_.ml_.size_ += n;
    }
    return *this;
}

// 追加 FBStringCore 对象
FBStringCore& FBStringCore::append(const FBStringCore& s) {
    return append(s.c_str(), s.size());
}

// 追加 FBStringCore 对象中的部分字符串
FBStringCore& FBStringCore::append(const FBStringCore& s, size_type pos, size_type n) {
    return append(s.c_str() + pos, n);
}

// 追加 n 个字符 c
FBStringCore& FBStringCore::append(size_type n, char c) {
    reserve(size() + n);
    if (type_ == StorageType::Small) {
        std::fill(storage_.small_ + smallSize(), storage_.small_ + smallSize() + n, c);
        setSmallSize(smallSize() + n);
    } else {
        std::fill(storage_.ml_.data_ + storage_.ml_.size_, storage_.ml_.data_ + storage_.ml_.size_ + n, c);
        storage_.ml_.size_ += n;
    }
    return *this;
}

// 追加迭代器范围内的字符
FBStringCore& FBStringCore::append(const_iterator first, const_iterator last) {
    return append(first, last - first);
}

// 用 C 风格字符串赋值
FBStringCore& FBStringCore::assign(const char* s) {
    return assign(s, std::strlen(s));
}

// 用 C 风格字符串的前 n 个字符赋值
FBStringCore& FBStringCore::assign(const char* s, size_type n) {
    destroy();
    switch (determineType(n)) {
        case StorageType::Small:
            initSmall(s, n);
            break;
        case StorageType::Medium:
            initMedium(s, n);
            break;
        case StorageType::Large:
            initLarge(s, n);
            break;
    }
    return *this;
}

// 用 FBStringCore 对象赋值
FBStringCore& FBStringCore::assign(const FBStringCore& s) {
    return assign(s.c_str(), s.size());
}

// 用 n 个字符 c 赋值
FBStringCore& FBStringCore::assign(size_type n, char c) {
    std::string temp(n, c);
    return assign(temp.c_str(), n);
}

// 用 FBStringCore 对象中的部分字符串赋值
FBStringCore& FBStringCore::assign(const FBStringCore& s, size_type start, size_type n) {
    return assign(s.c_str() + start, n);
}

// 用迭代器范围内的字符赋值
FBStringCore& FBStringCore::assign(const_iterator first, const_iterator last) {
    return assign(first, last - first);
}

// 在指定位置插入 C 风格字符串
FBStringCore& FBStringCore::insert(size_type pos, const char* s) {
    return insert(pos, s, std::strlen(s));
}

// 在指定位置插入 C 风格字符串的前 n 个字符
FBStringCore& FBStringCore::insert(size_type pos, const char* s, size_type n) {
    if (pos > size()) return *this;
    reserve(size() + n);
    if (type_ == StorageType::Small) {
        std::memmove(storage_.small_ + pos + n, storage_.small_ + pos, smallSize() - pos);
        std::memcpy(storage_.small_ + pos, s, n);
        setSmallSize(smallSize() + n);
    } else {
        std::memmove(storage_.ml_.data_ + pos + n, storage_.ml_.data_ + pos, storage_.ml_.size_ - pos);
        std::memcpy(storage_.ml_.data_ + pos, s, n);
        storage_.ml_.size_ += n;
    }
    return *this;
}

// 在指定位置插入 FBStringCore 对象
FBStringCore& FBStringCore::insert(size_type pos, const FBStringCore& s) {
    return insert(pos, s.c_str(), s.size());
}

// 在指定位置插入 FBStringCore 对象中的部分字符
FBStringCore& FBStringCore::insert(size_type pos, const FBStringCore& s, size_type pos2, size_type n) {
    return insert(pos, s.c_str() + pos2, n);
}

// 在指定位置插入 n 个字符 c
FBStringCore& FBStringCore::insert(size_type pos, size_type n, char c) {
    std::string temp(n, c);
    return insert(pos, temp.c_str(), n);
}

// 在迭代器位置插入字符 c
FBStringCore::iterator FBStringCore::insert(iterator it, char c) {
    size_type pos = it - begin();
    insert(pos, 1, c);
    return begin() + pos;
}

// 在迭代器位置插入 n 个字符 c
void FBStringCore::insert(iterator it, size_type n, char c) {
    size_type pos = it - begin();
    insert(pos, n, c);
}

// 在迭代器位置插入迭代器范围内的字符
void FBStringCore::insert(iterator it, const_iterator first, const_iterator last) {
    size_type pos = it - begin();
    insert(pos, first, last - first);
}

// 删除指定位置的 n 个字符
FBStringCore& FBStringCore::erase(size_type pos, size_type n) {
    if (pos + n > size()) return *this;
    if (type_ == StorageType::Small) {
        std::memmove(storage_.small_ + pos, storage_.small_ + pos + n, smallSize() - pos - n);
        setSmallSize(smallSize() - n);
    } else {
        std::memmove(storage_.ml_.data_ + pos, storage_.ml_.data_ + pos + n, storage_.ml_.size_ - pos - n);
        storage_.ml_.size_ -= n;
    }
    return *this;
}

// 删除迭代器位置的字符
FBStringCore::iterator FBStringCore::erase(iterator pos) {
    size_type index = pos - begin();
    erase(index, 1);
    return begin() + index;
}

// 删除迭代器范围内的字符
FBStringCore::iterator FBStringCore::erase(iterator first, iterator last) {
    size_type pos = first - begin();
    size_type n = last - first;
    erase(pos, n);
    return begin() + pos;
}

// 替换指定位置的 n 个字符为 C 风格字符串
FBStringCore& FBStringCore::replace(size_type p0, size_type n0, const char* s) {
    return replace(p0, n0, s, std::strlen(s));
}

// 替换指定位置的 n0 个字符为 C 风格字符串的前 n 个字符
FBStringCore& FBStringCore::replace(size_type p0, size_type n0, const char* s, size_type n) {
    erase(p0, n0);
    insert(p0, s, n);
    return *this;
}

// 替换指定位置的 n 个字符为 FBStringCore 对象
FBStringCore& FBStringCore::replace(size_type p0, size_type n0, const FBStringCore& s) {
    return replace(p0, n0, s.c_str(), s.size());
}

// 替换指定位置的 n0 个字符为 FBStringCore 对象中的部分字符
FBStringCore& FBStringCore::replace(size_type p0, size_type n0, const FBStringCore& s, size_type pos, size_type n) {
    return replace(p0, n0, s.c_str() + pos, n);
}

// 替换指定位置的 n0 个字符为 n 个字符 c
FBStringCore& FBStringCore::replace(size_type p0, size_type n0, size_type n, char c) {
    std::string temp(n, c);
    return replace(p0, n0, temp.c_str(), n);
}

// 替换迭代器范围内的字符为 C 风格字符串
FBStringCore& FBStringCore::replace(iterator first0, iterator last0, const char* s) {
    size_type pos = first0 - begin();
    size_type n0 = last0 - first0;
    return replace(pos, n0, s);
}

// 替换迭代器范围内的字符为 C 风格字符串的前 n 个字符
FBStringCore& FBStringCore::replace(iterator first0, iterator last0, const char* s, size_type n) {
    size_type pos = first0 - begin();
    size_type n0 = last0 - first0;
    return replace(pos, n0, s, n);
}

// 替换迭代器范围内的字符为 FBStringCore 对象
FBStringCore& FBStringCore::replace(iterator first0, iterator last0, const FBStringCore& s) {
    size_type pos = first0 - begin();
    size_type n0 = last0 - first0;
    return replace(pos, n0, s.c_str(), s.size());
}

// 替换迭代器范围内的字符为 n 个字符 c
FBStringCore& FBStringCore::replace(iterator first0, iterator last0, size_type n, char c) {
    size_type pos = first0 - begin();
    size_type n0 = last0 - first0;
    return replace(pos, n0, n, c);
}

// 替换迭代器范围内的字符为另一个迭代器范围内的字符
FBStringCore& FBStringCore::replace(iterator first0, iterator last0, const_iterator first, const_iterator last) {
    size_type pos = first0 - begin();
    size_type n0 = last0 - first0;
    return replace(pos, n0, first, last - first);
}

// 交换当前字符串与另一个字符串的值
void FBStringCore::swap(FBStringCore& s2) {
    std::swap(storage_, s2.storage_);
    std::swap(type_, s2.type_);
}

// 拷贝字符串中的字符到字符数组
FBStringCore::size_type FBStringCore::copy(char* s, size_type n, size_type pos) const {
    if (pos > size()) return 0;
    size_type len = std::min(n, size() - pos);
    std::memcpy(s, c_str() + pos, len);
    return len;
}

// 返回子字符串
FBStringCore FBStringCore::substr(size_type pos, size_type n) const {
    if (pos > size()) throw std::out_of_range("Index out of range");
    return FBStringCore(c_str() + pos, std::min(n, size() - pos));
}

// 比较两个字符串是否相等
bool FBStringCore::operator==(const FBStringCore& other) const {
    return size() == other.size() && std::memcmp(c_str(), other.c_str(), size()) == 0;
}

// 比较两个字符串是否不相等
bool FBStringCore::operator!=(const FBStringCore& other) const {
    return !(*this == other);
}

// 比较两个字符串大小
bool FBStringCore::operator<(const FBStringCore& other) const {
    return std::lexicographical_compare(c_str(), c_str() + size(), other.c_str(), other.c_str() + other.size());
}

bool FBStringCore::operator<=(const FBStringCore& other) const {
    return !(other < *this);
}

bool FBStringCore::operator>(const FBStringCore& other) const {
    return other < *this;
}

bool FBStringCore::operator>=(const FBStringCore& other) const {
    return !(*this < other);
}

// 比较当前字符串和另一个字符串的大小
int FBStringCore::compare(const FBStringCore& s) const {
    return compare(0, size(), s);
}

// 比较当前字符串的子串和另一个字符串的大小
int FBStringCore::compare(size_type pos, size_type n, const FBStringCore& s) const {
    return compare(pos, n, s.c_str(), s.size());
}

// 比较当前字符串的子串和另一个字符串的子串的大小
int FBStringCore::compare(size_type pos, size_type n, const FBStringCore& s, size_type pos2, size_type n2) const {
    return compare(pos, n, s.c_str() + pos2, n2);
}

// 比较当前字符串和 C 风格字符串的大小
int FBStringCore::compare(const char* s) const {
    return compare(0, size(), s, std::strlen(s));
}

// 比较当前字符串的子串和 C 风格字符串的大小
int FBStringCore::compare(size_type pos, size_type n, const char* s) const {
    return compare(pos, n, s, std::strlen(s));
}

// 比较当前字符串的子串和 C 风格字符串前 pos2 个字符的大小
int FBStringCore::compare(size_type pos, size_type n, const char* s, size_type pos2) const {
    size_type len1 = std::min(n, size() - pos);
    size_type len2 = std::min(pos2, std::strlen(s));
    int cmp = std::memcmp(c_str() + pos, s, std::min(len1, len2));
    if (cmp != 0) return cmp;
    if (len1 < len2) return -1;
    if (len1 > len2) return 1;
    return 0;
}

// 查找字符在字符串中的位置
FBStringCore::size_type FBStringCore::find(char c, size_type pos) const {
    const char* result = std::strchr(c_str() + pos, c);
    return result ? result - c_str() : npos;
}

// 查找 C 风格字符串在字符串中的位置
FBStringCore::size_type FBStringCore::find(const char* s, size_type pos) const {
    const char* result = std::strstr(c_str() + pos, s);
    return result ? result - c_str() : npos;
}

// 查找 C 风格字符串的前 n 个字符在字符串中的位置
FBStringCore::size_type FBStringCore::find(const char* s, size_type pos, size_type n) const {
    FBStringCore temp(s, n);
    return find(temp.c_str(), pos);
}

// 查找 FBStringCore 对象在字符串中的位置
FBStringCore::size_type FBStringCore::find(const FBStringCore& s, size_type pos) const {
    return find(s.c_str(), pos);
}

// 从后向前查找字符在字符串中的位置
FBStringCore::size_type FBStringCore::rfind(char c, size_type pos) const {
    for (size_type i = std::min(pos, size()); i-- > 0;) {
        if ((*this)[i] == c) return i;
    }
    return npos;
}

// 从后向前查找 C 风格字符串在字符串中的位置
FBStringCore::size_type FBStringCore::rfind(const char* s, size_type pos) const {
    size_type len = std::strlen(s);
    if (len > size()) return npos;
    for (size_type i = std::min(pos, size() - len); i-- > 0;) {
        if (std::memcmp(c_str() + i, s, len) == 0) return i;
    }
    return npos;
}

// 从后向前查找 C 风格字符串的前 n 个字符在字符串中的位置
FBStringCore::size_type FBStringCore::rfind(const char* s, size_type pos, size_type n) const {
    FBStringCore temp(s, n);
    return rfind(temp.c_str(), pos);
}

// 从后向前查找 FBStringCore 对象在字符串中的位置
FBStringCore::size_type FBStringCore::rfind(const FBStringCore& s, size_type pos) const {
    return rfind(s.c_str(), pos);
}

// 查找字符串中第一个出现的字符
FBStringCore::size_type FBStringCore::find_first_of(char c, size_type pos) const {
    return find(c, pos);
}

// 查找字符串中第一个出现的 C 风格字符串
FBStringCore::size_type FBStringCore::find_first_of(const char* s, size_type pos) const {
    for (size_type i = pos; i < size(); ++i) {
        if (std::strchr(s, (*this)[i])) return i;
    }
    return npos;
}

// 查找字符串中第一个出现的 C 风格字符串的前 n 个字符
FBStringCore::size_type FBStringCore::find_first_of(const char* s, size_type pos, size_type n) const {
    FBStringCore temp(s, n);
    return find_first_of(temp.c_str(), pos);
}

// 查找字符串中第一个出现的 FBStringCore 对象
FBStringCore::size_type FBStringCore::find_first_of(const FBStringCore& s, size_type pos) const {
    return find_first_of(s.c_str(), pos);
}

// 查找字符串中第一个不在指定字符集中的字符
FBStringCore::size_type FBStringCore::find_first_not_of(char c, size_type pos) const {
    for (size_type i = pos; i < size(); ++i) {
        if ((*this)[i] != c) return i;
    }
    return npos;
}

// 查找字符串中第一个不在指定 C 风格字符串中的字符
FBStringCore::size_type FBStringCore::find_first_not_of(const char* s, size_type pos) const {
    for (size_type i = pos; i < size(); ++i) {
        if (!std::strchr(s, (*this)[i])) return i;
    }
    return npos;
}

// 查找字符串中第一个不在指定 C 风格字符串的前 n 个字符中的字符
FBStringCore::size_type FBStringCore::find_first_not_of(const char* s, size_type pos, size_type n) const {
    FBStringCore temp(s, n);
    return find_first_not_of(temp.c_str(), pos);
}

// 查找字符串中第一个不在指定 FBStringCore 对象中的字符
FBStringCore::size_type FBStringCore::find_first_not_of(const FBStringCore& s, size_type pos) const {
    return find_first_not_of(s.c_str(), pos);
}

// 从后向前查找字符串中最后一个出现的字符
FBStringCore::size_type FBStringCore::find_last_of(char c, size_type pos) const {
    return rfind(c, pos);
}

// 从后向前查找字符串中最后一个出现的 C 风格字符串
FBStringCore::size_type FBStringCore::find_last_of(const char* s, size_type pos) const {
    for (size_type i = std::min(pos, size()); i-- > 0;) {
        if (std::strchr(s, (*this)[i])) return i;
    }
    return npos;
}

// 从后向前查找字符串中最后一个出现的 C 风格字符串的前 n 个字符
FBStringCore::size_type FBStringCore::find_last_of(const char* s, size_type pos, size_type n) const {
    FBStringCore temp(s, n);
    return find_last_of(temp.c_str(), pos);
}

// 从后向前查找字符串中最后一个出现的 FBStringCore 对象
FBStringCore::size_type FBStringCore::find_last_of(const FBStringCore& s, size_type pos) const {
    return find_last_of(s.c_str(), pos);
}

// 从后向前查找字符串中最后一个不在指定字符集中的字符
FBStringCore::size_type FBStringCore::find_last_not_of(char c, size_type pos) const {
    for (size_type i = std::min(pos, size()); i-- > 0;) {
        if ((*this)[i] != c) return i;
    }
    return npos;
}

// 从后向前查找字符串中最后一个不在指定 C 风格字符串中的字符
FBStringCore::size_type FBStringCore::find_last_not_of(const char* s, size_type pos) const {
    for (size_type i = std::min(pos, size()); i-- > 0;) {
        if (!std::strchr(s, (*this)[i])) return i;
    }
    return npos;
}

// 从后向前查找字符串中最后一个不在指定 C 风格字符串的前 n 个字符中的字符
FBStringCore::size_type FBStringCore::find_last_not_of(const char* s, size_type pos, size_type n) const {
    FBStringCore temp(s, n);
    return find_last_not_of(temp.c_str(), pos);
}

// 从后向前查找字符串中最后一个不在指定 FBStringCore 对象中的字符
FBStringCore::size_type FBStringCore::find_last_not_of(const FBStringCore& s, size_type pos) const {
    return find_last_not_of(s.c_str(), pos);
}

// 返回字符串的大小
FBStringCore::size_type FBStringCore::size() const {
    return type_ == StorageType::Small ? smallSize() : storage_.ml_.size_;
}

// 返回字符串的长度
FBStringCore::size_type FBStringCore::length() const {
    return size();
}

// 判断字符串是否为空
bool FBStringCore::empty() const {
    return size() == 0;
}

// 返回当前容量
FBStringCore::size_type FBStringCore::capacity() const {
    return type_ == StorageType::Small ? 22 : storage_.ml_.capacity_;
}

// 返回可存放的最大字符串长度
FBStringCore::size_type FBStringCore::max_size() const {
    return std::numeric_limits<size_type>::max();
}

// 返回字符串的起始位置迭代器
FBStringCore::iterator FBStringCore::begin() {
    return type_ == StorageType::Small ? storage_.small_ : storage_.ml_.data_;
}

// 返回字符串的起始位置常量迭代器
FBStringCore::const_iterator FBStringCore::begin() const {
    return type_ == StorageType::Small ? storage_.small_ : storage_.ml_.data_;
}

// 返回字符串的结束位置迭代器
FBStringCore::iterator FBStringCore::end() {
    return begin() + size();
}

// 返回字符串的结束位置常量迭代器
FBStringCore::const_iterator FBStringCore::end() const {
    return begin() + size();
}

// 返回字符串的最后一个字符位置的反向迭代器
FBStringCore::reverse_iterator FBStringCore::rbegin() {
    return reverse_iterator(end());
}

// 返回字符串的最后一个字符位置的常量反向迭代器
FBStringCore::const_reverse_iterator FBStringCore::rbegin() const {
    return const_reverse_iterator(end());
}

// 返回字符串第一个字符位置的前面的反向迭代器
FBStringCore::reverse_iterator FBStringCore::rend() {
    return reverse_iterator(begin());
}

// 返回字符串第一个字符位置的前面的常量反向迭代器
FBStringCore::const_reverse_iterator FBStringCore::rend() const {
    return const_reverse_iterator(begin());
}

// 输入操作符重载
std::istream& operator>>(std::istream& in, FBStringCore& s) {
    std::string temp;
    in >> temp;
    s = temp.c_str();
    return in;
}

// 输出操作符重载
std::ostream& operator<<(std::ostream& out, const FBStringCore& s) {
    out << s.c_str();
    return out;
}

// 从输入流中读取字符串
std::istream& getline(std::istream& in, FBStringCore& s, char delim) {
    std::string temp;
    std::getline(in, temp, delim);
    s = temp.c_str();
    return in;
}

// 初始化小型存储
void FBStringCore::initSmall(const char* str, size_type size) {
    std::memcpy(storage_.small_, str, size);
    setSmallSize(size);
    storage_.small_[size] = '\0';
    type_ = StorageType::Small;
}

// 初始化中型存储
void FBStringCore::initMedium(const char* str, size_type size) {
    storage_.ml_.data_ = allocate(size + 1);
    std::memcpy(storage_.ml_.data_, str, size);
    storage_.ml_.data_[size] = '\0';
    storage_.ml_.size_ = size;
    storage_.ml_.capacity_ = size;
    storage_.ml_.refCount_ = new std::atomic<size_type>(1);
    type_ = StorageType::Medium;
}

// 初始化大型存储
void FBStringCore::initLarge(const char* str, size_type size) {
    storage_.ml_.data_ = allocate(size + 1);
    std::memcpy(storage_.ml_.data_, str, size);
    storage_.ml_.data_[size] = '\0';
    storage_.ml_.size_ = size;
    storage_.ml_.capacity_ = size;
    storage_.ml_.refCount_ = new std::atomic<size_type>(1);
    type_ = StorageType::Large;
}

// 从另一个实例复制
void FBStringCore::copyFrom(const FBStringCore& other) {
    switch (other.type_) {
        case StorageType::Small:
            initSmall(other.c_str(), other.size());
            break;
        case StorageType::Medium:
        case StorageType::Large:
            storage_ = other.storage_;
            type_ = other.type_;
            storage_.ml_.refCount_->fetch_add(1, std::memory_order_relaxed);
            break;
    }
}

// 销毁当前存储
void FBStringCore::destroy() {
    if (type_ == StorageType::Medium || type_ == StorageType::Large) {
        if (storage_.ml_.refCount_->fetch_sub(1, std::memory_order_acq_rel) == 1) {
            deallocate(storage_.ml_.data_);
            delete storage_.ml_.refCount_;
        }
    }
}

// 解除共享
void FBStringCore::unshare() {
    if (type_ == StorageType::Large && storage_.ml_.refCount_->load(std::memory_order_acquire) > 1) {
        size_type newCapacity = storage_.ml_.capacity_;
        char* newData = allocate(newCapacity + 1);
        std::memcpy(newData, storage_.ml_.data_, storage_.ml_.size_ + 1);
        storage_.ml_.refCount_->fetch_sub(1, std::memory_order_acq_rel);
        storage_.ml_.data_ = newData;
        storage_.ml_.refCount_ = new std::atomic<size_type>(1);
    }
}

// 分配内存
char* FBStringCore::allocate(size_type size) {
#ifdef USE_JEMALLOC
    return static_cast<char*>(je_malloc(size));
#else
    return static_cast<char*>(std::malloc(size));
#endif
}

// 释放内存
void FBStringCore::deallocate(char* ptr) {
#ifdef USE_JEMALLOC
    je_free(ptr);
#else
    std::free(ptr);
#endif
}

// 确定存储类型
FBStringCore::StorageType FBStringCore::determineType(size_type size) const {
    if (size <= 22) {
        return StorageType::Small;
    } else if (size <= 255) {
        return StorageType::Medium;
    } else {
        return StorageType::Large;
    }
}

// 设置小型存储的大小
void FBStringCore::setSmallSize(size_type s) {
    storage_.small_[23] = static_cast<char>(22 - s);
}

// 返回小型存储的大小
FBStringCore::size_type FBStringCore::smallSize() const {
    return 22 - static_cast<size_type>(storage_.small_[23]);
}

// 返回容量
FBStringCore::size_type FBStringCore::capacityImpl() const {
    return type_ == StorageType::Small ? 22 : storage_.ml_.capacity_;
}

// 重新分配内存
void FBStringCore::realloc(size_type newCapacity) {
    if (type_ == StorageType::Small) {
        char* newData = allocate(newCapacity + 1);
        std::memcpy(newData, storage_.small_, smallSize() + 1);
        initMedium(newData, smallSize());
        deallocate(newData);
    } else {
        char* newData = allocate(newCapacity + 1);
        std::memcpy(newData, storage_.ml_.data_, storage_.ml_.size_ + 1);
        deallocate(storage_.ml_.data_);
        storage_.ml_.data_ = newData;
        storage_.ml_.capacity_ = newCapacity;
    }
}

// 判断是否为小端
bool FBStringCore::isLittleEndian() {
    int num = 1;
    return *reinterpret_cast<char*>(&num) == 1;
}

// 对齐大小
FBStringCore::size_type FBStringCore::alignSize(size_type size) {
    return (size + 7) & ~7;
}

// 假设不可达
void FBStringCore::assumeUnreachable() {
#if defined(__GNUC__)
    __builtin_unreachable();
#elif defined(_MSC_VER)
    __assume(0);
#else
    std::abort();
#endif
}

## FBStringCore.h

- 默认构造函数
- 拷贝构造函数
- 有参构造函数 //用c字符串s初始化
- 有参构造函数 //用n个字符c初始化
- 赋值运算符函数
- 移动构造函数
- 移动赋值运算符重载
- 析构函数
- 
- c_str() //返回一个以null终止的c字符串
- *data() //返回一个非null终止的c字符数组
- 下标访问运算符重载
- at()
- int copy(char *s, int n, int pos = 0) const;
//把当前串中以pos开始的n个字符拷贝到以s为起始位置的字符数组中，返回实际拷贝的数目

-  
- capacity()
- max_size()
- size()
- length()
- empty()
- resize(int len, char c)
- 
- operator>>
- operator<<
- getline()
- 
- string &operator+=(const string &s);//把字符串s连接到当前字符串的结尾
-  string &append(const char *s);            //把c类型字符串s连接到当前字符串结尾
-  string &append(const char *s,int n);//把c类型字符串s的前n个字符连接到当前字符串结尾
-  string &append(const string &s);    //同operator+=()
-  string &append(const string &s,int pos,int n);//把字符串s中从pos开始的n个字符连接到当前字符串的结尾
-  string &append(int n,char c);        //在当前字符串结尾添加n个字符c
-  string &append(const_iterator first,const_iterator last);//把迭代器first和last之间的部分连接到当前字符串的结尾
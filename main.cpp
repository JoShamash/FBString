#include <cstdlib>
// 声明测试函数
void testStringPerformance();

int main() {
    testStringPerformance();

    // 调用 Python 脚本
    system("python plot_creation.py");
    system("python plot_find.py");
    system("python plot_memory.py");

    return 0;
}

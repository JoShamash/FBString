#include "FBString.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <unordered_set>

// 生成 Python 脚本
void generatePythonScript(const std::vector<double>& stdData, const std::vector<double>& fbData, const std::string& operation, const std::string& ylabel, size_t numIterations) {
    std::string filename = "plot_" + operation + ".py";
    std::ofstream script(filename);

    script << "import matplotlib.pyplot as plt\n";
    script << "x = ['small', 'medium', 'large']\n";
    script << "std_data = [" << stdData[0] << ", " << stdData[1] << ", " << stdData[2] << "]\n";
    script << "fb_data = [" << fbData[0] << ", " << fbData[1] << ", " << fbData[2] << "]\n";
    script << "plt.plot(x, std_data, 'r-', label='std::string')\n";
    script << "plt.plot(x, fb_data, 'b-', label='FBString')\n";
    script << "plt.xlabel('String Size')\n";
    script << "plt.ylabel('" << ylabel << "')\n";
    script << "plt.title('Performance Comparison: " << operation << " (Iterations: " << numIterations << ")')\n"; // 添加 numIterations
    script << "plt.legend()\n";
    script << "plt.savefig('plot_" << operation << ".png')\n";  // 保存图表为文件
    script << "plt.close()\n";  // 关闭图表

    script.close();
}

// 计算内存占用
size_t calculateMemoryUsage(const std::vector<std::string>& strings) {
    size_t totalSize = 0;
    for (const auto& str : strings) {
        totalSize += sizeof(str) + str.capacity();
    }
    return totalSize;
}

size_t calculateMemoryUsage(const std::vector<FBString>& strings) {
    size_t totalSize = 0;
    std::unordered_set<const char*> uniqueDataPointers;

    for (const auto& str : strings) {
        const char* dataPtr = str.c_str();
        if (uniqueDataPointers.find(dataPtr) == uniqueDataPointers.end()) {
            uniqueDataPointers.insert(dataPtr);
            totalSize += sizeof(str) + str.size(); // 使用 size() 估算内存
        }
    }
    return totalSize;
}

// 测试字符串创建和查找性能
void testStringPerformance() {
    const size_t numIterations = 100000;
    const size_t smallStringLength = 10;
    const size_t mediumStringLength = 100;
    const size_t largeStringLength = 1000;

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<char> distribution('a', 'z');

    auto randomString = [&](size_t length) {
        std::string str;
        for (size_t i = 0; i < length; ++i) {
            str += distribution(generator);
        }
        return str;
    };

    std::vector<double> stdCreationTimes, fbCreationTimes;
    std::vector<double> stdFindTimes, fbFindTimes;
    std::vector<size_t> stdMemoryUsages, fbMemoryUsages;

    auto testPerformance = [&](size_t stringLength, const std::string& testType) {
        std::cout << "Testing " << testType << " strings of length " << stringLength << std::endl;

        // 测试 std::string 创建性能
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::string> stdStrings;
        for (size_t i = 0; i < numIterations; ++i) {
            stdStrings.emplace_back(randomString(stringLength));
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> stdCreationDuration = end - start;
        stdCreationTimes.push_back(stdCreationDuration.count());
        std::cout << "std::string creation time: " << stdCreationDuration.count() << " seconds" << std::endl;

        // 记录 std::string 内存使用
        size_t stdMemoryUsage = calculateMemoryUsage(stdStrings);
        stdMemoryUsages.push_back(stdMemoryUsage);
        std::cout << "std::string memory usage: " << stdMemoryUsage << " bytes" << std::endl;

        // 测试 std::string find 性能
        start = std::chrono::high_resolution_clock::now();
        for (auto& str : stdStrings) {
            str.find("jowadshamashsadwadw");
            str.find("zyv");
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> stdFindDuration = end - start;
        stdFindTimes.push_back(stdFindDuration.count());
        std::cout << "std::string find time: " << stdFindDuration.count() << " seconds" << std::endl;

        // 测试 FBString 创建性能（直接构造和拷贝构造）
        auto testFBStringCreation = [&](size_t stringLength) {
            // 直接构造
            start = std::chrono::high_resolution_clock::now();
            std::vector<FBString> fbStrings;
            for (size_t i = 0; i < numIterations / 2; ++i) {
                fbStrings.emplace_back(randomString(stringLength).c_str());
            }
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> fbDirectCreationDuration = end - start;
            fbCreationTimes.push_back(fbDirectCreationDuration.count());
            std::cout << "FBString creation time (direct): " << fbDirectCreationDuration.count() << " seconds" << std::endl;

            // 记录 FBString 内存使用
            size_t fbMemoryUsage = calculateMemoryUsage(fbStrings);
            fbMemoryUsages.push_back(fbMemoryUsage);
            std::cout << "FBString memory usage: " << fbMemoryUsage << " bytes" << std::endl;

            // 拷贝构造
            start = std::chrono::high_resolution_clock::now();
            std::vector<FBString> fbCopyStrings;
            for (size_t i = 0; i < numIterations / 2; ++i) {
                fbCopyStrings.emplace_back(fbStrings[i]);
            }
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> fbCopyCreationDuration = end - start;
            fbCreationTimes.push_back(fbCopyCreationDuration.count());
            std::cout << "FBString creation time (copy): " << fbCopyCreationDuration.count() << " seconds" << std::endl;

            // 测试 FBString find 性能
            start = std::chrono::high_resolution_clock::now();
            for (auto& str : fbStrings) {
                str.find("jowadshamashsadwadw");
                str.find("zyv");
            }
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> fbFindDuration = end - start;
            fbFindTimes.push_back(fbFindDuration.count());
            std::cout << "FBString find time: " << fbFindDuration.count() << " seconds" << std::endl;
        };

        testFBStringCreation(stringLength);
    };

    testPerformance(smallStringLength, "small");
    testPerformance(mediumStringLength, "medium");
    testPerformance(largeStringLength, "large");

    // 在 testStringPerformance 函数中调用 generatePythonScript
    generatePythonScript(stdCreationTimes, fbCreationTimes, "creation", "Time (seconds)", numIterations);
    generatePythonScript(stdFindTimes, fbFindTimes, "find", "Time (seconds)", numIterations);

    // 转换内存使用数据类型
    std::vector<double> stdMemoryUsagesDouble(stdMemoryUsages.begin(), stdMemoryUsages.end());
    std::vector<double> fbMemoryUsagesDouble(fbMemoryUsages.begin(), fbMemoryUsages.end());
    generatePythonScript(stdMemoryUsagesDouble, fbMemoryUsagesDouble, "memory", "Memory Usage (bytes)", numIterations);
}

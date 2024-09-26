## 项目名称: FBStringProject

### 项目简介

FBStringProject 是一个用于比较 `std::string` 和自定义字符串类 `FBString` 在多场景下的内存占用和速度的项目。通过测试不同大小的字符串，评估两者在创建、查找和内存使用方面的性能。

### 项目结构

```
FBStringProject/
├── include/
│   ├── FBStringCore.h
│   ├── FBString.h
├── src/
│   ├── FBStringCore.cpp
│   ├── FBString.cpp
│   ├── main.cpp
├── CMakeLists.txt
└── README.md
```

### 构建和运行

1. 克隆项目：

    ```sh
    git clone <repository_url>
    cd FBStringProject
    ```

2. 创建构建目录并生成构建文件：

    ```sh
    mkdir build
    cd build
    cmake ..
    ```

3. 编译项目：

    ```sh
    make
    ```

4. 运行可执行文件：

    ```sh
    ./FBStringProject
    ```

该项目将输出 `std::string` 和 `FBString` 在多场景下的内存占用和速度比较结果，并生成相应的性能图表。

### 使用 jemalloc

如果使用 jemalloc 进行内存分配，可以在生成构建文件时添加 `-DUSE_JEMALLOC=ON` 选项：

```sh
cmake -DUSE_JEMALLOC=ON ..
```

确保你的系统上已经安装了 jemalloc 库。

### 项目介绍

1. **项目背景**：开发了一个自定义字符串类 `FBString`，并与标准库中的 `std::string` 进行性能和内存占用的比较。
2. **技术栈**：使用 C++、CMake、Python（用于生成性能图表）。
3. **职责和贡献**：
   - 设计并实现了 `FBString` 类，支持高效的字符串操作。
   - 编写了性能测试，比较 `FBString` 和 `std::string` 在多场景下的表现。
   - 使用 CMake 管理项目构建，确保跨平台兼容性。
4. **成果和影响**：
   - 通过测试，证明 `FBString` 在特定场景下比 `std::string` 更高效。
   - 项目代码结构清晰，易于扩展和维护。
   - 提升了团队对高效编程的认识。

---

### 代码功能概述

- **`main.cpp`**: 负责调用性能测试函数，并生成性能图表。
- **`FBStringCore` 和 `FBString`**: 实现了自定义字符串类，支持多种字符串操作。
- **性能测试**: 比较 `std::string` 和 `FBString` 在创建、查找和内存使用方面的性能。
- **Python 脚本生成**: 自动生成用于绘制性能图表的 Python 脚本。

通过这个项目，深入了解了字符串操作的性能优化，并掌握如何在 C++ 项目中集成 Python 脚本以生成可视化结果。
![image](https://github.com/user-attachments/assets/1f3b5207-f530-48af-afe5-a3b582a1ee77)


![plot_creation](https://github.com/user-attachments/assets/3830983d-02f3-43ce-8811-848209355428)
![plot_find](https://github.com/user-attachments/assets/86c1743f-a319-4df1-8b3d-63ec5a9cb971)
![plot_memory](https://github.com/user-attachments/assets/99b3cd4d-7a91-4028-b6e8-ae33d702cd4b)

# Software Renderer

一个使用 C++、Qt Widgets 和 Eigen 编写的 CPU 软光栅化练习项目。程序把颜色写入自建 RGBA 帧缓冲，再通过 Qt 的 `QImage` 显示；渲染核心包含线框绘制、三角形填充、重心坐标插值和深度缓冲。

## 当前实现

- **线段与线框**：使用 Bresenham 算法绘制线段和三角形边。
- **三角形光栅化**：包含扫描线填充和基于重心坐标的三角形覆盖测试。
- **深度与着色**：使用 Z-buffer 做深度比较，并根据三角形法线和固定光照方向计算平面光照。
- **模型与显示**：读取简单 OBJ 网格数据，写入 RGBA 帧缓冲，并用 Qt Widgets 窗口显示。
- **矩阵代码**：使用 Eigen 保存模型、相机、投影和视口矩阵。

这是一个仍在开发中的学习项目。相机和透视矩阵尚未完整接入渲染路径；目前 `My_Camera::transform()` 实际执行的是视口坐标变换。窗口中的 A/D 按键处理也还是空的占位代码。

## 环境要求

项目文件当前配置为：

- Windows、Visual Studio C++ 工具集 v143、x64
- Qt 6.3.2 MSVC 2019 64-bit，模块：Core、Gui、Widgets
- Eigen 3.3.3（通过 NuGet 包配置）

使用 Qt Visual Studio Tools 配置 Qt kit，并在 Visual Studio 中恢复 NuGet 包。若本机 Qt 或 Eigen 版本、安装位置不同，请相应调整项目设置。

## 构建和运行

1. 打开 `QtWidgetsApplication3.sln`。
2. 选择 **Debug | x64**，确认 Qt Visual Studio Tools 使用项目配置的 Qt kit，并恢复 Eigen NuGet 包。
3. 当前 `main.cpp` 尝试从相对路径 `african_head/african_head.obj` 加载模型。
4. 仓库中没有提交 `african_head` 模型目录。请将兼容的 OBJ 文件放到上述相对路径，或修改 `main.cpp` 中的路径；同时将 Visual Studio 的调试工作目录设为仓库根目录。
5. 编译并运行。程序创建 1000×1000 的 Qt 窗口；当模型文件存在且路径正确时，会绘制模型，并同时绘制一个用于演示的彩色三角形。

## 代码结构

| 文件 | 说明 |
| --- | --- |
| `canvas.cpp / canvas.h` | 像素写入、线段绘制、三角形填充、光照和 Z-buffer |
| `FrameBuffer.cpp / FrameBuffer.h` | RGBA 像素缓冲区 |
| `Vertex.cpp / Vertex.h` | 顶点位置与颜色 |
| `Triangle.cpp / Triangle.h` | 三角形顶点数据 |
| `Model.cpp / Model.h` | OBJ 网格读取与顶点数据管理 |
| `My_Camera.cpp / My_Camera.h` | Eigen 矩阵与视口变换代码 |
| `QtWidgetsApplication3.cpp / .h` | Qt 窗口和画布显示 |
| `main.cpp` | 应用入口和示例模型路径 |

## 模型文件

模型路径在 `main.cpp` 中设置。替换模型时，请确认文件路径相对于程序的工作目录有效，并检查 `Model.cpp` 当前支持的 OBJ 顶点和三角面格式。仓库不包含模型、纹理或材质资源。

## 备注

该仓库是图形学练习原型，重点在 CPU 侧的基础光栅化过程。当前版本没有 GPU 渲染后端，也没有模型浏览、交互式相机控制或完整的 OBJ/材质支持。

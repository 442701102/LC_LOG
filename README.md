# LC_LOG

LC_LOG 是一个高效的日志封装库，旨在为 C++ 应用提供一个简单、灵活且功能强大的日志管理解决方案。本库封装了多种常用的 C++ 日志库，并通过 [Conan](https://conan.io/) 管理依赖，以简化日志系统的集成和使用过程。

## 特性

- **多日志库支持**：支持集成多种流行的日志库（如 SPDLOG, GLOG, LOG4CPLUS, DLT 等）。
- **灵活配置**：支持通过命令行参数及 JSON 配置文件灵活配置日志。
- **便捷的依赖管理**：使用 Conan 管理依赖，自动化安装所需的日志库。

## 快速开始

### 准备工作

确保您的系统已安装 [Conan](https://conan.io/) 和 [CMake](https://cmake.org/)。这两个工具对于编译和依赖管理是必需的。

### 安装依赖项

初次运行项目前，需要安装所需的依赖项。打开终端，执行以下命令：

```bash
sudo apt-get update
sudo apt-get install libnsl-dev #log4cplus 依赖
./build.py --action=all
```

这个脚本会自动调用 Conan 安装所有必要的依赖项，并准备项目的构建环境。

### 编译项目

编译项目非常简单，只需运行默认的构建脚本：

```bash
./build.py
```

默认情况下，这将以 Debug 模式编译项目。如果需要以 Release 模式编译，请使用以下命令：

```bash
./build.py --build_type=Release
```


### 选择日志类型

LC_LOG 支持多种日志库，并允许您通过命令行参数来选择特定的日志实现。这提供了灵活性，使您可以根据项目需求或个人偏好来选择最合适的日志系统。

要指定日志类型，使用 `--log_type` 参数，并传入您希望使用的日志库标识。支持的日志类型包括：

- `CUSTOM` - 使用自定义日志实现。
- `SPDLOG` - 使用 SPDLOG 库。
- `GLOG` - 使用 Google GLOG 库。
- `LOG4CPLUS` - 使用 LOG4CPLUS 库。
- `DLT` - 使用 Diagnostic Log and Trace (DLT) 库。

例如，如果您想使用 SPDLOG 作为日志系统，可以这样运行构建脚本：

```bash
./build.py --log_type=SPDLOG
```

同样，如果您要在 Release 模式下编译项目，并且使用 GLOG 作为日志系统，您可以使用：

```bash
./build.py --build_type=Release --log_type=GLOG
```

这样的灵活性让您可以轻松切换日志系统，无需修改代码或进行复杂配置。


### 生成的文件

编译完成后，可执行文件和库文件将被放置在项目的 `build/output/` 目录下。

## 配置

LC_LOG 支持通过命令行参数和 JSON 配置文件来定制日志设置。更多详细的配置选项，请参阅 [配置文档](#)。

## 贡献

我们欢迎任何形式的贡献，无论是功能请求、反馈、文档改进还是代码提交。请通过 Issue 或 Pull Request 提交您的贡献。

## 许可证

本项目采用 MIT 许可证。有关详细信息，请参阅 `LICENSE` 文件。


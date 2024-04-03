# LC_LOG
Encapsulate the commonly used C++ log library and manage dependencies with conan.

1.初次运行调用./build.py --action=all脚本,安装依赖项
2.后续运行调用./build.py 默认编译,默认为debug模式
3.编译完成后，在主目录的build/outout/目录下生成可执行文件和库文件
#!/bin/bash

# build_x86_ubuntu.sh

# 获取当前脚本所在的绝对路径
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

# 引用 common_utils.sh
source "$SCRIPT_DIR/common_utils.sh"

DEFAULT_HOST_PROFILE="profiles_Ubuntu_20.04"
DEFAULT_ACTION="all"
DEFAULT_CONANFILE="conanfile.py"
DEFAULT_LOG_TYPE="CUSTOM"

# 帮助信息函数
show_help() {
    echo "Usage: $0 [action] [conanfile] [HOST_PROFILE] [test] [LOG_TYPE]"
    echo "***** action: Build action to perform, default is 'all'"
    echo "***** conanfile: Path to the conanfile, if not provided defaults will be used"
    echo "***** host_profile: Host profile, default is 'profiles_Ubuntu_20.04'"
    echo "***** test: If 'test' is passed as the fourth argument, test_package will be executed"
    echo "***** LOG_TYPE: set the log type, default is 'CUSTOM', it can be 'CUSTOM', 'SPDLOG', 'GLOG', 'LOG4CPLUS', 'DLT'"
    echo "***** --help, -h: Show this help message"
}

# 检查 --help 或 -h 参数
if [[ "$1" == "--help" ]] || [[ "$1" == "-h" ]]; then
    show_help
    exit 0
fi

# mkdir -p $BUILD_DIR
# pushd $BUILD_DIR

# 调用 run_build 函数


ACTION="${1:-$DEFAULT_ACTION}"
CONANFILE="${2:-$DEFAULT_CONANFILE}"
HOST_PROFILE="${3:-$DEFAULT_HOST_PROFILE}" #因为脚本明确了意图,所以这个参数不重要
BUILD_TYPE="${4:-Debug}" # it can be Release or Debug
LOG_TYPE="${5:-$DEFAULT_LOG_TYPE}" # it can be GLOG or SPDLOG


echo "LOG_TYPE" $LOG_TYPE

# 如果输入的是单字符 D 或 d，转换为 Debug
if [[ "$BUILD_TYPE" == "D" || "$BUILD_TYPE" == "d" ]]; then
    BUILD_TYPE="Debug"
fi

# 如果输入的是单字符 R 或 r，转换为 Release
if [[ "$BUILD_TYPE" == "R" || "$BUILD_TYPE" == "r" ]]; then
    BUILD_TYPE="Release"
fi
# 转换绝对路径
if [[ -n "$CONANFILE" ]]; then
    CONANFILE=$(realpath "$CONANFILE")
fi

# 调用 run_build 函数
run_build "$HOST_PROFILE" "$ACTION" "$CONANFILE" "$BUILD_TYPE" "$LOG_TYPE"

#如果 ACTION 为 c create,则不执行后续操作
if [ "$ACTION" == "c" ] || [ "$ACTION" == "create" ]; then
    exit 0
fi
echo "start cmake build..."
# 如果cd失败就报错退出
cd "$SCRIPT_DIR/../build" || { echo "Failed to change build directory"; exit 1; }
cmake .. -DCMAKE_INSTALL_PREFIX:PATH="$(pwd)/output/" -DCMAKE_TOOLCHAIN_FILE="$(pwd)/${BUILD_TYPE}/generators/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" -G "Unix Makefiles"
cmake --build . --target install

if [ -d $PROJECT_DIR/test_package ] && [ "$4" == "test" ];then # 暂未开发 
  echo "start test_package ..."
  cp -r $PROJECT_DIR/test_package .
#  cp $PROJECT_DIR/test_package/config/example.ini $BUILD_DIR/test_package/build/Release
  # conan test $BUILD_DIR/test_package  
fi



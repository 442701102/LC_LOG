#!/usr/bin/env python3
"""
This module provides a script for running Conan commands with optional arguments.
It is specifically designed for Conan 2.1 and allows for setting various paths and
profiles for the Conan package manager. The script also supports deploying the
artifacts to a specified directory.

.. moduleauthor:: liuzhiyu (liuzhiyu27@foxmail.com)

:copyright: Copyright (c) 2024 by liuzhiyu, All Rights Reserved.
"""

# 导入模块
import argparse
import subprocess
import os
import sys

# 定义全局变量
default_conan_home = ""
default_build_type = "Debug"
default_build_py = ""
default_install_py="conanfile.py"
default_create_py="recipe_dlt.py"
default_profiles_name = "profiles_Ubuntu_20.04"
default_deployer_name = "deployer_thirdparty"


def run_conan_command(command, env_vars):
    """
    通用函数用于执行Conan命令。
    """
    # 打印命令
    print("Executing command:", " ".join(command))

    # 运行命令并捕获标准输出和标准错误输出
    result = subprocess.run(command, capture_output=True, text=True, env=env_vars)

    # 打印标准输出和标准错误输出
    print("Standard Output:\n", result.stdout)
    print("Standard Error:\n", result.stderr)
    
def create_package(args, env_vars):
    print(sys.argv)

    # 检查命令行参数，决定使用哪个参数
    if any(arg.startswith('--conanfile_create=') for arg in sys.argv):
        conanfile = os.path.join(os.getcwd(), args.conanfile_create)
    elif any(arg.startswith('--conanfile=') for arg in sys.argv):
        conanfile = os.path.join(os.getcwd(), args.conanfile)
    else:
        conanfile = os.path.join(os.getcwd(), default_create_py)

    print("create_package conanfile path:", conanfile)

    command = [
        "conan",
        "create",
        conanfile,
        "--build=missing",
        "-o",
        "*:shared=True",
        "--test-folder="
    ]

    command.extend([
        f"--profile:host={os.path.join(args.profiles_path, args.host_profile)}",
        f"--profile:build={os.path.join(args.profiles_path, args.build_profile)}"
    ])

    run_conan_command(command, env_vars)

def install_package(args, env_vars):
    global default_deployer_name

    # 检查命令行参数，决定使用哪个参数
    if any(arg.startswith('--conanfile_install=') for arg in sys.argv):
        conanfile = os.path.join(os.getcwd(), args.conanfile_install)
    elif any(arg.startswith('--conanfile=') for arg in sys.argv):
        conanfile = os.path.join(os.getcwd(), args.conanfile)
    else:
        conanfile = os.path.join(os.getcwd(), default_install_py)


    print("install_package conanfile path:", conanfile)
    # 构建命令
    command = [
        "conan",
        "install",
        conanfile,
        "--build=missing",
        "-o",
        "*:shared=True",
        "-s",
        "build_type=" + args.build_type
    ]
  

    command.extend([
        f"--profile:host={os.path.join(args.profiles_path, args.host_profile)}",
        f"--profile:build={os.path.join(args.profiles_path, args.build_profile)}"
    ])

    # 如果提供了部署器名称，则添加相关参数
    if args.deployer_name:
        default_deployer_name = args.deployer_name

    command.extend([
        f"--deployer={os.path.join(args.deploy_path, default_deployer_name)}",
        f"--deployer-folder={os.path.join(os.getcwd(), 'third-party')}"
    ])
    run_conan_command(command, env_vars)

def main():
    """
    Main function that parses command-line arguments and runs the Conan command.

    :param --conan_home: Path to the Conan home directory.
    :param --profiles_path: Path to the configuration profiles.
    :param --deploy_path: Path to deploy the artifacts.
    :param --host_profile: Conan host profile to use.
    :param --build_profile: Conan build profile to use. Defaults to host profile if not provided.
    :param --deployer_name: Name of the deployer to use.
    """
    # 创建一个解析器对象
    parser = argparse.ArgumentParser(description="Run Conan commands with optional arguments.")

    # 添加命令行参数
    parser.add_argument("--action", choices=['create', 'install', 'all'], default='install', help="Action to perform: create, install, or all. Defaults to 'all'.")
    parser.add_argument("--conan_home", help="Path to the Conan home directory.", default=default_conan_home)
    parser.add_argument("--build_type", help="Path to the Conan home directory.", default=default_build_type)
    parser.add_argument("--conanfile", help="Name of the conanfile to use.", default=default_build_py)
    parser.add_argument("--conanfile_install", help="Name of the conanfile to use.", default=default_install_py)
    parser.add_argument("--conanfile_create", help="Name of the conanfile to use.", default=default_create_py)

    parser.add_argument("--profiles_path", help="Path to the configuration profiles.", default=os.path.join(os.getcwd(), "profiles"))
    parser.add_argument("--build_profile", help="Conan build profile to use. ", default=default_profiles_name)#构建配置文件
    parser.add_argument("--host_profile", help="Conan host profile to use.", default=default_profiles_name)#主机配置文件

    # use to install
    parser.add_argument("--deploy_path", help="Path to deploy the artifacts.", default=os.path.join(os.getcwd(), "extensions", "deployers"))
    parser.add_argument("--deployer_name", help="Name of the deployer to use.")

    # 解析命令行参数
    args = parser.parse_args()

    print("args:", args)   
    



    # 设置环境变量
    env_vars = os.environ.copy()
    #env_vars["CONAN_HOME"] = args.conan_home

    if args.action == "create":
        create_package(args, env_vars)
    elif args.action == "install":
        install_package(args, env_vars)
    elif args.action == "all":
        create_package(args, env_vars)
        install_package(args, env_vars)



if __name__ == "__main__":
    main()
    # 设置构建类型
    BUILD_TYPE = default_build_type
    print("BUILD_TYPE:", BUILD_TYPE)

    # 主目录路径，这里假设脚本位于主目录
    main_dir = os.path.dirname(os.path.abspath(__file__))

    # 构建目录路径
    build_dir = os.path.join(main_dir, "build")

    # 确保构建目录存在
    os.makedirs(build_dir, exist_ok=True)

    # 切换到构建目录
    os.chdir(build_dir)
    print("build_dir:", build_dir)
    print("main_dir:", main_dir)

    # 构建命令
    cmake_configure_command = [
        "cmake",
        "..",
        f"-DCMAKE_INSTALL_PREFIX:PATH={os.path.join(build_dir, 'output/')}",
        f"-DCMAKE_TOOLCHAIN_FILE={os.path.join(build_dir, f'{BUILD_TYPE}/generators/conan_toolchain.cmake')}",
        f"-DCMAKE_BUILD_TYPE={BUILD_TYPE}",
        f"-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
        f"-DCMAKE_VERBOSE_MAKEFILE=ON",
        f"-DCMAKE_MODULE_PATH={os.path.join(build_dir, f'{BUILD_TYPE}/generators/')}",
        f"-Dlog4cplus_DIR={os.path.join(build_dir, f'{BUILD_TYPE}/generators/')}",
        f"-Dfmt_DIR={os.path.join(build_dir, f'{BUILD_TYPE}/generators/')}",
        f"-Dgflags_DIR={os.path.join(build_dir, f'{BUILD_TYPE}/generators/')}",
        f"-Dglog_DIR={os.path.join(build_dir, f'{BUILD_TYPE}/generators/')}",
        f"-Ddlt-daemon_DIR={os.path.join(build_dir, f'{BUILD_TYPE}/generators/')}",
        f"-Dspdlog_DIR={os.path.join(build_dir, f'{BUILD_TYPE}/generators/')}",
        f"-Dlibunwind_DIR={os.path.join(build_dir, f'{BUILD_TYPE}/generators/')}",
        f"-DLibLZMA_DIR={os.path.join(build_dir, f'{BUILD_TYPE}/generators/')}",
        f"-DZLIB_DIR={os.path.join(build_dir, f'{BUILD_TYPE}/generators/')}", 
        "-G",
        "Unix Makefiles"
    ]

    cmake_build_command = [
        "cmake",
        "--build",
        ".",
        "--target",
        "install"
    ]

    # 执行 CMake 配置
    subprocess.run(cmake_configure_command, check=True)

    # 执行 CMake 构建
    subprocess.run(cmake_build_command, check=True)
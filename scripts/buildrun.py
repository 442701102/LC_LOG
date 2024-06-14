#!/usr/bin/env python3
import os
import shutil
import sys

# 获取脚本的绝对路径
script_dir = os.path.dirname(os.path.abspath(__file__))
# 根据绝对路径定位到LC_LOG/third-party
third_party_dir = os.path.join(script_dir, '..', 'third-party')
third_party_dir = os.path.abspath(third_party_dir)

# 在LC_LOG/third-party创建一个runlib目录
runlib_dir = os.path.join(third_party_dir, 'runlib')
os.makedirs(runlib_dir, exist_ok=True)

lib_env_var = ''  # 先定义变量，确保其在后面的代码中已定义

# 确定当前操作系统类型并设置相应的动态库扩展名
if sys.platform.startswith("linux"):
    lib_exts = ['.so']  # 包括.so文件和其版本化的文件
    lib_env_var = 'LD_LIBRARY_PATH'
elif sys.platform.startswith("win"):
    lib_exts = ['.dll']
    lib_env_var = 'PATH'
else:
    raise Exception("Unsupported OS")

# 遍历LC_LOG/third-party下所有子目录拷贝所有动态库文件到runlib目录下
# 遍历third-party下所有子目录，拷贝所有动态库文件到runlib目录下
for root, dirs, files in os.walk(third_party_dir):
    # 跳过runlib目录
    if 'runlib' in dirs:
        dirs.remove('runlib')

    for file in files:
        if any(file.endswith(ext) or (ext in file and '.so.' in file) for ext in lib_exts):
            src_file = os.path.join(root, file)
            dst_file = os.path.join(runlib_dir, file)
            
            # 避免将文件复制到自身
            if os.path.abspath(src_file) != os.path.abspath(dst_file):
                try:
                    shutil.copy2(src_file, dst_file)
                    print(f"Copied '{src_file}' to '{dst_file}'")
                except Exception as e:
                    print(f"Error copying '{src_file}' to '{dst_file}': {e}")
# # 5. 设置运行时搜索库路径为runlib目录
# # 注意：这只会影响当前Python脚本运行的环境，不会永久改变系统环境变量
current_path = os.environ.get(lib_env_var, '')
if runlib_dir not in current_path:
    new_path = f"{runlib_dir}:{current_path}" if current_path else runlib_dir
    os.environ[lib_env_var] = new_path

print(f"请将环境变量 {lib_env_var} 临时设置为: {os.environ[lib_env_var]},例如: export {lib_env_var}={os.environ[lib_env_var]}")


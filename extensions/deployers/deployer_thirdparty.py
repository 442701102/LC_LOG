import os
import shutil
from conan.tools.files import copy

def deploy(graph, output_folder: str, **kwargs):
    # 遍历依赖图中的所有节点
    for node in graph.nodes:
        # 获取依赖项的名称和版本
        dep_name = node.ref.name
        #dep_version 不是字符串，而是 Version 对象。可以通过在构造 dest_folder 路径时将 dep_version 转换为字符串
        dep_version = str(node.ref.version)  # Convert Version object to string
        # 打印依赖项信息
        print(f"Deploying {dep_name} version {dep_version}")
        # 获取包的路径
        package_folder = node.conanfile.package_folder
        # 构建目标文件夹路径
        dest_folder = os.path.join(output_folder, dep_name, dep_version)
        # 如果目标文件夹不存在，则创建它
        if not os.path.exists(dest_folder):
            os.makedirs(dest_folder)
        # 复制文件夹内容
        if package_folder:
            # 使用Conan的copy工具来复制文件，以便更好地处理文件复制过程中的各种情况
            copy(node.conanfile, "*", package_folder, dest_folder)

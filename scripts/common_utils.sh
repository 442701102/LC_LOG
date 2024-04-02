#!/bin/bash
#  
update_paths() {
    local new_path="$1"
    local file_path="$2"
    local lib_name="$3"
    local lib_version="$4"

    # Check if the new path is a valid directory
    if [ ! -d "$new_path" ]; then
        echo "Error: '$new_path' is not a valid directory"
        return 1
    fi

    # Replace the paths in the specified file
    sed -i "s|source_src = \".*\"|source_src = \"$new_path\"|g" "$file_path"
    last_part=$(basename "$new_path")

    sed -i "s|self.folders.source = \".*\"|self.folders.source = \"$last_part/\"|g" "$file_path"
    # name =
    sed -i "s|name = \".*\"|name = \"$lib_name\"|g" "$file_path"
    # version =
    sed -i "s|version = \".*\"|version = \"$lib_version\"|g" "$file_path"


    echo "Updated paths in $file_path to '$new_path'"
}
update_zip_url() {
    local new_url="$1"
    local file_path="$2"
    local lib_name="$3"
    local lib_version="$4"


    # Replace the URL in the specified file
    sed -i "s|get(self, \".*\"|get(self, \"$new_url\"|g" "$file_path"
    # name =
    sed -i "s|name = \".*\"|name = \"$lib_name\"|g" "$file_path"
    # version =
    sed -i "s|version = \".*\"|version = \"$lib_version\"|g" "$file_path"


    echo "Updated URL in $file_path to '$new_url'"
}

# Run the function with the provided argument
run_build() {

    local script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
    local python_script="$script_dir/../build.py" # Python 构建的脚本路径
#    echo "Script directory: $script_dir"
#    echo "Python script path: $python_script"

    local default_action="all" #默认全部执行,不建议修改
    local default_profiles_path="$script_dir/../profiles" # 默认构建配置文件路径,不建议修改
    local default_deploy_path="$script_dir/../extensions/deployers" # 默认部署路径,不建议修改

    local default_deployer_name="deployer_thirdparty" # 按需求修改
    local default_host_profile="profiles_Ubuntu_20.04" # 按需求修改
    local default_build_type="Release" # 按需求修改

    local default_conanfile="" # 如果无效，则不传递给 Python 脚本,使用build.py中的默认值
    local default_conan_home="/path/to/conan/home" # 未使用 
    


    local host_profile="$1" #通过上层传递构建的编译配置文件
    local action=${2:-$default_action}
    local conanfile=${3:-$default_conanfile}
    local buile_type=${4:-$default_build_type}
    echo "Build profile: $host_profile"
    echo "Action: $action"
    echo "Conanfile: $conanfile"
############ 参数 1 为构建配置文件，参数 2 为构建动作，参数 3 为 Conanfile ############

############ 参数 1 为构建配置文件 ############
    # 如果 host_profile 参数为 "d" 或 "default"，则使用默认构建配置文件
    if [ "$host_profile" == "d" ] || [ "$host_profile" == "default" ]; then
        host_profile=$default_host_profile
    fi

    # 检查构建配置文件是否存在,如果不存在则使用默认构建配置文件
    if [ ! -f "$default_profiles_path/$host_profile" ]; then
        echo "Error: BUILD_PROFILE does not point to a valid file: $default_profiles_path/$host_profile"
        host_profile=$default_host_profile
    fi

############ 参数 2 为构建动作 ,先调用conan create 然后再调用conan install ############

    # 根据输入参数调整action的值
    case "$action" in
        build|install|i)
            action="install"
            ;;
        create|c)
            action="create"
            ;;
        *)
            action="all"
            ;;
    esac
############ 参数 3 为 Conanfile ############
    # 检查用户提供的值或默认值是否指向一个有效文件
    if [ "$conanfile" == "default" ] || [ "$conanfile" == "d" ] || [ -z "$conanfile" ]; then
        if [ -f "$default_conanfile" ]; then
            conanfile=$default_conanfile
        fi
    else
        if [ ! -f "$conanfile" ]; then
            echo "Error: CONANFILE does not point to a valid file: $conanfile"
        fi
    fi
############ 打印参数 ############
    echo "Build profile: $host_profile"

    echo "Conanfile: $conanfile"
############ 调用 Python 脚本并传递参数 ############
    local cmd="python3 $python_script \
        --action=$action \
        --profiles_path=$default_profiles_path \
        --host_profile=$host_profile \
        --deploy_path=$default_deploy_path \
        --deployer_name=$default_deployer_name \
        --build_type=$buile_type"

    if [[ -n "$conanfile" && -f "$conanfile" ]]; then
        echo "Conanfile: $conanfile"
        cmd="$cmd --conanfile=$conanfile"
    fi
############ 执行命令 ############
    cd "$script_dir/../" || exit
    echo "Executing command: $cmd"
    eval "$cmd"
}
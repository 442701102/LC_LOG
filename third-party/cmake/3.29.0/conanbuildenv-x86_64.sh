script_folder="/home/conan/workspace/prod-v2/bsr/9023/fbebb/p/b/cmakea57ae9f7d4dea/b"
echo "echo Restoring environment" > "$script_folder/deactivate_conanbuildenv-x86_64.sh"
for v in 
do
    is_defined="true"
    value=$(printenv $v) || is_defined="" || true
    if [ -n "$value" ] || [ -n "$is_defined" ]
    then
        echo export "$v='$value'" >> "$script_folder/deactivate_conanbuildenv-x86_64.sh"
    else
        echo unset $v >> "$script_folder/deactivate_conanbuildenv-x86_64.sh"
    fi
done


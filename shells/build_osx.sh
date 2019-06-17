# This script builds the sample for x64 OSX. 
# It assumes that both the dotnet CLI and g++ compiler are available on the path.

BASEDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SRCDIR=${BASEDIR}/../csharp/
OUTDIR=${BASEDIR}/../build/bin/csharpLibs/

DIR_SRC_SHADERS=${BASEDIR}/../shaders/
DIR_SRC_TEXTURES=${BASEDIR}/../textures/
DIR_DST_SHADERS=${BASEDIR}/../build/bin/shaders/
DIR_DST_TEXTURES=${BASEDIR}/../build/bin/textures/

echo "------------"
echo ${BASEDIR}
echo ${SRCDIR}
echo ${OUTDIR}
echo "------------"

# Make output directory, if needed
if [ ! -d "${OUTDIR}" ]; then
    mkdir -p ${OUTDIR}
fi

if [ ! -d "${DIR_DST_SHADERS}" ]; then
    mkdir -p ${DIR_DST_SHADERS}
fi

if [ ! -d "${DIR_DST_TEXTURES}" ]; then
    mkdir -p ${DIR_DST_TEXTURES}
fi



# 递归复制 几个目录 ...
cp -R ${DIR_SRC_SHADERS}  ${DIR_DST_SHADERS} 
cp -R ${DIR_SRC_TEXTURES} ${DIR_DST_TEXTURES}

# Build managed component

# echo Building Managed Library
# echo dotnet publish --self-contained -r osx-x64 ${SRCDIR}/ManagedLibrary/ManagedLibrary.csproj -o ${OUTDIR}
# dotnet publish --self-contained -r osx-x64 ${SRCDIR}/ManagedLibrary/ManagedLibrary.csproj -o ${OUTDIR}

# -r osx-x64: 设置 目标平台
# -o 设置 “生成文件目录”
# dotnet 似乎会自己寻找 同目录下的所有 cs 文件。


#---- 推迟到 cmake 中执行 -----
# Build native component
# -D both LINUX and OSX since most LINUX code paths apply to OSX also
#g++ -o ${OUTDIR}/SampleHost -D LINUX -D OSX ${SRCDIR}/SampleHost_unix.cpp -ldl
# 
# -o XX: 指定生成的目标文件
# -D XX: 编译时添加宏XX，值为1
#        这也是为什么，实例代码中，存在 LINUX, OSX 这两个 宏
# -ldl:  如果在城中，使用了 dlopen、dlsym、dlclose、dlerror
#        显式加载动态库，则需要设置链接选项 -ldl

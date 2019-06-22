#!/bin/bash

#------------- prepare dirs ---------------

DIR_base="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DIR_out=${DIR_base}/build/publish/

DIR_src_shaders=${DIR_base}/shaders/
DIR_src_textures=${DIR_base}/textures/
DIR_dst_shaders=${DIR_out}/shaders/
DIR_dst_textures=${DIR_out}/textures/

echo "------------"
echo ${DIR_out}
echo "------------"

if [ ! -d "${DIR_out}" ]; then
    mkdir -p ${DIR_out}
fi

if [ ! -d "${DIR_dst_shaders}" ]; then
    mkdir -p ${DIR_dst_shaders}
fi

if [ ! -d "${DIR_dst_textures}" ]; then
    mkdir -p ${DIR_dst_textures}
fi

cp -R ${DIR_src_shaders}  ${DIR_dst_shaders} 
cp -R ${DIR_src_textures}  ${DIR_dst_textures} 


#------------- build cpp/c# ----------------
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../src_cpp/
make
cd ..
dotnet publish -c Release -r osx-x64 --self-contained true


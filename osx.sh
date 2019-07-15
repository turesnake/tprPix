#!/bin/bash

#------------- prepare dirs ---------------

DIR_base="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DIR_out=${DIR_base}/build/publish/

DIR_tprLog=${DIR_out}/tprLog/

DIR_src_shaders=${DIR_base}/shaders/
DIR_src_textures=${DIR_base}/textures/
DIR_src_jsons=${DIR_base}/jsons/

DIR_dst_shaders=${DIR_out}/shaders/
DIR_dst_textures=${DIR_out}/textures/
DIR_dst_jsons=${DIR_out}/jsons/


echo "------------"
echo ${DIR_out}
echo "------------"

if [ ! -d "${DIR_out}" ]; then
    mkdir -p ${DIR_out}
fi

if [ ! -d "${DIR_tprLog}" ]; then
    mkdir -p ${DIR_tprLog}
fi

if [ ! -d "${DIR_dst_shaders}" ]; then
    mkdir -p ${DIR_dst_shaders}
fi

if [ ! -d "${DIR_dst_textures}" ]; then
    mkdir -p ${DIR_dst_textures}
fi

if [ ! -d "${DIR_dst_jsons}" ]; then
    mkdir -p ${DIR_dst_jsons}
fi

cp -R ${DIR_src_shaders}   ${DIR_dst_shaders} 
cp -R ${DIR_src_textures}  ${DIR_dst_textures} 
cp -R ${DIR_src_jsons}     ${DIR_dst_jsons} 


#------------- build cpp/c# ----------------
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd ..


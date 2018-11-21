#===================================================#
#           CMakeLists.txt -- <根目录>/deps/tprLibs/tprBitMap/
#===================================================#
#-- 将 tprBitMap 包装成 静态库。 进一步 模块化


#-- 指定 依赖的 h文件所在 路径:
include_directories ( include/ )


#-- 定义一个列表，放置所有 cpp 文件 --
FILE(GLOB TPRBITMAP_SRC  src/*.cpp )

#-- 建立 目标 库对象 -- 
#-- 在 主CMakeLists.txt 文件中，以此名来调用 本库
add_library ( tprBitMap ${TPRBITMAP_SRC} )

#-- 更改 库 最终生成的 可执行二进制文件 的放置目录：build/lib
#-- 这个设置 意味着，每次 cmake/make, 本库都是需要重新编译的。
#-- 可以在后期，版本稳定后，改为 库内目录 
#-- 如果不定义此句，本库的 生成文件将放在 build/deps/frapfunc 中
#-- 也许更适合
#set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


#-- 将 库 的输出名字改为 tprBitMap.lib
set_target_properties (tprBitMap PROPERTIES OUTPUT_NAME "tprBitMap" )


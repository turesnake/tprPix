#===================================================#
#           CMakeLists.txt -- 根目录
#===================================================#
#  
#-------------------

#-- cmake 最小 版本号
cmake_minimum_required(VERSION 3.1)

#---------------------------------------------------#
#                    gcc
#---------------------------------------------------#
#-- 根据不同 OS系统，做不同设置
if( APPLE )
    option( _TPR_OS_APPLE_ " APPLE " ON )

elseif( UNIX AND NOT APPLE )
    option( _TPR_OS_UNIX_ " UNIX " ON )

elseif( WIN32 )
    option( _TPR_OS_WIN32_ " WIN32 " ON )

endif( APPLE )
#-- 此段 务必 在 设置 project 之前


#-- cpp 版本控制。 c++17
#-- CMAKE_CXX_STANDARD 是 cmake 3.1 后的新语句。可放心使用
#-- 当低于此版本时，.h文件中的 inline 关键词将失效
set (CMAKE_CXX_STANDARD 17)


#---------------------------------------------------#
#                project name
#---------------------------------------------------#
#-- 工程 名。不是 最终输出的 可执行二进制文件 的名字
project ( tprPixel )

#-- *** 测试 ***
#-- 设置 工程 版本号： 0.1
set ( VERSION_MAJOR 0 )
set ( VERSION_MINOR 1 )



#---------------------------------------------------#
#                   include
#---------------------------------------------------#
#-- 指定 依赖的 h文件所在 路径:
include_directories (${PROJECT_SOURCE_DIR}/include)
include_directories (${PROJECT_SOURCE_DIR}/include/test)

include_directories (${PROJECT_SOURCE_DIR}/src/gl)
include_directories (${PROJECT_SOURCE_DIR}/src/camera)
include_directories (${PROJECT_SOURCE_DIR}/src/game)
include_directories (${PROJECT_SOURCE_DIR}/src/game/map)
include_directories (${PROJECT_SOURCE_DIR}/src/game_src)
include_directories (${PROJECT_SOURCE_DIR}/src/random)

include_directories (${PROJECT_SOURCE_DIR}/deps)

include_directories (${PROJECT_SOURCE_DIR}/deps/glad/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/glfw/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/lodepng/include)

include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprBase/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprDataType)
include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprFileModeT)
include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/MsgIO/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/wrapFuncs/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprBitMap/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprDB/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprFileSys/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprCrypto/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprBinary/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprTimer/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprMemAllor/include)


include_directories (${PROJECT_BINARY_DIR}/sysconfig)

#-- 创建变量 PIXEL_FILES （是个 list）
#-- 包含 主进程需要的 cpp 文件
FILE(GLOB PIXEL_FILES  src/*.cpp    
                        src/class/*.cpp   
                        src/sys/*.cpp 
                        src/test/*.cpp 
                        src/gl/*.cpp 
                        src/camera/*.cpp 
                        src/random/*.cpp 
                        src/game_src/*.cpp 
                        src/game/*.cpp 
                        src/game/map/*.cpp 
                        )

#---------------------------------------------------#
#                   build/bin/
#---------------------------------------------------#
#-- 更改 项目最终生成的 可执行二进制文件 的放置目录： build/bin 
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
        #-- 更改输出 会导致 很多 glfw 的测试 也挪到 build/bin/ 目录中
        

#---------------------------------------------------#
#                       doit
#---------------------------------------------------#
add_executable(
    tprpix
    ${PIXEL_FILES}
    ${PROJECT_SOURCE_DIR}/deps/glad/src/glad.c
)


#---------------------------------------------------#
#             子目录 CMakeLists.txt
#---------------------------------------------------#
#-- 子 CMakeLists.txt 执行的 中间产物，将分别放在 
#      build/src  build/libhello  目录中。
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/glfw)

add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprLibs/MsgIO)
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprLibs/wrapFuncs)
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprBitMap)
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprFileSys)
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprCrypto)
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprBinary)
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprDB)
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprTimer)
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprLibs/tprMemAllor)


#---------------------------------------------------#
#             target_link_libraries
#---------------------------------------------------#
if ( APPLE )
    target_link_libraries( tprpix 
        glfw
        ${GLFW_LIBRARIES}
        MsgIO
        wrapFuncs
        tprBitMap
        tprFileSys
        tprCrypto
        tprBinary
        tprDB
        tprTimer
        tprMemAllor
        )

elseif( UNIX AND NOT APPLE )
    target_link_libraries( tprpix 
        glfw
        ${GLFW_LIBRARIES}
        MsgIO
        wrapFuncs
        tprBitMap
        tprFileSys
        tprCrypto
        tprBinary
        tprDB
        tprTimer
        tprMemAllor
        )

endif( APPLE )


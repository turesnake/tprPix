#===================================================#
#           CMakeLists.txt -- 根目录
#===================================================#
#  
#-------------------

#-- cmake 最小 版本号
cmake_minimum_required(VERSION 3.1)



if( APPLE )
    option( TPR_OS_APPLE_ " APPLE " ON )
    message( STATUS "___APPLE___" )

elseif( UNIX AND NOT APPLE )
    option( TPR_OS_UNIX_ " UNIX " ON )
    message( STATUS "___UNIX___" )

#-- 当使用 Virsual Studio 编译 cmake项目时，可用 WIN32
#   如果出错，尝试改用 MSVC
elseif( WIN32 )
    option( TPR_OS_WIN32_ " WIN32 " ON )
    message( STATUS "___WIN32___" )

endif( APPLE )
#-- 此段 务必 在 设置 project 之前


#-- cpp 版本控制。 c++17
#-- CMAKE_CXX_STANDARD 是 cmake 3.1 后的新语句。可放心使用
#-- 当低于此版本时，.h文件中的 inline 关键词将失效
set (CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

#-- 测试用
#set ( CMAKE_CXX_COMPILER            "clang++"  )
#set ( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall" ) 


#-- 单独设置 c++ 的编译选项 的方式:
#add_definitions ( -DXXX_XXX=1 )


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
#                     SySconfig
#---------------------------------------------------#
#-- 配置 一个 h文件，用来 从 cmake 传递一些 变量 到 源文件 中。
#-- 我们需要手动编写 .h.in 文件。
#-- 然后，cmake调用会 帮我们自动生成对应的 h文件。
#-- 最后，正式 make编译时。源文件 就能 include 这些生成的 h文件。
option( SWITCH_1
    " option test: switch 1 " ON )
configure_file(
    "${PROJECT_SOURCE_DIR}/sysconfig/SysConfig.h.in"
    "${PROJECT_BINARY_DIR}/sysconfig/SysConfig.h"
)


#---------------------------------------------------#
#                   include
#---------------------------------------------------#
#-- 指定 依赖的 h文件所在 路径:
include_directories (${PROJECT_SOURCE_DIR}/src)

#-- Config --
include_directories (${PROJECT_SOURCE_DIR}/src/Config)

#-- Engine 中的所有目录 都会被 include 到 --
include_directories (${PROJECT_SOURCE_DIR}/src/Engine)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/actionFSM)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/actionSwitch)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/affect)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/AI)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/animFrameSet)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/camera)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/collision)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/dataBase)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/tprDebug)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/ecoSys)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/game)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/game/gameObj)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/game/map)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/game/map/fieldBorderSet)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/game/map/chunkBuild)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/gl)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/input)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/mesh)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/move)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/multiThread)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/player)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/random)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/resource)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/scene)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/script)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/shaderProgram)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/sys)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/time)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/UI)
include_directories (${PROJECT_SOURCE_DIR}/src/Engine/VAOVBO)


#-- Script 只 include 顶层目录 --
include_directories (${PROJECT_SOURCE_DIR}/src/Script)

include_directories (${PROJECT_SOURCE_DIR}/deps)

include_directories (${PROJECT_SOURCE_DIR}/deps/glad/include)
include_directories (${PROJECT_SOURCE_DIR}/deps/glm.9.9.5)

include_directories (${PROJECT_BINARY_DIR}/sysconfig)


#-- glfw --
#-- 这两个目录中的文件 好像是 一样的 --
if( UNIX )
    include_directories (${PROJECT_SOURCE_DIR}/deps/glfw_unix/include)
else()
    include_directories (${PROJECT_SOURCE_DIR}/deps/glfw_win/include)
endif( UNIX )


#-- 创建变量 PIXEL_FILES （是个 list）
#-- 包含 主进程需要的 cpp 文件
FILE(GLOB PIXEL_FILES   src/*.cpp     
                        src/test/*.cpp

                        src/Config/*.cpp

                        src/Engine/*.cpp
                        src/Engine/actionFSM/*.cpp
                        src/Engine/actionSwitch/*.cpp
                        src/Engine/affect/*.cpp
                        src/Engine/AI/*.cpp
                        src/Engine/animFrameSet/*.cpp 
                        src/Engine/camera/*.cpp 
                        src/Engine/collision/*.cpp 
                        src/Engine/tprDebug/*.cpp 
                        src/Engine/dataBase/*.cpp 
                        src/Engine/ecoSys/*.cpp
                        src/Engine/game/*.cpp 
                        src/Engine/game/gameObj/*.cpp 
                        src/Engine/game/map/*.cpp 
                        src/Engine/game/map/fieldBorderSet/*.cpp 
                        src/Engine/game/map/chunkBuild/*.cpp 
                        src/Engine/gl/*.cpp 
                        src/Engine/input/*.cpp 
                        src/Engine/mesh/*.cpp 
                        src/Engine/move/*.cpp 
                        src/Engine/multiThread/*.cpp 
                        src/Engine/player/*.cpp 
                        src/Engine/random/*.cpp 
                        src/Engine/resource/*.cpp 
                        src/Engine/scene/*.cpp 
                        src/Engine/script/*.cpp 
                        src/Engine/shaderProgram/*.cpp 
                        src/Engine/sys/*.cpp
                        src/Engine/time/*.cpp 
                        src/Engine/UI/*.cpp 
                        src/Engine/VAOVBO/*.cpp 
                        
                        src/Script/*.cpp 
                        src/Script/gameObjs/*.cpp
                        src/Script/gameObjs/bush/*.cpp
                        src/Script/gameObjs/majorGos/*.cpp
                        src/Script/gameObjs/majorGos/trees/*.cpp
                        src/Script/gameObjs/spawners/*.cpp
                        src/Script/gameObjs/mapSurfaces/*.cpp
                        src/Script/resource/*.cpp
                        src/Script/UIs/*.cpp 
                        src/Script/UIs/buttons/*.cpp 
                        )

#---------------------------------------------------#
#                   build/bin/
#---------------------------------------------------#
#-- 更改 项目最终生成的 可执行二进制文件 的放置目录： build/bin 
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
        #-- 更改输出 会导致 很多 glfw 的测试 也挪到 build/bin/ 目录中
        

#---------------------------------------------------#
#                      tprpix 
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
if( UNIX )
    #-- 官网推荐，避免编译 test --
    # 需要在 add_subdirectory 语句之前
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory (${PROJECT_SOURCE_DIR}/deps/glfw_unix)
    add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprInUnix)
else()
    add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprInWin)
endif( UNIX )

add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprGeneral)
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/sqlite.3.28)


#---------------------------------------------------#
#             target_link_libraries
#---------------------------------------------------#
if ( UNIX )
    target_link_libraries( tprpix 
        glfw
        ${GLFW_LIBRARIES}
        tprGeneral
        tprInUnix
        SQLite
        )
else()
    target_link_libraries( tprpix 
        ${PROJECT_SOURCE_DIR}/deps/glfw_win/lib-vc2017/glfw3.lib
        tprGeneral
        tprInWin
        SQLite
        )

endif( UNIX )




target_compile_options( tprpix PRIVATE -Wall )





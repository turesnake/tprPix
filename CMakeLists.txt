#===================================================#
#           CMakeLists.txt -- 根目录
#===================================================#

#---- 推荐选项 ----
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# make 


#---------------------------------------------------#
#                cmake version
#---------------------------------------------------#
cmake_minimum_required(VERSION 3.1)
message( STATUS "cmake: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" )


#---------------------------------------------------#
#                    OS
#---------------------------------------------------#
if( APPLE )
    option( TPR_OS_MACOSX_ " MACOSX " ON )
    option( TPR_OS_UNIX_ " UNIX " ON )
    message( STATUS "___APPLE___" )

elseif( UNIX AND NOT APPLE )
    option( TPR_OS_LINUX_ " LINUX " ON )
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


#-- 显示编译时使用的 编译选项
set ( CMAKE_VERBOSE_MAKEFILE ON )


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
#                     src
#---------------------------------------------------#
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
)

#---------------------------------------------------#
#                     C#
#---------------------------------------------------#
# 调用 shell 文件，会在 make 阶段

if( APPLE )
    add_custom_target( csDll ALL
        COMMAND /bin/sh ${PROJECT_SOURCE_DIR}/shells/build_osx.sh
        )

elseif( WIN32 )
    add_custom_target( csDll ALL
        COMMAND cmd ${PROJECT_SOURCE_DIR}/shells/build_win.bat
    )

endif( APPLE )


#---------------------------------------------------#
#                   include
#---------------------------------------------------#
target_include_directories ( tprpix PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/Config

    ${PROJECT_SOURCE_DIR}/src/Engine
    ${PROJECT_SOURCE_DIR}/src/Engine/actionFSM
    ${PROJECT_SOURCE_DIR}/src/Engine/actionSwitch
    ${PROJECT_SOURCE_DIR}/src/Engine/affect
    ${PROJECT_SOURCE_DIR}/src/Engine/AI
    ${PROJECT_SOURCE_DIR}/src/Engine/animFrameSet
    ${PROJECT_SOURCE_DIR}/src/Engine/camera
    ${PROJECT_SOURCE_DIR}/src/Engine/collision
    ${PROJECT_SOURCE_DIR}/src/Engine/dataBase
    ${PROJECT_SOURCE_DIR}/src/Engine/tprDebug
    ${PROJECT_SOURCE_DIR}/src/Engine/ecoSys
    ${PROJECT_SOURCE_DIR}/src/Engine/game
    ${PROJECT_SOURCE_DIR}/src/Engine/game/gameObj
    ${PROJECT_SOURCE_DIR}/src/Engine/game/map
    ${PROJECT_SOURCE_DIR}/src/Engine/game/map/fieldBorderSet
    ${PROJECT_SOURCE_DIR}/src/Engine/game/map/chunkBuild
    ${PROJECT_SOURCE_DIR}/src/Engine/gl
    ${PROJECT_SOURCE_DIR}/src/Engine/input
    ${PROJECT_SOURCE_DIR}/src/Engine/mesh
    ${PROJECT_SOURCE_DIR}/src/Engine/move
    ${PROJECT_SOURCE_DIR}/src/Engine/multiThread
    ${PROJECT_SOURCE_DIR}/src/Engine/player
    ${PROJECT_SOURCE_DIR}/src/Engine/random
    ${PROJECT_SOURCE_DIR}/src/Engine/resource
    ${PROJECT_SOURCE_DIR}/src/Engine/scene
    ${PROJECT_SOURCE_DIR}/src/Engine/script
    ${PROJECT_SOURCE_DIR}/src/Engine/shaderProgram
    ${PROJECT_SOURCE_DIR}/src/Engine/sys
    ${PROJECT_SOURCE_DIR}/src/Engine/time
    ${PROJECT_SOURCE_DIR}/src/Engine/UI
    ${PROJECT_SOURCE_DIR}/src/Engine/VAOVBO

    ${PROJECT_SOURCE_DIR}/src/Script

    ${PROJECT_BINARY_DIR}/sysconfig
    )

#-- 防止 第三方库h文件 报 warnings --
target_include_directories ( tprpix SYSTEM PUBLIC
    ${PROJECT_SOURCE_DIR}/deps
    ${PROJECT_SOURCE_DIR}/deps/stb_image
    ${PROJECT_SOURCE_DIR}/deps/glm.9.9.5
    )



#-- glfw --
#-- 这两个目录中的文件 好像是 一样的 --
if( UNIX )
    target_include_directories ( tprpix PUBLIC ${PROJECT_SOURCE_DIR}/deps/glfw_unix/include)
else()
    target_include_directories ( tprpix PUBLIC ${PROJECT_SOURCE_DIR}/deps/glfw_win/include)
endif( UNIX )



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

add_subdirectory (${PROJECT_SOURCE_DIR}/deps/glad)
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/tprGeneral)
add_subdirectory (${PROJECT_SOURCE_DIR}/deps/sqlite.3.28)


#---------------------------------------------------#
#             target_link_libraries
#---------------------------------------------------#
if ( UNIX )
    target_link_libraries( tprpix  
        tprGeneral
        tprInUnix
        glad
        glfw
        ${GLFW_LIBRARIES}
        SQLite
        )

else()
    target_link_libraries( tprpix 
        tprGeneral
        tprInWin
        glad
        ${PROJECT_SOURCE_DIR}/deps/glfw_win/lib-vc2017/glfw3.lib
        SQLite
        )
endif( UNIX )


target_compile_options( tprpix 
                        PRIVATE 
                        -Wall -Wextra
                        -Wno-c++98-compat
                        -Wno-c++98-compat-pedantic
                        -Wno-language-extension-token      #- glad
						-Wno-documentation                 #- glfw3
						-Wno-documentation-unknown-command #- glfw3
						-Wno-nonportable-system-include-path
						-Wno-reserved-id-macro
						-Wno-global-constructors            #++ need ++
                        -Wno-exit-time-destructors          #++ need ++
                        -Wno-covered-switch-default         #++ need ++
                        -Wno-switch-enum                    #++ need ++
                        -Wno-unused-parameter               #++ need ++
                        -Wno-unused-member-function         #++ need ++
                        -Wno-missing-variable-declarations  #++ need ++
                        -Wno-missing-prototypes
                        -Wno-old-style-cast   #---- tmp ----                        
                        )


#=========== 强行关闭所有 优化 ============
#set ( TPR_LINK_FLAGS "-rdynamic" )

#-- 这组属性确实在 win 中起了作用，建议保留 
#-- 可以使用 dumpbin /exports xxx.dll 查看 win 中 dll 符号表
#set( CMAKE_SHARED_LINKER_FLAGS                "${TPR_LINK_FLAGS}" )
#set( CMAKE_SHARED_LINKER_FLAGS_DEBUG          "${TPR_LINK_FLAGS}" )
#set( CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL     "${TPR_LINK_FLAGS}" )
#set( CMAKE_SHARED_LINKER_FLAGS_RELEASE        "${TPR_LINK_FLAGS}" )
#set( CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${TPR_LINK_FLAGS}" )


#-------- UNIX ----------
if( UNIX )
    set ( CMAKE_C_FLAGS                  "-O0"              CACHE STRING "regular mode: no optimize" FORCE )
    set ( CMAKE_C_FLAGS_DEBUG            "-O0 -g"           CACHE STRING "debug mode: no optimize" FORCE )
    set ( CMAKE_C_FLAGS_MINSIZEREL       "-O0 -DNDEBUG"     CACHE STRING "minSizeRel mode: no optimize" FORCE )
    set ( CMAKE_C_FLAGS_RELEASE          "-O0 -DNDEBUG"     CACHE STRING "release mode: no optimize" FORCE )
    set ( CMAKE_C_FLAGS_RELWITHDEBINFO   "-O0 -g -DNDEBUG"  CACHE STRING "relWithDebugInfo mode: no optimize" FORCE )

    set ( CMAKE_CXX_FLAGS                 "-O0"             CACHE STRING "regular mode: no optimize" FORCE )
    set ( CMAKE_CXX_FLAGS_DEBUG           "-O0 -g"          CACHE STRING "debug mode: no optimize" FORCE )
    set ( CMAKE_CXX_FLAGS_MINSIZEREL      "-O0 -DNDEBUG"    CACHE STRING "minSizeRel mode: no optimize" FORCE )
    set ( CMAKE_CXX_FLAGS_RELEASE         "-O0 -DNDEBUG"    CACHE STRING "release mode: no optimize" FORCE )
    set ( CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O0 -g -DNDEBUG" CACHE STRING "relWithDebugInfo mode: no optimize" FORCE )


#-------- WIN ----------
else()
    # do not have CMAKE_C_FLAGS;
    set ( CMAKE_C_FLAGS_DEBUG            "/MDd /Zi /Ob0 /Od /RTC1"   CACHE STRING "debug mode: no optimize" FORCE )
    set ( CMAKE_C_FLAGS_MINSIZEREL       "/MD /O0 /Ob1 /DNDEBUG"     CACHE STRING "minSizeRel mode: no optimize" FORCE )
    set ( CMAKE_C_FLAGS_RELEASE          "/MD /O0 /Ob1 /DNDEBUG"     CACHE STRING "release mode: no optimize" FORCE )
    set ( CMAKE_C_FLAGS_RELWITHDEBINFO   "/MD /Zi /O0 /Ob1 /DNDEBUG" CACHE STRING "relWithDebugInfo mode: no optimize" FORCE )

    # do not have CMAKE_CXX_FLAGS;
    set ( CMAKE_CXX_FLAGS_DEBUG            "/MDd /Zi /Ob0 /Od /RTC1"   CACHE STRING "debug mode: no optimize" FORCE )
    set ( CMAKE_CXX_FLAGS_MINSIZEREL       "/MD /O0 /Ob1 /DNDEBUG"     CACHE STRING "minSizeRel mode: no optimize" FORCE )
    set ( CMAKE_CXX_FLAGS_RELEASE          "/MD /O0 /Ob1 /DNDEBUG"     CACHE STRING "release mode: no optimize" FORCE )
    set ( CMAKE_CXX_FLAGS_RELWITHDEBINFO   "/MD /Zi /O0 /Ob1 /DNDEBUG" CACHE STRING "relWithDebugInfo mode: no optimize" FORCE )
endif( UNIX )





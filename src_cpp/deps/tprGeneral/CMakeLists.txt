#===================================================#
#           CMakeLists.txt -- <根目录>/deps/tprGeneral/
#===================================================#
#-- 将 tprGeneral 包装成 静态库。 进一步 模块化
#   win/unix 通用



#-- 定义一个列表，放置所有 cpp 文件 --
FILE(GLOB TPR_IN_GENERAL_SRC  *.cpp )

#-- 建立 目标 库对象 -- 
#-- 在 主CMakeLists.txt 文件中，以此名来调用 本库
add_library ( tprGeneral 
            STATIC
            ${TPR_IN_GENERAL_SRC} )


#-- 指定 依赖的 h文件所在 路径:
#   PRIVATE - 自身编译include
#   SYSTEM INTERFACE - 提供给全局的 include（调用者不用再指定 include 了）
target_include_directories ( tprGeneral
                            PRIVATE ./
                            SYSTEM INTERFACE  ./
                            )         

target_compile_options ( tprGeneral 
                        PRIVATE 
                        -Wall 
                        -Wno-c++98-compat
						-Wno-c++98-compat-pedantic
                        )


#-- 更改 库 最终生成的 可执行二进制文件 的放置目录：build/lib
#-- 这个设置 意味着，每次 cmake/make, 本库都是需要重新编译的。
#-- 可以在后期，版本稳定后，改为 库内目录 
#-- 如果不定义此句，本库的 生成文件将放在 build/deps/frapfunc 中
#-- 也许更适合
#set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


#-- 将 库 的输出名字改为 tprGeneral.lib
set_target_properties (tprGeneral PROPERTIES PREFIX "")
set_target_properties (tprGeneral PROPERTIES OUTPUT_NAME "tprGeneral" )


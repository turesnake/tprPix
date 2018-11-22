/*
 * ========================= global.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   全局变量， 
 * 
 *   使用 cpp 特有的 inline 关键词来 一步到位地 声明+初始化
 * 
 * ----------------------------
 */
#ifndef _TPR_GLOBAL_H_
#define _TPR_GLOBAL_H_

//-------------------- C ----------------------//
#include <unistd.h> //- pid_t fork
#include <sys/stat.h> //- st_mode 

//-------------------- CPP --------------------//
#include <string>


inline bool is_fst_run {}; //-- 本次运行，是否为 本进程编译后的 首次运行
                            //-- 每次运行时，由函数 check_fst_run() 设置
                            //-- 若为 true，  是 首次运行
                            //-- 若为 false， 不是 首次运行

//----------------------------------------------------//
//                     file
//----------------------------------------------------//
inline int         fd_cwd {};   //-- 项目 主目录 fd
inline std::string path_cwd {}; //-- 项目 主目录 路径名
                
inline std::string path_home {};  //-- ~/
inline std::string path_data {};  //-- .../data/



//----------------------------------------------------//
//                       OS
//----------------------------------------------------//
//-- 当前身处什么 操作系统 --//
#define OS_NULL  0
#define OS_APPLE 1
#define OS_UNIX  2
#define OS_WIN32 3
inline int current_OS {};


#endif

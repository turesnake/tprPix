/*
 * ========================= global.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.08.23
 *                                        修改 -- 2018.08.23
 * ----------------------------------------------------------
 *   全局变量， 
 *   server / client 通用
 * 
 *   使用 cpp 特有的 inline 关键词来 一步到位地 声明+初始化
 * 
 * ----------------------------
 */

#ifndef _TPR_GLOBAL_H_
#define _TPR_GLOBAL_H_

#include <unistd.h> //- pid_t fork
#include <sys/stat.h> //- st_mode 

#include <string>


inline bool is_fst_run{}; //-- 本次运行，是否为 本进程编译后的 首次运行
                            //-- 每次运行时，由函数 check_fst_run() 设置
                            //-- 若为 true，  是 首次运行
                            //-- 若为 false， 不是 首次运行

inline int         fd_cwd{};   //-- 项目 主目录 fd
inline std::string path_cwd{}; //-- 项目 主目录 路径名
                
inline std::string path_home{};  //-- ~/
inline std::string path_data{};  //-- .../data/


inline std::string path_downloads{}; //- ~/Downloads/tprnet/ 目录的路径名
                            //- server进程 在这个目录下 存放 
                            //- 从 tcp 接收的 文件
                            //- 通常不直接用此 path，而用下方的 fd_downloads

inline int fd_downloads{}; //-- server 专用，在整个 server 程序期间，始终存在
                            //- openat,配合此 fd，可以在 ~/Downloads/tprnet/
                            //- 目录中，存储文件（从 tcp 传来的文件）
                            //- 暂设为全局版本，意味着， client 其实也有这个 fd，只是不用它。


//-- 当前身处什么 操作系统 --//
#define OS_NULL  0
#define OS_APPLE 1
#define OS_UNIX  2
#define OS_WIN32 3
inline int current_OS{};




#endif

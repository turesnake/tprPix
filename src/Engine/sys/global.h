/*
 * ========================= global.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   全局变量
 *   使用 cpp 特有的 inline 关键词来 一步到位地 声明+初始化
 * ----------------------------
 */
#ifndef TPR_GLOBAL_H
#define TPR_GLOBAL_H

//-------------------- CPP --------------------//
#include <string>

//inline bool is_fst_run {}; //-- 本次运行，是否为 本进程编译后的 首次运行
                            //-- 每次运行时，由函数 check_fst_run() 设置
                            //-- 若为 true，  是 首次运行
                            //-- 若为 false， 不是 首次运行

//----------------------------------------------------//
//                     file
//----------------------------------------------------//
extern int         fd_cwd;   //-- 项目 主目录 fd

extern std::string path_cwd; //-- exe 所在目录的 path
                
extern std::string path_dataBase;  //-- .../build/publish/dataBase/
extern std::string path_shaders; //-- .../build/publish/shaders/
//extern std::string path_textures; //-- .../build/publish/textures/ // 暂未被使用 ...
extern std::string path_jsons; //-- .../build/publish/jsons/
extern std::string path_tprLog; //-- .../build/publish/tprLog/

extern std::string path_blueprintDatas; //-- .../build/publish/blueprintDatas/

extern std::string path_gameObjDatas; //-- .../build/publish/gameObjDatas/

//----------------------------------------------------//
//                       OS
//----------------------------------------------------//
//-- 当前身处什么 操作系统 --//
#define OS_NULL  0
#define OS_APPLE 1
#define OS_UNIX  2
#define OS_WIN32 3
extern int current_OS;


#endif

/*
 * ========================= globfunc.h ==========================
 *                            -- tpr --
 *                                        创建 -- 2018.08.23
 *                                        修改 -- 2018.08.23
 * ----------------------------------------------------------
 *   全局函数
 *   server / client 通用
 * ----------------------------
 */

#ifndef _TPR_FLOBFUNC_H_
#define _TPR_FLOBFUNC_H_

#include <fstream>   //-- ifstream,ofstream


#include "MsgIO.h"


//====== 宏函数 =========
#define name2str(name) (#name) //-- 输入变量名，获得对应的 c风格字符串


//----- prepare.cpp -----
void prepare();


//------ oth.cpp --------
const std::string pid2string( pid_t _pid );
const std::string fifo_path_with_pid( const std::string _head,
                                    pid_t _pid );
const std::string get_exit_status_info( int _status );

int sleep_tpr( long _msec );
void Sleep_tpr( long _msec, const std::string &_err_info );



#endif



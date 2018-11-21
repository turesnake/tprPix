/*
 * ========================= oth.cpp ==========================
 *                         -- tpr --
 *                                        创建 -- 2018.09.21
 *                                        修改 -- 2018.09.21
 * ----------------------------------------------------------
 *    专门存放 无法归类的 零碎的 函数
 * ----------------------------
 */

//-------------------- C ----------------------//
#include <unistd.h> //- pid_t
#include <sys/wait.h> //- wait, status

#include <sys/select.h>
#include <sys/time.h>  //- select 
#include <errno.h> //- errno
#include <string.h> //- strerror

//-------------------- CPP --------------------//
#include <string>
#include <iostream> //-- cout
#include <sstream>  //-- stringstream

//------------------- SELF --------------------//
#include "wrapFuncs.h"


using std::cout;
using std::endl;
using std::string;
using std::stringstream;


//------------------- 提供给外部的 函数 ----------------
const std::string pid2string( pid_t _pid );
const std::string fifo_path_with_pid( const std::string _head,
                                    pid_t _pid );
const std::string get_exit_status_info( int _status );

int sleep_tpr( long _msec );
void Sleep_tpr( long _msec, const std::string &_err_info );

/* ===========================================================
 *                      pid2string 
 * -----------------------------------------------------------
 */
const std::string pid2string( pid_t _pid ){
    stringstream ss;
    string spid; //-- 转换成 string 类型的 pid_self

    ss << _pid;
    ss >> spid;

    return spid;
}


/* ===========================================================
 *                    fifo_path_with_pid 
 * -----------------------------------------------------------
 * -- 一步到位地 合成 fifo path (带 pid 的)
 */
const std::string fifo_path_with_pid( const std::string _head,
                                    pid_t _pid ){
    stringstream ss;
    string spid; //-- 转换成 string 类型的 pid_self

    ss << _pid;
    ss >> spid;

    return (_head + spid);
}


/* ===========================================================
 *                    check_exit
 * -----------------------------------------------------------
 * -- 检查 exit 返回的 status，获得具体信息
 */
const std::string get_exit_status_info( int _status ){

    string r; //--  返回值
    string tmp; //-- 将 整形 变为 string 的中转容器
    stringstream ss;

    //-- 若是 正常 exit --//
    if( WIFEXITED(_status) ){
        r = "normal exit. status = ";
        ss << WEXITSTATUS(_status);
    }
    //-- 若是 信号导致的 异常终止 --//
    else if( WIFSIGNALED(_status) ){
        r = "abnormal exit. signal = ";
        ss << WTERMSIG(_status);

    }
    //-- 若是 作业系统导致的 进程暂停 返回的 状态 --//
    else if( WIFSTOPPED(_status) ){
        r = "child stopped, signal = ";
        ss << WSTOPSIG(_status);
    }

    ss >> tmp;

    return (r + tmp);
}

/* ===========================================================
 *                    sleep_tpr
 * -----------------------------------------------------------
 * -- 通过 select 实现的 毫秒级 定时器。
 * -- 返回值 直接就是 select 的返回值:
 *    若 返回 0，说明时 select 超时引发的，此为正常情况
 *    其他返回值 都为 异常状态。
 */
int sleep_tpr( long _msec ){

    //-- 将时间制作成 timeval 格式 --
    timeval tm;
    tm.tv_sec  = _msec / 1000;
    tm.tv_usec = (_msec % 1000) * 1000;

    //--
    return select( 0, NULL, NULL, NULL, &tm );
}


/* ===========================================================
 *                    Sleep_tpr
 * -----------------------------------------------------------
 * -- sleep_tpr 的包裹函数
 * -- 目前版本中，当 select 出错，并没有异常终止，而是报错后 返回主流程。
 */
void Sleep_tpr( long _msec, const std::string &_err_info ){

    int r;
    r = sleep_tpr( _msec );
    if( r < 0 ){
        cout << "Sleep_tpr: ERROR. return = " << r << ". "
            << "errno = " << errno 
            << ": " << strerror(errno) << ". "
            << _err_info  
            << endl;
        
        //-- 并没有调用 assert，而是选择正常返回
        cout << "but go on working..." << endl;
    }

}






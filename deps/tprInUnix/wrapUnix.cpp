/*
 * ========================= wrapUnix.cpp ==========================
 *                             -- tpr --
 *                                        CREATE -- 2018.09.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *     unix 函数系列的 包裹函数 
 * ----------------------------
 */

//-------------------- C ----------------------//
#include <cerrno> //- errno.h
#include <cstring> //- strerror
#include <fcntl.h>  //- open, mode_t
#include <unistd.h> //- close，pid_t fork, lseek
#include <sys/wait.h> //- wait, waitpid
#include <sys/stat.h>  //- mkdir
#include <cstdlib>  //- getenv

#include <sys/select.h>
#include <sys/time.h> //- select

#include <cassert> //- assert
//-------------------- CPP --------------------//
#include <string>
#include <iostream>

using std::cout;
using std::endl;

namespace tprUnix {//--------------- namespace: tprUnix -------------------//


//---------------------- 提供给外部的 函数 --------------------
int Open( const char *_path, int _oflag, mode_t _mode, const std::string &_err_info );

void Mkdir( const char *_path, mode_t _mode, const std::string &_err_info );
void Mkdirat( int _fd, const char *_path, mode_t _mode, const std::string &_err_info );

void Close( int _fd, const std::string &_err_info );
void Unlink( const char *_pathname, const std::string &_err_info );

off_t Lseek( int _fd, off_t _offset, int _whence, const std::string &_err_info);
void Fseeko( FILE *_fp, off_t _offset, int _whence, const std::string &_err_info);
off_t Ftello( FILE *_fp, const std::string &_err_info );

/*
pid_t Fork( const std::string &_err_info );
void Wait( int *_status, pid_t _pid_target, const std::string &_err_info );
int Select( int _maxfdp1, fd_set *_readset, fd_set *_writeset, fd_set *_exceptset,
            struct timeval *_timeout, const std::string &_err_info );
*/
ssize_t Read( int _fd, void *_buf, size_t _nbytes, const std::string &_err_info );
void Write( int _fd, const void *_buf, size_t _nbytes, const std::string &_err_info );
//std::string Getenv( const char *_name, const std::string &_err_info );

//===========================================================
//                         Open
//-----------------------------------------------------------
int Open( const char *_path, int _oflag, mode_t _mode, const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    int fd;
    fd = open(_path, _oflag, _mode);
    if( fd == -1 ){
        cout << "Open: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
    return fd;
}


//===========================================================
//                      Mkdir    
//-----------------------------------------------------------
void Mkdir( const char *_path, mode_t _mode, const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    if( mkdir(_path, _mode) == -1 ){
        cout << "Mkdir: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
}

//===========================================================
//                      Mkdirat    
//-----------------------------------------------------------
void Mkdirat( int _fd, const char *_path, mode_t _mode, const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    if( mkdirat( _fd, _path, _mode) == -1 ){
        cout << "Mkdirat: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
}


//===========================================================
//                      Close
//-----------------------------------------------------------
void Close( int _fd, const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    if( close(_fd) == -1 ){
        cout << "Close: ERROR. return -1."
            << "fd = " << _fd << "\n"
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
}


//===========================================================
//                      Unlink
//-----------------------------------------------------------
void Unlink( const char *_pathname, const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    if( unlink(_pathname) < 0 ){
        cout << "Unlink: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
}

//===========================================================
//                      Lseek
//-----------------------------------------------------------
//-- off_t 是 带符号的 64-bits 整形。可以放心容纳一切大小的文件。
off_t Lseek( int _fd, off_t _offset, int _whence, const std::string &_err_info){

    errno = 0; //-- 清空 errno
    off_t r;
    if( (r=lseek(_fd, _offset, _whence)) == (off_t)(-1) ){
                        //-- 有些 非常规文件 的 当前偏移量 可能是 负数。
                        //-- 所以，检测出错是，不要使用 " < 0"  这种写法
        cout << "Lseek: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
    return r;
}

//===========================================================
//                      Fseeko
//-----------------------------------------------------------
//-- off_t 是 带符号的 8-bytes 整形。可以放心容纳一切大小的文件。
void Fseeko( FILE *_fp, off_t _offset, int _whence, const std::string &_err_info){

    errno = 0; //-- 清空 errno
    if( fseeko(_fp, _offset, _whence) == -1 ){
        cout << "Fseeko: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
}


//===========================================================
//                      Ftello
//-----------------------------------------------------------
//-- off_t 是 带符号的 8-bytes 整形。可以放心容纳一切大小的文件。
/*
off_t Ftello( FILE *_fp, const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    off_t r;
    if( (r=ftello(_fp)) == (off_t)(-1) ){
        cout << "Ftello: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
    return r;
}
*/




//===========================================================
//                        Fork
//-----------------------------------------------------------
/*
pid_t Fork( const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    pid_t p = fork();
    if( p < 0 ){
        cout << "Fork: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
    return p;
}
*/


//===========================================================
//                        Wait
//-----------------------------------------------------------
/*
void Wait( int *_status, pid_t _pid_target, const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    if( wait(_status) != _pid_target ){
        cout << "Wait: ERROR. return != " << _pid_target << ". "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
}
*/


//===========================================================
//                         Select
//-----------------------------------------------------------
/*
int Select( int _maxfdp1, fd_set *_readset, fd_set *_writeset, fd_set *_exceptset,
            struct timeval *_timeout, const std::string &_err_info ){
    
    errno = 0; //-- 清空 errno
    int r;
    r = select( _maxfdp1, _readset, _writeset, _exceptset, _timeout );

    if( r < 0 ){
        cout << "Select: ERROR. return < 0. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
    return (r); 
}
*/


//===========================================================
//                        Read
//-----------------------------------------------------------
ssize_t Read( int _fd, void *_buf, size_t _nbytes, const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    ssize_t len;
    len = read( _fd, _buf, _nbytes );
    if( len == -1 ){
        cout << "Read: ERROR. return -1. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
    return (len);
}


//===========================================================
//                        Write
//-----------------------------------------------------------
//-- 很严格，只要 不成功写入 目标字节数，都算错
void Write( int _fd, const void *_buf, size_t _nbytes, const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    ssize_t len;
    len = write( _fd, _buf, _nbytes );
    if( (size_t)len != _nbytes ){
        cout << "Write: ERROR. return != nbytes. \n"
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
}

//===========================================================
//                        Getenv
//-----------------------------------------------------------
std::string Getenv( const char *_name, const std::string &_err_info ){
    
    errno = 0; //-- 清空 errno
    char *r;
    r = getenv(_name);
    if( r == NULL ){
        cout << "Getenv: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }

    std::string rstr = r; 
    return rstr; //-- 直接返回一个 string，会让调用者 更方便些
}





}//------------------- namespace: tprUnix ------------------------//


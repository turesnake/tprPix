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
int Open( const char *path_, int oflag_, mode_t mode_, const std::string &err_info_ );

void Mkdir( const char *path_, mode_t mode_, const std::string &err_info_ );
void Mkdirat( int fd_, const char *path_, mode_t mode_, const std::string &err_info_ );

void Close( int fd_, const std::string &err_info_ );
void Unlink( const char *pathname_, const std::string &err_info_ );

off_t Lseek( int fd_, off_t offset_, int whence_, const std::string &err_info_);
void Fseeko( FILE *_fp, off_t offset_, int whence_, const std::string &err_info_);
off_t Ftello( FILE *_fp, const std::string &err_info_ );

/*
pid_t Fork( const std::string &err_info_ );
void Wait( int *_status, pid_t _pid_target, const std::string &err_info_ );
int Select( int _maxfdp1, fd_set *_readset, fd_set *_writeset, fd_set *_exceptset,
            struct timeval *_timeout, const std::string &err_info_ );
*/
ssize_t Read( int fd_, void *buf_, size_t nbytes_, const std::string &err_info_ );
void Write( int fd_, const void *buf_, size_t nbytes_, const std::string &err_info_ );
//std::string Getenv( const char *name_, const std::string &err_info_ );

//===========================================================
//                         Open
//-----------------------------------------------------------
int Open( const char *path_, int oflag_, mode_t mode_, const std::string &err_info_ ){

    errno = 0; //-- 清空 errno
    int fd;
    fd = open(path_, oflag_, mode_);
    if( fd == -1 ){
        cout << "Open: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
            << endl;
        assert(0);
    }
    return fd;
}


//===========================================================
//                      Mkdir    
//-----------------------------------------------------------
void Mkdir( const char *path_, mode_t mode_, const std::string &err_info_ ){

    errno = 0; //-- 清空 errno
    if( mkdir(path_, mode_) == -1 ){
        cout << "Mkdir: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
            << endl;
        assert(0);
    }
}

//===========================================================
//                      Mkdirat    
//-----------------------------------------------------------
void Mkdirat( int fd_, const char *path_, mode_t mode_, const std::string &err_info_ ){

    errno = 0; //-- 清空 errno
    if( mkdirat( fd_, path_, mode_) == -1 ){
        cout << "Mkdirat: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
            << endl;
        assert(0);
    }
}


//===========================================================
//                      Close
//-----------------------------------------------------------
void Close( int fd_, const std::string &err_info_ ){

    errno = 0; //-- 清空 errno
    if( close(fd_) == -1 ){
        cout << "Close: ERROR. return -1."
            << "fd = " << fd_ << "\n"
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
            << endl;
        assert(0);
    }
}


//===========================================================
//                      Unlink
//-----------------------------------------------------------
void Unlink( const char *pathname_, const std::string &err_info_ ){

    errno = 0; //-- 清空 errno
    if( unlink(pathname_) < 0 ){
        cout << "Unlink: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
            << endl;
        assert(0);
    }
}

//===========================================================
//                      Lseek
//-----------------------------------------------------------
//-- off_t 是 带符号的 64-bits 整形。可以放心容纳一切大小的文件。
off_t Lseek( int fd_, off_t offset_, int whence_, const std::string &err_info_){

    errno = 0; //-- 清空 errno
    off_t r;
    if( (r=lseek(fd_, offset_, whence_)) == (off_t)(-1) ){
                        //-- 有些 非常规文件 的 当前偏移量 可能是 负数。
                        //-- 所以，检测出错是，不要使用 " < 0"  这种写法
        cout << "Lseek: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
            << endl;
        assert(0);
    }
    return r;
}

//===========================================================
//                      Fseeko
//-----------------------------------------------------------
//-- off_t 是 带符号的 8-bytes 整形。可以放心容纳一切大小的文件。
void Fseeko( FILE *_fp, off_t offset_, int whence_, const std::string &err_info_){

    errno = 0; //-- 清空 errno
    if( fseeko(_fp, offset_, whence_) == -1 ){
        cout << "Fseeko: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
            << endl;
        assert(0);
    }
}


//===========================================================
//                      Ftello
//-----------------------------------------------------------
//-- off_t 是 带符号的 8-bytes 整形。可以放心容纳一切大小的文件。
/*
off_t Ftello( FILE *_fp, const std::string &err_info_ ){

    errno = 0; //-- 清空 errno
    off_t r;
    if( (r=ftello(_fp)) == (off_t)(-1) ){
        cout << "Ftello: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
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
pid_t Fork( const std::string &err_info_ ){

    errno = 0; //-- 清空 errno
    pid_t p = fork();
    if( p < 0 ){
        cout << "Fork: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
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
void Wait( int *_status, pid_t _pid_target, const std::string &err_info_ ){

    errno = 0; //-- 清空 errno
    if( wait(_status) != _pid_target ){
        cout << "Wait: ERROR. return != " << _pid_target << ". "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
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
            struct timeval *_timeout, const std::string &err_info_ ){
    
    errno = 0; //-- 清空 errno
    int r;
    r = select( _maxfdp1, _readset, _writeset, _exceptset, _timeout );

    if( r < 0 ){
        cout << "Select: ERROR. return < 0. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
            << endl;
        assert(0);
    }
    return (r); 
}
*/


//===========================================================
//                        Read
//-----------------------------------------------------------
ssize_t Read( int fd_, void *buf_, size_t nbytes_, const std::string &err_info_ ){

    errno = 0; //-- 清空 errno
    ssize_t len;
    len = read( fd_, buf_, nbytes_ );
    if( len == -1 ){
        cout << "Read: ERROR. return -1. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
            << endl;
        assert(0);
    }
    return (len);
}


//===========================================================
//                        Write
//-----------------------------------------------------------
//-- 很严格，只要 不成功写入 目标字节数，都算错
void Write( int fd_, const void *buf_, size_t nbytes_, const std::string &err_info_ ){

    errno = 0; //-- 清空 errno
    ssize_t len;
    len = write( fd_, buf_, nbytes_ );
    if( (size_t)len != nbytes_ ){
        cout << "Write: ERROR. return != nbytes. \n"
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
            << endl;
        assert(0);
    }
}

//===========================================================
//                        Getenv
//-----------------------------------------------------------
std::string Getenv( const char *name_, const std::string &err_info_ ){
    
    errno = 0; //-- 清空 errno
    char *r;
    r = getenv(name_);
    if( r == NULL ){
        cout << "Getenv: ERROR. return -1."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << err_info_
            << endl;
        assert(0);
    }

    std::string rstr = r; 
    return rstr; //-- 直接返回一个 string，会让调用者 更方便些
}





}//------------------- namespace: tprUnix ------------------------//


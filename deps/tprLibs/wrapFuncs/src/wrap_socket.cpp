/*
 * ========================= wrap_socket.cpp ==========================
 *                              -- tpr --
 *                                        创建 -- 2018.09.11
 *                                        修改 -- 2018.09.11
 * ----------------------------------------------------------
 *     socket 函数系列的 包裹函数 
 *     cpp 报错版
 * ----------------------------
 */

//-------------------- C ----------------------//
#include <sys/socket.h> //- socket
                        //- bind
                        //- accept
                        //- connect
                        //- listen
                        //- shutdown

#include <arpa/inet.h>  //- inet_ntop
                        //- inet_pton


#include <errno.h>  //- errno
#include <signal.h>

#include <errno.h> //- errno.h
#include <string.h> //- strerror

#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <iostream>
#include <string>


using std::string;
using std::cout;
using std::endl;


namespace tpr { //--------------- namespace: tpr -------------------//


//---------------------- 提供给外部的 函数 --------------------
int Socket( int _family, int _type, int _protocol, const std::string &_err_info );
int Accept( int _sockfd, struct sockaddr *_cliaddr, socklen_t *_addrlen, 
            const std::string &_err_info );
void Bind( int _sockfd, const struct sockaddr *_addr, socklen_t _len, 
            const std::string &_err_info );
void Connect( int _sockfd, const struct sockaddr *_servaddr, socklen_t _addrlen, 
            const std::string &_err_info );
void Listen( int _sockfd, int _backlog, const std::string &_err_info );
const char* Inet_ntop( int _family, 
                        const void* _addrptr, 
                        char* _strptr, 
                        socklen_t _size,
                        const std::string &_err_info );
void Inet_pton( int _family, const char* _strptr, void* _addrptr, 
                const std::string &_err_info );
void Shutdown( int _sockfd, int _howto, const std::string &_err_info );


//===========================================================
//                      Socket
//-----------------------------------------------------------
int Socket( int _family, int _type, int _protocol, 
            const std::string &_err_info ){
    
    errno = 0; //-- 清空 errno
    int n;
    if( (n = socket(_family, _type, _protocol)) < 0 ){
        cout << "Socket: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
    return( n );
}


//===========================================================
//                      Accept
//-----------------------------------------------------------
//-- 此 wrap函数 有点 复杂：

//-- 首先，accept出错 有可能是 因为 网络问题，此时有必要检查 errno 的值。
//   如果 errno 值为 EPROTO 或 ECONNABORTED。说明是 网路问题。
//   此时 使用 goto 语句来 重新调用 accept。
//   如果并不是 网络问题，则可以走 常规 err_sys 路线 报错并退出程序。

//-- 此外，并不是所有系统的 errno 常量都含有 EPROTO.
//   此处使用一个 宏判断来 处理。
//   在 linux 中， EPROTO， ECONNABORTED 两个常量都存在。
int Accept( int _sockfd, struct sockaddr *_cliaddr, socklen_t *_addrlen,
            const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    int n;

again:
    if( (n = accept( _sockfd, _cliaddr, _addrlen )) < 0 ){
#ifdef  EPROTO
        if( errno==EPROTO || errno==ECONNABORTED )
#else
        if(errno==ECONNABORTED)
#endif 
        { 
            goto again;
        } else {
            cout << "Accept: ERROR. "
                << "errno = " << errno << " " << strerror(errno) << "\n"
                << _err_info
                << endl;
            assert(0);
        }
    }
    return( n );
}


//===========================================================
//                      Bind
//-----------------------------------------------------------
//-- bind 的返回值 只用于检测 错误，没有其它功能。
//   所以 此wrap函数自己的返回值为 void
void Bind( int _sockfd, const struct sockaddr *_addr, socklen_t _len,
            const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    if( bind( _sockfd, _addr, _len ) < 0 ){
        cout << "Bind: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << ". "
            << _err_info
            << endl;
        assert(0);
    }
}


//===========================================================
//                      Connect
//-----------------------------------------------------------
//-- 类似 Bind。
//-- 但是， connect 作为 用户端函数，拥有更多的 errno 情况，
//   不知为何，在 UNPV 中，没有展开这些。
void Connect( int _sockfd, const struct sockaddr *_servaddr, socklen_t _addrlen,
                const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    if( connect( _sockfd, _servaddr, _addrlen ) < 0 ){
        cout << "Connect: ERROR. "
            << "errno = " << errno << ": " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
}


//===========================================================
//                      Listen
//-----------------------------------------------------------
//-- UNPV 设置得更复杂些，本函数做了简化
//-- 参数 backlog 推荐设置为 1024
void Listen( int _sockfd, int _backlog, const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    if( listen( _sockfd, _backlog ) < 0 ){
        cout << "Listen: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
}


//===========================================================
//                   Inet_ntop
//-----------------------------------------------------------
//-- inet_ntop, 从参数 addrptr 获得 ip地址 网络字节序，
//-- 转换成 点分十进制 ascii码，存入 参数 strptr
//-- 参数 strptr 不可以是 NULL。 这是检测的 第一环。
const char* Inet_ntop( int _family, 
                        const void* _addrptr, 
                        char* _strptr, 
                        socklen_t _size,
                        const std::string &_err_info ){
    
    errno = 0; //-- 清空 errno
    const char *r; 

    if( _strptr == NULL ){
        cout << "Inet_ntop: ERROR. param strptr is NULL."
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }

    if( (r = inet_ntop( _family, _addrptr, _strptr, _size )) == NULL ){
        cout << "Inet_ntop: ERROR. return NULL"
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
    return( r );
}


//===========================================================
//                   Inet_pton
//-----------------------------------------------------------
void Inet_pton( int _family, const char* _strptr, void* _addrptr,
                const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    int n;

    if( (n = inet_pton( _family, _strptr, _addrptr )) == 0 ){
        cout << "Inet_pton: ERROR. IP addr is invalid: "
            << _strptr << " "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
    else if( n == -1 ){
        cout << "Inet_ntop: ERROR. return -1. "
            << _strptr << " "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << _err_info
            << endl;
        assert(0);
    }
}

//===========================================================
//                     Shutdown
//-----------------------------------------------------------
void Shutdown( int _sockfd, int _howto, const std::string &_err_info ){

    errno = 0; //-- 清空 errno
    if( shutdown(_sockfd, _howto) < 0 ){
        cout << "Shutdown: ERROR. return < 0. "
            << _err_info
            << endl;
        assert(0);
    }
}









}//------------------- namespace: tpr ------------------------//


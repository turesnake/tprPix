/*
 * ========================= wrapFuncs.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.09.11
 *                                        修改 -- 2018.10.14
 * ----------------------------------------------------------
 *      包裹函数 
 * ----------------------------
 */

#ifndef _TPR_WRAPFUNCS_H_
#define _TPR_WRAPFUNCS_H_

//#include <sys/socket.h> //- socklen_t, sockaddr
//#include <netinet/in.h> //- sockaddr_in

#include <fcntl.h>  //- mode_t, O_RDONLY, O_WRONLY

#include <string>

namespace tpr {//--------------- namespace: tpr -------------------//


//========================== wrap_unix.cpp ===============================//
int Open( const char *_path, int _oflag, mode_t _mode, const std::string &_err_info );

void Mkdir( const char *_path, mode_t _mode, const std::string &_err_info );
void Mkdirat( int _fd, const char *_path, mode_t _mode, const std::string &_err_info );

void Close( int _fd, const std::string &_err_info );

off_t Lseek( int _fd, off_t _offset, int _whence, const std::string &_err_info);
void Fseeko( FILE *_fp, off_t _offset, int _whence, const std::string &_err_info);
//off_t Ftello( FILE *_fp, const std::string &_err_info );

/*
pid_t Fork( const std::string &_err_info );
void Wait( int *_status, pid_t _pid_target, const std::string &_err_info );
int Select( int _maxfdp1, fd_set *_readset, fd_set *_writeset, fd_set *_exceptset,
            struct timeval *_timeout, const std::string &_err_info );
*/
ssize_t Read( int _fd, void *_buf, size_t _nbytes, const std::string &_err_info );
void Write( int _fd, const void *_buf, size_t _nbytes, const std::string &_err_info );

std::string Getenv( const char *_name, const std::string &_err_info );

//========================== wrap_socket.cpp ===============================//
/*
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
*/



}//------------------- namespace: tpr ------------------------//

#endif

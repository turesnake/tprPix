/*
 * ========================= wrapUnix.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.09.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *      包裹函数 
 * ----------------------------
 */

#ifndef TPR_WRAP_UNIX_H_
#define TPR_WRAP_UNIX_H_

//#include <sys/socket.h> //- socklen_t, sockaddr
//#include <netinet/in.h> //- sockaddr_in

#include <fcntl.h>  //- mode_t, O_RDONLY, O_WRONLY

#include <string>

namespace tprUnix {//--------------- namespace: tprUnix -------------------//


//========================== wrap_unix.cpp ===============================//
int Open( const char *path_, int oflag_, mode_t mode_, const std::string &err_info_ );

void Mkdir( const char *path_, mode_t mode_, const std::string &err_info_ );
void Mkdirat( int fd_, const char *path_, mode_t mode_, const std::string &err_info_ );

void Close( int fd_, const std::string &err_info_ );

off_t Lseek( int fd_, off_t offset_, int whence_, const std::string &err_info_);
void Fseeko( FILE *fp_, off_t offset_, int whence_, const std::string &err_info_);
//off_t Ftello( FILE *fp_, const std::string &err_info_ );

/*
pid_t Fork( const std::string &err_info_ );
void Wait( int *_status, pid_t _pid_target, const std::string &err_info_ );
int Select( int _maxfdp1, fd_set *_readset, fd_set *_writeset, fd_set *_exceptset,
            struct timeval *_timeout, const std::string &err_info_ );
*/
ssize_t Read( int fd_, void *buf_, size_t nbytes_, const std::string &err_info_ );
void Write( int fd_, const void *buf_, size_t nbytes_, const std::string &err_info_ );

std::string Getenv( const char *name_, const std::string &err_info_ );

//========================== wrap_socket.cpp ===============================//
/*
int Socket( int _family, int type_, int _protocol, const std::string &err_info_ );
int Accept( int _sockfd, struct sockaddr *_cliaddr, socklen_t *_addrlen, 
            const std::string &err_info_ );
void Bind( int _sockfd, const struct sockaddr *_addr, socklen_t _len, 
            const std::string &err_info_ );
void Connect( int _sockfd, const struct sockaddr *_servaddr, socklen_t _addrlen, 
            const std::string &err_info_ );
void Listen( int _sockfd, int _backlog, const std::string &err_info_ );
const char* Inet_ntop( int _family, 
                        const void* _addrptr, 
                        char* _strptr, 
                        socklen_t _size,
                        const std::string &err_info_ );
void Inet_pton( int _family, const char* _strptr, void* _addrptr, 
                const std::string &err_info_ );
void Shutdown( int _sockfd, int _howto, const std::string &err_info_ );
*/



}//------------------- namespace: tprUnix ------------------------//

#endif

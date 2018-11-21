/*
 * ========================= MsgIO.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.09.15
 *                                        修改 -- 2018.10.03
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */

#include "MsgIO.h"

//-------------------- C ----------------------//

#include <unistd.h> //- read, write
#include <string.h> //- memcpy, strerror
#include <errno.h> //- errno

#include <cassert> //- assert

#include <limits.h>  //- PIPE_BUF

//-------------------- CPP --------------------//
#include <sstream> //-- stringstream


using std::cout;
using std::endl;
using std::string;
using std::stringstream;

namespace tpr { //--------------- namespace: tpr -------------------//


//---------------------- 局部 变量 ---------------------



/* ===========================================================
 *                        read_msg_3
 * -----------------------------------------------------------
 * -- 从 pipe / fifo / tcp 读取 一条 msg。
 *    调用 readn 来读。 将io结果 记录在 read_result 中，
 *    让调用者 自行处理错误。增加本函数的 适用范围
 * -----------------------------------------------------------
 * -- return
 *    若完全正确，返回 0
 *    若出错，(目前版本，所有错误都向调用者提交，没有调用 assert)
 *          返回 -1. 此时调用者需要 检查 read_result 
 */
int MsgIO::read_msg_3( int fd ){

    //-------------------------------//
    //     标志符 与 暂存容器 清空
    //-------------------------------//
    clear_msgHead( msgHead_read );
    msgBinary_read.clear();
    clear_read_result();

    //-------- 函数临时变量 --------//
    ssize_t len;       //-- 单次调用 read 返回的 已读字节数
    
    int is_eof;    //-- 状态判断，readn 中是否读到 eof
    int fst_errno; //-- 捕捉 readn 中的第一个 errno

    //---------------------------------------//
    //        先读取 一条 msg 的 head 段
    //    len / type / pid_sender / checkSum
    //---------------------------------------//
    len = readn( fd, 
                (void*)&msgHead_read, 
                BASEMSG_HEAD_SIZE,
                &is_eof,
                &fst_errno
                );
    
    read_result.len_head = len;
    if( fst_errno != 0 ){
        read_result.fst_errno = fst_errno;
    }

    //-- 某次 read 时，读到的 EOF，
    //-- 说明 tcp 的对面发了 FIN。或者 pipe/fifo 关闭了
    //-- 调用者 应执行 结束流程，或者再次连接 tcp ／ 管道
    if( is_eof == 1 ){
        read_result.is_EOF = true;
        return -1; //-- 提醒 调用者 检查 read_result
    }

    //-- 若是 返回的 已读 字节数 不对，说明 read 函数调用出错了。
    //-- 错误处理 交给 调用者
    if( len != BASEMSG_HEAD_SIZE ){
        return -1; //-- 提醒 调用者 检查 read_result
    }

            //++  现在 msgHead_read 可以访问了 ++

    //---------------------------------------//
    //             验证 checkSum
    //---------------------------------------//
    if( checkSum_confirm(msgHead_read) == false ){
        read_result.is_checkSum_error = true;
        return -1; //-- 提醒 调用者 检查 read_result
    }

    //---------------------------------------------//
    //   一步到位 地将 msgBinary_read 扩容到 合适的尺寸。
    //---------------------------------------------//
    msgBinary_read.resize( BASEMSG_HEAD_SIZE + msgHead_read.len );

    //---------------------------------------//
    //     将 head 段数据 复制进 二进制容器
    //---------------------------------------//
    memcpy( (void*)&(msgBinary_read[0]),
            (const void*)&msgHead_read,
            BASEMSG_HEAD_SIZE
            );

    //-- 有些 msg.data段 本身就是空的。 此时直接返回 0.
    if( msgHead_read.len == 0 ){
        log_record("read_msg: ", msgHead_read);
        return 0; //-- 正确返回
    }

    //---------------------------------------//
    //        读取 msg 的 data 段
    //     反复读取,直到 一条 msg 组装完成
    //---------------------------------------//
    len = readn( fd, 
                (void*)&(msgBinary_read[ BASEMSG_HEAD_SIZE ]),
                msgHead_read.len,
                &is_eof,
                &fst_errno
                );
    
    read_result.len_data = len;
    if( fst_errno != 0 ){
        read_result.fst_errno = fst_errno;
    }

    //-- 某次 read 时，读到的 EOF，
    //-- 说明 tcp 的对面发了 FIN。或者 pipe/fifo 关闭了
    //-- 调用者 应执行 结束流程，或者再次连接 tcp ／ 管道
    if( is_eof == 1 ){
        read_result.is_EOF = true;
        return -1; //-- 提醒 调用者 检查 read_result
    }

    //-- 若是 返回的 已读 字节数 不对，说明 read 函数调用出错了。
    //-- 错误处理 交给 调用者
    if( len != msgHead_read.len ){
        return -1; //-- 提醒 调用者 检查 read_result
    }

    //---------------------------------------//
    //       现在，一条 msg 完全读取完
    //       msgHead_read    保存 head段数据
    //       msgBinary_read  保存 完整的 msg 的二进制态数据
    //---------------------------------------//
    log_record("read_msg: ", msgHead_read);
    return 0; //-- 正确返回
}


/* ===========================================================
 *                    write_msg_3
 * -----------------------------------------------------------
 * -- 
 * -- return
 *    若完全正确，返回 0
 *    若出错，一部分 直接 assert
 *           一部分 返回 -1. 此时调用者需要 检查 write_result 
 */
int MsgIO::write_msg_3( int fd, u8 *_data, u32 _len, u32 _type, pid_t _pid ){

    //-- 排查 参数 异常 --
    if( _len>0 && _data==NULL ){
        cout << "MsgIO::write_msg_3: ERROR. "
            << "_len>0 && _data==NULL "
            << endl;
        assert(0);
    }

    //-------------------------------//
    //      清空 msg_write
    //-------------------------------//
    clear_msgHead( msgHead_write );
    msgBinary_write.clear();
    clear_write_result();

    //-------------------------------//
    //   复制数据，合成 msgHead_write
    //-------------------------------//
    msgHead_write.len        = _len;
    msgHead_write.type       = _type;
    msgHead_write.pid_sender = (i32)_pid;
    checkSum_build( msgHead_write );

    //---------------------------------------------//
    //   一步到位 地将 msgBinary_write 扩容到 合适的尺寸。
    //---------------------------------------------//
    //-- 整个 msg 的 字节数，也是 此次 二进制容器 要分配的 字节数。
    size_t len_binary;  
    len_binary = BASEMSG_HEAD_SIZE + msgHead_write.len;
    msgBinary_write.resize( len_binary );

    //---------------------------------------//
    //     将 head 段数据 复制进 二进制容器
    //---------------------------------------//
    memcpy( (void*)&(msgBinary_write[0]),
            (const void*)&msgHead_write,
            BASEMSG_HEAD_SIZE
            );
    
    //---------------------------------------//
    //     将 data 段数据 复制进 二进制容器
    //            若没有，则跳过
    //---------------------------------------//
    if( msgHead_write.len > 0 ){
        memcpy( (void*)&(msgBinary_write[BASEMSG_HEAD_SIZE]),
                (const void*)_data,
                (size_t)msgHead_write.len
                );
    }

        //+++ 现在，msgHead_write, msgBinary_write 都制作完毕 +++//

    //-------------------------------//
    //        向 fd 写入 msg
    //-------------------------------//
    int fst_errno; //-- 捕捉 readn 中的第一个 errno
    ssize_t len;
    len = writen( fd,
                (const void*)&(msgBinary_write[0]),
                len_binary,
                &fst_errno
                );
    
    write_result.len_binary = len;
    if( fst_errno != 0 ){
        write_result.fst_errno = fst_errno;
    }

    //-- 若是 返回的 已读 字节数 不对，说明 write 函数调用出错了。
    //-- 错误处理 交给 调用者

    //-- 对于 tcp 来说，一种情况是：
    //   向一个 已经收到 RST 的 套接字 write。
    //   此时 调用 write 的进程 会收到 SIGPIPE 信号。
    //   若从 此信号处理函数返回，或忽略此信号。
    //   write调用 会返回 -1，errno == EPIPE。
    //   此时 调用者 应该 执行 终止流程，或者 重新连接 tcp

    //-- 对于 pipe ／ fifo 来说：
    //   也存在 SIGPIPE，EPIPE 问题。
    if( len != len_binary ){

        if( errno == EPIPE ){
            write_result.is_EPIPE = true;
        }

        return -1; //-- 提醒 调用者 检查 write_result
    }

    log_record( "write_msg: ", msgHead_write );
    return 0; //-- 正确返回  
}


/* ===========================================================
 *                send_msgBinary_read_3
 * -----------------------------------------------------------
 * -- 将 msgBinary_read 中的数据 发送到 pipe / fifo / tcp。
 * -- 简化版本，不再做 read/write 检查
 * -- 调用本函数之前，务必确保：msgHead_read/msgBinary_read
 *    装填完毕
 * 
 * -- 本函数的 结果记录在 write_result 中
 * 
 * -- return
 *    若完全正确，返回 0
 *    若出错，( 目前没有 assert )
 *           大部分 返回 -1. 此时调用者需要 检查 write_result 
 */
int MsgIO::send_msgBinary_read_3( int fd ){

    //-------------------------------//
    //           清空 
    //-------------------------------//
    clear_write_result();

    size_t len_binary;  
    len_binary = BASEMSG_HEAD_SIZE + msgHead_read.len;

    int fst_errno; //-- 捕捉 readn 中的第一个 errno
    ssize_t len;
    len = writen( fd,
                (const void*)&(msgBinary_read[0]),
                len_binary,
                &fst_errno
                );
    
    write_result.len_binary = len;
    if( fst_errno != 0 ){
        write_result.fst_errno = fst_errno;
    }

    //-- 若是 返回的 已读 字节数 不对，说明 write 函数调用出错了。
    //-- 错误处理 交给 调用者
    
    //-- 对于 tcp 来说，一种情况是：
    //   向一个 已经收到 RST 的 套接字 write。
    //   此时 调用 write 的进程 会收到 SIGPIPE 信号。
    //   若从 此信号处理函数返回，或忽略此信号。
    //   write调用 会返回 -1，errno == EPIPE。
    //   此时 调用者 应该 执行 终止流程，或者 重新连接 tcp

    //-- 对于 pipe ／ fifo 来说：
    //   也存在 SIGPIPE，EPIPE 问题。
    if( len != len_binary ){

        if( errno == EPIPE ){
            write_result.is_EPIPE = true;
        }

        return -1; //-- 提醒 调用者 检查 write_result
    }

    log_record( "write_msg: ", msgHead_read );
    return 0; //-- 正确返回 
}


/* ===========================================================
 *                      checkSum_build  --2
 * -----------------------------------------------------------
 * -- 阉割版，目前只留了一个 固定值 写入
 */
void MsgIO::checkSum_build( baseMsgHead &_msgh ){
    _msgh.checkSum = CHECK_SUM;
}

/* ===========================================================
 *                   checkSum_confirm  --2
 * -----------------------------------------------------------
 * -- 阉割版，目前只留了一个 固定值 检测
 */
bool MsgIO::checkSum_confirm( baseMsgHead &_msgh ){

    if( _msgh.checkSum == CHECK_SUM ){
        return true;
    }else{
        cout << "MsgIO::checkSum_confirm: ERROR. "
            << "checkSum != CHECK_SUM"
            << endl;
        return false;
    }
}

/* ===========================================================
 *                     log_record  --2
 * -----------------------------------------------------------
 * -- 每成功 read／write 一条msg，就 添加一条记录。
 * -- 目前仅仅是 打印到 终端上。
 * -- 本函数 受标志符 is_infoPrint_on 的控制
 */
void MsgIO::log_record( const std::string &_head, 
                baseMsgHead &_msgh ){
    
    if( is_infoPrint_on == false ){
        return;
    }
    //------------------------------//
    cout << "    " << _head.c_str() 
        <<   "len = " << _msgh.len 
        << ". type = " << _msgh.type
        << ". pid = " << _msgh.pid_sender
        << "." << endl;
}

/* ===========================================================
 *              check_msgHead_read_type --2
 * -----------------------------------------------------------
 * -- 确保 msgHead_read.type == _type. 否则报错
 */
bool MsgIO::check_msgHead_read_type( u32 _type, std::string &err_info ){

    if( msgHead_read.type != _type ){
        cout << "MsgIO::check_msgHead_read_type: ERROR. "
            << "msgHead_read.type != " << _type << "\n"
            << err_info
            << endl;
        assert(0);
        return false; //-- 暂不会执行到 此行
    }

    return true;
}


/* ===========================================================
 *                    read_result_2_str
 * -----------------------------------------------------------
 */
std::string MsgIO::read_result_2_str( const std::string &_err_info ){

    stringstream ss;
    ss << _err_info 
        << "\n    fst_errno         = " << read_result.fst_errno
        << "\n    is_EOF            = " << bool_2_str(read_result.is_EOF)
        << "\n    is_checkSum_error = " << bool_2_str(read_result.is_checkSum_error)
        << "\n    len_head = " << read_result.len_head
        << "\n    len_data = " << read_result.len_data;
    
    return ss.str(); //-- 直接获取 ss 中的 完整字符串拷贝
}

/* ===========================================================
 *                   write_result_2_str
 * -----------------------------------------------------------
 */
std::string MsgIO::write_result_2_str( const std::string &_err_info ){
    
    stringstream ss;
    ss <<  _err_info 
        << "\n    fst_errno  = " << write_result.fst_errno
        << "\n    is_EPIPE   = " << bool_2_str(write_result.is_EPIPE)
        << "\n    len_binary = " << write_result.len_binary;

    return ss.str(); //-- 直接获取 ss 中的 完整字符串拷贝
}


/* ===========================================================
 *                       bool_2_str
 * -----------------------------------------------------------
 */
std::string bool_2_str( bool _b ){
    if( _b == true ){
        return "true";
    }else{
        return "false";
    }   
}




}//------------------- namespace: tpr ------------------------//


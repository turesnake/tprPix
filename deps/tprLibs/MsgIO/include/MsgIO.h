/*
 * ========================= MsgIO.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.09.15
 *                                        修改 -- 2018.09.15
 * ----------------------------------------------------------
 * 
 *   将 fifo / tcp 的数据 结构化( baseMsg )
 *   msg.data 段内容为 二进制数据，可进一步结构化
 *   本模块 力求 通用
 * 
 * ----------------------------
 * 依赖的其它库：
 *        tprDataType
 * ----------------------------
 */

#ifndef _TPR_MSG_IO_H_
#define _TPR_MSG_IO_H_

//------------ C -------------//
#include <unistd.h> //-- pid

//------------ CPP -------------//
#include <string>
#include <vector> 
#include <iostream> 

//------------ self -------------//
#include "tprDataType.h"
#include "MsgType.h" 
#include "MsgDataHead.h" 


namespace tpr { //--------------- namespace: tpr -------------------//


//-- 当 read_msg() 出现 checkSUm验证失败时。往往意味着 管道／FIFO 里的数据已经
//-- 混乱了。目前采用的解决方案是: 再次调用 read_msg()
//-- 当 复读若干次后仍无效，就直接报错 （一个不是很完善的方案）
#define NR_REREAD_MAX 10 //-- nr_reread 所能允许的 最大值

//------------ pid --------------
#define PID_NULL    0         //-- 表示 此pid 未设置
#define PID_SERVER  (u32)(-1) //-- 用一个极大数来表达，此pid 发自 服务器

//======================================================//
//                       baseMsg
//------------------------------------------------------//
//-- 从 fifo / tcp 中读取数据，要读取两次：
//- 第一次读取 BASEMSG_HEAD_SIZE 字节，从中获得 len, type, pid_client, checkSum
//- 第二次读取 msg.len 个字节，存入 msg.data 段
#define BASEMSG_HEAD_SIZE ( sizeof(u32) * 3 + sizeof(i32) )

//- pid_sender 在 二进制容器 中的 地址偏移量。
#define BASEMSG_PID_OFFSET ( sizeof(u32) * 2 )

//-- 阉割版 checkSum 的 固定值。值随意
#define CHECK_SUM 7171

//----- msg head 的 通用结构 ------//
//     fifo / tcp 都遵循此格式
//-------------------------------//
struct baseMsgHead{
    u32         len;        //-- 仅仅表示 data 段的 字节数
    u32         type;       //-- msg 类型
    i32       pid_sender;   //-- 发送者的 pid，只在部分场合有作用。
                            //   为了支持跨平台，将 pid_t 改为 msgi32
    u32         checkSum;   //-- 检验和。 head段 验证用
                            //-- 目前为阉割版，写入固定值 CHECK_SUM
};

//----- msg read/write 的 结果 ----//
//-------------------------------//
struct ReadResult{
    int  fst_errno;           //-- 捕捉到的第一个 errno
    bool is_EOF;              //-- 是否读到 EOF。
    bool is_checkSum_error;   //-- checkSum 是否出错
    int  len_head;  //-- 已经 read 到的 head 段字节数
    int  len_data;  //-- 已经 read 到的 data 段字节数
};

struct WriteResult{
    int  fst_errno; //-- 捕捉到的第一个 errno
    bool is_EPIPE;  //-- 当 write 返回 -1 出错时，errno 是否为 EPIPE
    int len_binary; //-- 已经 write 的 msg 字节数（包含 head 和 data）
};


/* ===========================================================
 *                        MsgIO 
 * -----------------------------------------------------------
 */
class MsgIO{

    //============== friend ==============
public:
    //-- 构造函数 --//
    MsgIO( bool _is_infoPrint_on = true )
        :
        is_infoPrint_on(_is_infoPrint_on)
        {}

    //---- 第三代 msg IO 函数 ----//
    // 1 -- 通过 readn / writen 实现底层io
    // 2 -- 将调用结果 存储在 read_result / write_result
    // 3 -- 将大部分错误 交给 调用者 去处理
    //     （这意味着，每个调用者，都要亲自实现 本套io函数的 包裹函数）
    int read_msg_3( int fd );
    int write_msg_3( int fd, u8 *_data, u32 _len, u32 _type, pid_t _pid );
    int send_msgBinary_read_3( int fd );


    //-- 返回 msgHead_read 的引用。
    //-- 调用者 可通过此 接口来获得 msgHead_read 数据
    inline const baseMsgHead &get_msgHead_read() const
    {
        return msgHead_read;
    }

    //-- 返回 msgHead_write 的引用。
    //-- 调用者 可通过此 接口来获得 msgHead_write 数据
    inline const baseMsgHead &get_msgHead_write() const
    {
        return msgHead_write;
    }

    //-- 访问 msgHead_read.type
    inline u32 get_msgHead_read_type()
    {
        return msgHead_read.type;
    }

    //-- 获得 二进制容器 的 data段数据 访问权
    //-- 通过 参数 _lenp 返回 data段 字节数
    inline u8* get_msgData_read( u32 *_lenp )
    {
        *_lenp = msgHead_read.len;
        return (u8*)&(msgBinary_read[ BASEMSG_HEAD_SIZE ]);
    }

    //-- 获得 二进制容器 的 data段数据 访问权
    //-- 通过 参数 _lenp 返回 data段 字节数
    inline u8* get_msgData_write( u32 *_lenp )
    {
        *_lenp = msgHead_write.len;
        return (u8*)&(msgBinary_write[ BASEMSG_HEAD_SIZE ]);
    }

    //-- 通过此函数，专门修改 msg read 中的 pid，
    //-- head 和 二进制容器 都要改
    inline void change_msg_read_pid( pid_t _pid )
    {
        //------- 修改 head ---------
        msgHead_read.pid_sender = (i32)_pid;
        //------- 修改 二进制容器 ---------
        i32* pidp = (i32*)&(msgBinary_read[ BASEMSG_PID_OFFSET ]);
        *pidp = (i32)_pid;
    }

    //-- 获得 read_result
    //-- 记录了 read_tcp 函数的 执行结果
    inline const ReadResult &get_read_result() const
    {
        return read_result;
    }

    //-- 获得 write_result
    //-- 记录了 write_tcp 函数的 执行结果
    inline const WriteResult &get_write_result() const
    {
        return write_result;
    }

    inline void set_is_infoPrint_on( bool _b )
    {
        is_infoPrint_on = _b;
    }


    //-- 确保 msgHead_read.type == _type. 否则报错
    bool check_msgHead_read_type( u32 _type, std::string &err_info );

    //-- 将 result 变成 字符串string
    std::string read_result_2_str( const std::string &_err_info );
    std::string write_result_2_str( const std::string &_err_info );

private:

    //---- MsgIO 是 类“双全工” 的，读取数据独立存储 ----//

    //-- msgHead 数据 和 下方完整的 二进制数据 是 部分重合的，但是没关系，开销不大
    baseMsgHead msgHead_read;  //-- 从 fd 读取出的 msg head
    baseMsgHead msgHead_write; //-- 将要写入 fd 的 msg head

    //-- 整个 msg 的数据 被整合成一个 完整的 二进制数据包。
    std::vector<u8> msgBinary_read;
    std::vector<u8> msgBinary_write;

    //-- 记录 read ／ write 执行结果的 信息
    ReadResult read_result;
    WriteResult write_result; 

    //-- 标志符，是否 开启 信息打印：每次 read/write 成功，
    //-- 都会向 终端 打印 信息。 默认为 开（true）
    bool is_infoPrint_on;
    
    //------ func ------s
    inline void clear_msgHead( baseMsgHead &_msgh ) //-- 将 msg 清空
    {
        _msgh.len = 0;
        _msgh.type = MSG_TYPE_NULL;
        _msgh.pid_sender = PID_NULL;
        _msgh.checkSum = 0;
    }

    inline void clear_read_result(){
        read_result.fst_errno = 0;
        read_result.is_EOF = false;
        read_result.is_checkSum_error = false;
        read_result.len_head = 0;
        read_result.len_data = 0;
    }

    inline void clear_write_result(){
        write_result.fst_errno = 0;
        write_result.is_EPIPE = false;
        write_result.len_binary = 0;
    }

    void checkSum_build( baseMsgHead &_msgh );  //- 为 msg 生成检验和
    bool checkSum_confirm( baseMsgHead &_msgh );//- 检测 msg 的检验和


    void log_record( const std::string &_head, //- 单条记录
                baseMsgHead &_msgh );

};

//---- 暂时把 readn / writen 系列函数 放在此处 ----
ssize_t readn( int _fd, void *_buf, size_t _nbytes, 
                int *_is_EOF, int *_fst_errno );

ssize_t writen( int _fd, const void *_buf, size_t _nbytes,
                int *_fst_errno );


//-- 根据 bool ，获得字符串 "true" / "false"
std::string bool_2_str( bool _b );


}//------------------- namespace: tpr ------------------------//

#endif


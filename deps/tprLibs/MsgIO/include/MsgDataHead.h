/*
 * ========================= MsgDataHead.h ==========================
 *                            -- tpr --
 *                                        创建 -- 2018.09.20
 *                                        修改 -- 2018.09.20
 * ----------------------------------------------------------
 * 
 *   msg data 段 数据结构
 * 
 * ----------------------------
 */

#ifndef _TPR_MSG_DATA_HEAD_H_
#define _TPR_MSG_DATA_HEAD_H_

#include "tprDataType.h"


namespace tpr { //--------------- namespace: tpr -------------------//

//======= msg.data 段的 结构 ======
//-- 每次玩家登陆时，client 发送给 主逻辑进程的 登记数据
struct msgDataHead_board{
};




//=========== msg.data 段的 head段的 结构 ==========
//-- 对应type: MSG_TYPE_SEND_FILE, MSG_TYPE_RECV_FILE
//-- 
struct msgDataHead_cp_file{
    u64    seq_ack;  //-- 在 文件发送者 发出的 msg 中，此为 seq（序号）
                        //      代表 本条msg中，数据首字节，在 文件中的 偏移字节数
                        //-- 在 文件接受者 发出的 msg 中，此为 ack（确认号）
                        //      代表 接受者 已接收的 最后一个字节号 + 1
                        //      也就是，期望 发送者在 下一条 msg 中的 数据首字节 的 序号。
                        //--  由于是 半双工，所以 两者合一

                        //-- 当值为 (u64)(-1) 时，表示此条msg 是一个 "路径确认msg"

    u8     cmd;   //-- 指令。（应当首先检测 此字段）
                     //-- 具体选项参见下方
                     
};
//-- 头结构 字节数
#define MSGDH_CP_FILE_SIZE (sizeof(struct msgDataHead_cp_file))

#define MSGDH_CP_FILE_CMD_PATH 1 //- 本条msg 为 ”路径确认msg“。（此时 seq_ack 值 是无意义的）
                                 //  若 路径有效，     回复 GOON
                                 //  若 存在重复文件名，回复 DUPNAME
                                 //  若 路径无效       回复 FAIL

#define MSGDH_CP_FILE_CMD_DUPNAME 2 //- 路径名存在重复，接受者自行改写了路径名（添加后缀 .n）
                                    //- 此条信息中，seq_ack 的值 表示 接受者 设置的 后缀值 (接受者 回复 发送者)

#define MSGDH_CP_FILE_CMD_DATA 3 //- 本条msg 为 数据传输msg。（此时可读取 seq_ack 值）

#define MSGDH_CP_FILE_CMD_GOON 4 //- 可以继续传输（上一条msg 成功接收）   (接受者 回复 发送者)
#define MSGDH_CP_FILE_CMD_REDO 5 //- 请求重传（上一条msg 接收失败／出错） (接受者 回复 发送者)
#define MSGDH_CP_FILE_CMD_FAIL 6 //- 终止（传输失败）。(因为某些原因，此次文件传输被终止，此时，msg中应该携带一段额外的数据，
                                 //    记录传输状态, 以及 出错理由)  （双向）

//-- 传输失败导致的 终止时。主动提出终止的 那一方，发送的 msg中，携带的 数据的结构
//-- 记录 当前传输状态, 以及 出错理由
struct msgDataHead_cp_file_fail{

};




}//------------------- namespace: tpr ------------------------//

#endif


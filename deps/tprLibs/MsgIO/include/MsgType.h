/*
 * ========================= MsgType.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.09.20
 *                                        修改 -- 2018.09.20
 * ----------------------------------------------------------
 * 
 *   baseMsg 中的 type段 选项
 * 
 * ----------------------------
 */

#ifndef _TPR_MSG_TYPE_H_
#define _TPR_MSG_TYPE_H_

namespace tpr { //--------------- namespace: tpr -------------------//

//======= baseMsg.type =======
#define MSG_TYPE_NULL    0 //-- 什么类型也不属于

//======== game client =========
#define MSG_TYPE_BOARD   1 //-- client 第一次登陆，获取 完整的 当前状态信息
#define MSG_TYPE_UPDATE  2 //-- 每隔 1/n 秒，更新／同步 一次 全局状态信息


//======= fifo create ========
//-- 所有 子进程 向 公共fifo口 发送数据之前，
//-- 都会先通过以下 一对msg 建立 双向连接
#define MSG_TYPE_FIFO_CREATE 3 
                        //-- client -> centre logic
                        //-- shell  -> main
                        //-- 请求建立一条 fifo 连接。data段为 空。
#define MSG_TYPE_FIFO_CREATE_REPLY 4
                        //-- 收到上一类型 msg 后， 主逻辑进程 回复的msg。 
                        //-- 通过这个 看似多余的 流程，client进程的 open 函数就不再 阻塞了


//======== shell IO =========
#define MSG_TYPE_EXIT_ALL 5 
                        //-- 外部shell -> server main
                        //-- 请求 终止所有 服务器进程
#define MSG_TYPE_EXIT_ALL_REPLY 6
                        //-- server main -> 外部shell
                        //-- 针对上一 msg 的回复


//========= 在 tcp 中传递纯粹的 字符串 ========= 
#define MSG_TYPE_STRING 7 //-- 不包含 尾后0，需要接收者 自行补上。


//========= 在 tcp 中传递 一个文件 =========
#define MSG_TYPE_SEND_FILE 8 
                        //-- [client --> server]
                        //-- 发送者 将文件分成 数个 数据块，每个块都包装成一个 msg（使用本type）
                        //-- 接受者 针对收取的 每个 数据块，都回复 发送者一个 msg（使用本type）
                        //-- 关联 结构：

#define MSG_TYPE_RECV_FILE 9
                        //-- [client <-- server]
                        


}//------------------- namespace: tpr ------------------------//

#endif

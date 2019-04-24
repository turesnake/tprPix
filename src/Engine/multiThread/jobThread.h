/*
 * ======================= JobThread.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  游戏存在一个 主线程，和数个 job线程
 *  主线程 向 esrc::jobQue 写入 job。
 *  job线程 从 esrc::jobQue 取出job，并完成它们
 * ----------------------------
 */
#ifndef _TPR_JOB_THREAD_H_
#define _TPR_JOB_THREAD_H_


void jobThread_main();


#endif 


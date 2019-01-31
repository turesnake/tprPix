/*
 * ========================= TimeBase.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   定义一种 时间循环
 * ----------------------------
 */
#ifndef _TPR_TIME_CIRCLE_H_
#define _TPR_TIME_CIRCLE_H_

#include "TimeBase.h"



//--- 管理一种 时间循环 ---//
//  每 n 帧 算一次 循环
class TimeCircle{
public: 
    explicit TimeCircle( TimeBase *_timerp, int _frameNum ):
        timerp(_timerp),
        frameNum(_frameNum)
        {}

    
    //-- 返回当前 帧 在 时间循环中的 位置 --
    inline int current(){
        return (timerp->get_frameNum() % frameNum);
    }


private:
    TimeBase  *timerp; //- 绑定一个 全局 timer
    int        frameNum; //- 一个circle 需要 多少 帧。

};


#endif

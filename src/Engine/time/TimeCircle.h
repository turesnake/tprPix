/*
 * ========================= TimeBase.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   定义一种 时间循环
 * ----------------------------
 */
#ifndef TPR_TIME_CIRCLE_H
#define TPR_TIME_CIRCLE_H

#include "TimeBase.h"

//--- 管理一种 时间循环 ---//
//  每 n 帧 算一次 循环
class TimeCircle{
public: 
    TimeCircle( TimeBase &timerRef_, size_t frameNum_ ):
        timerRef(timerRef_),
        frameNum(frameNum_)
        {}

    //-- 返回当前 帧 在 时间循环中的 位置 --
    inline size_t current()noexcept{
        return (this->timerRef.get_frameNum() % this->frameNum);
    }

private:
    TimeBase  &timerRef; //- 绑定一个 全局 timer
    size_t     frameNum;        //- 一个circle 需要 多少 帧。
};


#endif

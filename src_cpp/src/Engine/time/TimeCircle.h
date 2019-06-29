/*
 * ========================= TimeBase.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   定义一种 时间循环
 * ----------------------------
 */
#ifndef TPR_TIME_CIRCLE_H_
#define TPR_TIME_CIRCLE_H_

#include "TimeBase.h"


//--- 管理一种 时间循环 ---//
//  每 n 帧 算一次 循环
class TimeCircle{
public: 
    explicit TimeCircle( TimeBase *timerPtr_, size_t frameNum_ ):
        timerPtr(timerPtr_),
        frameNum(frameNum_)
        {}

    //-- 返回当前 帧 在 时间循环中的 位置 --
    inline size_t current(){
        return (this->timerPtr->get_frameNum() % this->frameNum);
    }

private:
    TimeBase  *timerPtr  {nullptr}; //- 绑定一个 全局 timer
    size_t     frameNum  {};        //- 一个circle 需要 多少 帧。

};


#endif

/*
 * ========================= FramePos.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    animFrameSet 中，单张 图元帧 拥有的 全部 pos数据集
 */
#ifndef TPR_FRAME_POS_H_
#define TPR_FRAME_POS_H_

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "MapCoord.h"
#include "AltiRange.h"
#include "ColliEntHead.h"
#include "MapEntCompass.h"
#include "NineBox.h"
#include "IntVec.h"

//-- 仅用来描述 animFrameSet，所以必须是 静态数据 --//
class FramePos{
public:
    FramePos(){}

    //---- set ----//
    // set 阶段 无需考虑性能
    inline void clear_all(){
        this->is_rootAnchorPos_set = false;
        this->is_colliEntHead_set  = false;
    }

    //-- 统一设置 --
    inline void set_rootAnchorPPosOff( const IntVec2 &rootAnchorPPosOff_ ){
        this->rootAnchorPPosOff = rootAnchorPPosOff_;
        this->is_rootAnchorPos_set  = true;
    }


    //-- ceh --
    inline void set_colliEntHead( const ColliEntHead &ceh_ ){
        this->colliEntHead = ceh_; //- copy
        this->is_colliEntHead_set = true;
    }


    void calc_ceh_mposOff_from_cesLB_2_centerMPos();
    void calc_ceh_rootAnchorCompass_and_off_from_rootAnchor_2_mapEntMid();


    //-- 检测 rootAnchor偏移 是否与 idx指向的 colliEntSet预制件
    //   中的 center 偏移 吻合 
    void check();

    //---- get ----//
    inline const IntVec2 &get_rootAnchorPPosOff() const {
        return this->rootAnchorPPosOff;
    }


    //-- IMPORTANT !!! --//
    inline const ColliEntHead &get_colliEntHead() const {
        return this->colliEntHead;
    }
    
private:

    IntVec2     rootAnchorPPosOff  {};
                        //-- 最原始的数据，从 图元帧左下角ppos，到 rootAnchor点的 pposOff
                        //-- *** 不用对齐于 mapEnt ***
                        //-- 只有在以 图元帧左下角 思考问题时，此值才有意义。

    //-- 精简版，每个 图元帧，仅拥有一个 ceh
    ColliEntHead  colliEntHead {};

                    //-- 在这里，ceh 也可以被打散


    
    //-- 在未来，下方部分 flag 可能会被改成 计数器。
    bool is_rootAnchorPos_set       {false};
    bool is_colliEntHead_set        {false};
};


#endif 


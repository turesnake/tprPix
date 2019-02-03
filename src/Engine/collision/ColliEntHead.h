/*
 * ========================= ColliEntHead.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    animFrameSet:: J.png 往往会携带 数个 ColliEntHead 信息点。
 *    本结构是对这个 信息点的 描述。
 *    因为这组信息往往会 存储在一起: animFrameSet.framePos.colliEntHead
 *    ----
 *    这个 struct 名字不够好，后期可能会被修改
 * ----------------------------
 */
#ifndef _TPR_COLLI_ENT_HEAD_H_
#define _TPR_COLLI_ENT_HEAD_H_

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "AltiRange.h"
#include "MapCoord.h"


class ColliEntHead{
public:

    inline void clear_all(){
        colliEntSetIdx = 0;
        pposOff_fromRootAnchor.clear_all();
        lAltiRange.clear_all();
    }

    //======== vals ========//
    int         colliEntSetIdx; 
                            //- ces预制件 idx号

    IntVec2     pposOff_fromRootAnchor;
                            //-- 从图元帧 rootAnchorPPos，到 本ces左下角 的偏移向量
                            //   *** 注意，此值可为 任意整形值，不用对齐于 mapEnt ***
                            //   这个值 被用于 碰撞检测

    AltiRange   lAltiRange;
                            //- 和 colliEntHead 对应，记载每一个 ces实例 携带的 高度区间信息
                            //-- 仅仅是 相对高度区间 

    bool        isCarryAffect {false}; 
                            //- 本 ceh 是否携带 affect／碰撞触发的主动技能
                            // 挥刀动作中，刀部的 ceh，火球的ceh，都会携带 affect
                            // 常规生物的 身体部分的 ceh，则不会携带 affect
                            // 用于 碰撞检测
};

#endif 


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

                            // 这个概念可能要被修改...

    bool        isBody       {true};
                            //- body    - “实体ceh” 会被登记到 mapent上，比如 生物的身体部分。
                            //- virtual - “虚幻ceh” 不会被登记到mapent上，比如火球，刀。
                            // ------------------
                            //  在 J.png 数据上， 
                            //  (low, high)         时，isBody == true
                            //  (100+low, 100+high) 时，isBody == false

            //- "虚幻ceh" 的优势在于，它可以实现 “单向碰撞检测”:
            //  由于 虚幻ceh 自身并不登记在 mapent上，所以别人无法碰撞它
            //  但它可以主动碰撞别人。
            //  这在实现 火球，箭，挥出的刀 等 ceh时 特别有效。

            //- 暂时使用 "body" 来表示 “实体”。因为没找到更好的翻译...
};

#endif 


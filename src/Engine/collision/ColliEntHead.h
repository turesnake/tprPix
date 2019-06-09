/*
 * ========================= ColliEntHead.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    animFrameSet:: J.png 的每一帧，都会拥有一个 ceh实例
 *    存储在: animFrameSet.framePos.colliEntHead
 * 
 *          目前只存在于 FramePos 中，很可能被 打散整合 ...
 * 
 * ----------------------------
 */
#ifndef TPR_COLLI_ENT_HEAD_H_
#define TPR_COLLI_ENT_HEAD_H_

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "AltiRange.h"
#include "MapCoord.h"
#include "MapEntCompass.h"


class ColliEntHead{
public:

    inline void clear_all(){
        colliEntSetIdx = 0;
        mposOff_from_cesLB_2_centerMPos.clear_all();
        lAltiRange.clear_all();
    }

    //======== vals ========//
    int         colliEntSetIdx; 
                            //- ces预制件 idx号

    IntVec2     mposOff_from_cesLB_2_centerMPos;
                            //  ces左下角，到 rootAnchor 所在mapent 的 mpos 偏移。
                            //  值等于 ces.centerMPos。
                            //  用来快速计算 ces 左下角 mpos

    MapEntCompass  rootAnchorCompass {};
                            // 和 ces.centerCompass 是一摸一样的

    glm::vec2  off_from_rootAnchor_2_mapEntMid;
                            //  将 goPos.currentFPos 加上此值，就能获得 rootAnchor 所在 mapent中心 的fpos
                            //  用于 新版的 crawl 位移系统
                            //  ----
                            //  一个固定值，原本应该存储在 ces中，为了此值的访问频率很高，故存储于此
                            //  仅用于 goPos 

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
                            //  目前，这个值 主要在 png文件 内被设置
                            //  在 J.png 数据上， 
                            //  (low, high)         时，isBody == true
                            //  (100+low, 100+high) 时，isBody == false
                            // --------------
                            // 在未来，需要提供 代码层的 改写口，方便在代码中制作一些 隐形的 技能向 碰撞区
                            

            //- "虚幻ceh" 的优势在于，它可以实现 “单向碰撞检测”:
            //  由于 虚幻ceh 自身并不登记在 mapent上，所以别人无法碰撞它
            //  但它可以主动碰撞别人。
            //  这在实现 火球，箭，挥出的刀 等 ceh时 特别有效。

            //- 暂时使用 "body" 来表示 “实体”。因为没找到更好的翻译...
};

#endif 


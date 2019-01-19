/*
 * ========================= AnchorPos.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.19
 *                                        修改 -- 2019.01.19
 * ----------------------------------------------------------
 *    rootAnchor / childAnchor
 * ----------------------------
 */
#ifndef _TPR_ROOT_ANCHOR_POS_H_
#define _TPR_ROOT_ANCHOR_POS_H_


//-------------------- Engine --------------------//
#include "NineBox.h"
#include "PixVec.h"
#include "MapCoord.h"


//-- rootAnchor / childAnchor 记录在 action:: j.png 图片资源中。
//   但是 这组数据 过于复杂，制作一个 结构 来管理它。
class AnchorPos{
public:
    PixVec2     pposOff     {};
                        //-- 最原始的数据，从 图元帧左下角，到 rootAnchor点的 pposOff
                        //-- *** 可以是任意 整形数，不用对齐于 mapEnt ***
                        //-- 只有在以 图元帧左下角 思考时，这个值才有意义。

    NineBox     compass   {};
                        //-- 当对齐到 mapent坐标系后，rootAnchor 在其 mapEnt 中的位置

    //---- funcs ----//
    inline void clear_all(){
        pposOff.clear_all();
        compass = nineBoxNull;
    }


    //-- 调用者传入 图元帧左下角的 当前ppos，
    //   计算出 本anchor 所在的 mapent pos
    inline MapCoord calc_mapCoordPos( const PixVec2 &_left_bottom_ppos ){

        PixVec2 pixPPos = _left_bottom_ppos + pposOff + PixVec2{ compass.x, compass.y };

        MapCoord  mcpos; //- return
        mcpos.set_by_ppos( pixPPos );
        return mcpos;
    } 
    

};




#endif 


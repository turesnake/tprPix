/*
 * ========================= AnchorPos.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    rootAnchor / childAnchor
 * ---
 *   这组数据仅仅是 anchor 相对于 图元帧左下角的 “相对数据” 
 *   而且是静态的，不代表任何 go的 当前pos
 * ----------------------------
 */
#ifndef _TPR_ROOT_ANCHOR_POS_H_
#define _TPR_ROOT_ANCHOR_POS_H_


//-------------------- Engine --------------------//
#include "NineBox.h"
#include "MapEntCompass.h"
#include "IntVec.h"
#include "MapCoord.h"


//-- rootAnchor / childAnchor 记录在 animFrameSet:: j.png 图片资源中。
//   但是 这组数据 过于复杂，制作一个 结构 来管理它。
class AnchorPos{
public:
    IntVec2         pposOff     {};
                        //-- 最原始的数据，从 图元帧左下角，到 rootAnchor点的 pposOff
                        //-- *** 可以是任意 整形数，不用对齐于 mapEnt ***
                        //-- 只有在以 图元帧左下角 思考问题时，此值才有意义。

    MapEntCompass   compass   {};
                        //-- 当对齐到 mapent坐标系后，rootAnchor 在其 mapEnt 中的位置 (基于左下角)

    //---- funcs ----//
    inline void clear_all(){
        pposOff.clear_all();
        compass.clear_all();
    }


    //-- 调用者传入 图元帧左下角的 当前ppos，
    //   计算出 本anchor 所在的 mapent pos
    inline MapCoord calc_mapCoordPos( const IntVec2 &_left_bottom_ppos ){

        IntVec2 pixPPos = _left_bottom_ppos + pposOff - compass.to_IntVec2();
        return MapCoord { ppos_2_mpos(pixPPos) };
    } 
    

};




#endif 


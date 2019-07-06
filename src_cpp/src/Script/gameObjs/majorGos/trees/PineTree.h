/*
 * ========================= PineTree.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   橡树
 * ----------------------------
 */
#ifndef TPR_PINE_TREE_H
#define TPR_PINE_TREE_H

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObj.h"
#include "GameObjMesh.h"
#include "AnimFrameSet.h"
#include "MapAltitude.h"
#include "Density.h"


namespace gameObjs{//------------- namespace gameObjs ----------------

struct PineTree_PvtBinary{
    
    int   age {0}; 
        //-- 树木年龄：
        // 1 -- 树苗
        // 2 -- 幼树
        // 3 -- 成年树
        // 4 -- 巨树（树精）
    
    size_t  pineId {0};
        // 每颗树在 init 最初阶段，就根据 age，isSingleTRunk,
        // 分配得到一个 具体的 树id。（一般对应 actionFrames图中某一帧）
        // [0, 17]

    //===== padding =====//
    //u8_t padding[3]  {0};
};


class PineTree{
public:
    PineTree() = default;

    //--- 延迟init ---//
    static void init_in_autoMod(   goSpecId_t specID_,
                            GameObj &goRef_,
                            const IntVec2 &mpos_,
					        double fieldWeight_,
					        const MapAltitude &alti_,
					        const Density &_density );

private:

    //--  每次调用回调函数，都需要做的 指针重绑定 --
    inline static  PineTree_PvtBinary *rebind_ptr( GameObj &goRef_ ){
        return reinterpret_cast<PineTree_PvtBinary*>(goRef_.get_pvtBinaryPtr());
    }
                            
    static void bind( GameObj &goRef_ );
    static void rebind( GameObj &goRef_ );

    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnLogicUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace gameObjs: end ----------------
#endif 


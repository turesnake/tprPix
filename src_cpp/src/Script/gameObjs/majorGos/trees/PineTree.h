/*
 * ========================= PineTree.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   橡树
 * ----------------------------
 */
#ifndef TPR_PINE_TREE_H_
#define TPR_PINE_TREE_H_

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObj.h"
#include "GameObjMesh.h"
#include "AnimFrameSet.h"
#include "PubBinaryValType.h"
#include "MapAltitude.h"
#include "Density.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


//-- 定义了 go.binary 的数据格式 --
inline std::vector<PubBinaryValType> pineTree_pubBinaryValTypes {
    PubBinaryValType::HP,
    PubBinaryValType::MP
};

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
    void init_in_autoMod(   GameObj &goRef_,
                            const IntVec2 &mpos_,
					        float fieldWeight_,
					        const MapAltitude &alti_,
					        const Density &_density );
                            
    void bind( GameObj &goRef_ );

    //--- 从硬盘读取到 go实例数据后，重bind callback
    void rebind( GameObj &goRef_ );

    //--- callback ---//
    void OnRenderUpdate( GameObj &goRef_ ); 
    void OnLogicUpdate( GameObj &goRef_ ); 


    //--  每次调用回调函数，都需要做的 指针重绑定 --
    inline PineTree_PvtBinary *rebind_ptr( GameObj &goRef_ ){
        
        tprAssert( goRef_.species == PineTree::specId );
        return reinterpret_cast<PineTree_PvtBinary*>(goRef_.get_pvtBinaryPtr());
    }

    //======== static ========//
    static  u32_t  specId; //- 在 onGoSpecIds_SignUp() 中手动设置...


private:

    //--- callback ---//
    void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};

//---------- static ----------//
inline u32_t  PineTree::specId {0}; //- 具体值在 goSpecIds.cpp 中分配

//=====< 本类 唯一的 保留实例 >=====
inline PineTree  pineTree {};


}//------------- namespace gameObjs: end ----------------
#endif 


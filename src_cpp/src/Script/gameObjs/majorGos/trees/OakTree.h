/*
 * ========================= OakTree.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   橡树
 * ----------------------------
 */
#ifndef TPR_OAK_TREE_H
#define TPR_OAK_TREE_H

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
inline std::vector<PubBinaryValType> oakTree_pubBinaryValTypes {
    PubBinaryValType::HP,
    PubBinaryValType::MP
};

struct OakTree_PvtBinary{
    
    int   age {0}; 
        //-- 树木年龄：
        // 1 -- 树苗
        // 2 -- 幼树
        // 3 -- 成年树
        // 4 -- 巨树（树精）
    
    size_t  oakId {0};
        // 每颗树在 init 最初阶段，就根据 age，isSingleTRunk,
        // 分配得到一个 具体的 树id。（一般对应 actionFrames图中某一帧）
        // [0, 17]

    bool  isSingleTRunk {true};
        // true  -- 只有一个主树干
        // false -- 有多个分叉树干

    //===== padding =====//
    u8_t padding[3]  {0};
};


class OakTree{
public:
    OakTree() = default;

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
    inline OakTree_PvtBinary *rebind_ptr( GameObj &goRef_ ){

        tprAssert( goRef_.species == OakTree::specId );
        return reinterpret_cast<OakTree_PvtBinary*>(goRef_.get_pvtBinaryPtr());
    }


    //======== static ========//
    static  u32_t  specId; //- 在 onGoSpecIds_SignUp() 中手动设置...


private:

    //--- callback ---//
    void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};

//---------- static ----------//
inline u32_t  OakTree::specId {0}; //- 具体值在 goSpecIds.cpp 中分配

//=====< 本类 唯一的 保留实例 >=====
inline OakTree  oakTree {};


}//------------- namespace gameObjs: end ----------------
#endif 


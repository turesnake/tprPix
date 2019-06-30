/*
 * ========================= BigMan.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   诺曼人
 * 
 * ----------------------------
 */
#ifndef TPR_BIG_MAN_H
#define TPR_BIG_MAN_H

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
inline std::vector<PubBinaryValType> bigMan_pubBinaryValTypes {
    PubBinaryValType::HP,
    PubBinaryValType::MP
};

struct BigMan_PvtBinary{
    int   tmp {};
};


class BigMan{
public:
    BigMan() = default;

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
    inline BigMan_PvtBinary *rebind_ptr( GameObj &goRef_ ){

        tprAssert( goRef_.species == BigMan::specId );
        return reinterpret_cast<BigMan_PvtBinary*>(goRef_.get_pvtBinaryPtr());
    }

    //======== static ========//
    static  u32_t  specId; //- 在 onGoSpecIds_SignUp() 中手动设置...

private:

    //--- callback ---//
    void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );


};

//---------- static ----------//
inline u32_t  BigMan::specId {0}; //- 具体值在 goSpecIds.cpp 中分配

//=====< 本类 唯一的 保留实例 >=====
inline BigMan  big_man {};

}//------------- namespace gameObjs: end ----------------
#endif 


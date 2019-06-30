/*
 * ========================= Crab.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   螃蟹
 * -----------------------
 */
#ifndef TPR_CRAB_H
#define TPR_CRAB_H

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

#include "AI_Crab.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


//-- 定义了 go.binary 的数据格式 --
inline std::vector<PubBinaryValType> crab_pubBinaryValTypes {
    PubBinaryValType::HP,
    PubBinaryValType::MP
};

struct Crab_PvtBinary{
    int      tmpVal  {};
    AI_Crab  ai_crab {};
};


class Crab{
public:
    Crab() = default;

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
    inline Crab_PvtBinary *rebind_ptr( GameObj &goRef_ ){

        tprAssert( goRef_.species == Crab::specId );
        return reinterpret_cast<Crab_PvtBinary*>(goRef_.get_pvtBinaryPtr());
    }

    //======== static ========//
    static  goSpecId_t  specId; //- 在 onGoSpecIds_SignUp() 中手动设置...


private:

    //--- callback ---//
    void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );


};

//---------- static ----------//
inline goSpecId_t  Crab::specId {0}; //- 具体值在 goSpecIds.cpp 中分配

//=====< 本类 唯一的 保留实例 >=====
inline Crab  crab {};

}//------------- namespace gameObjs: end ----------------
#endif 


/*
 * ========================= Norman.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   诺曼人
 * ----------------------------
 */
#ifndef TPR_NORMAN_H
#define TPR_NORMAN_H

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
inline std::vector<PubBinaryValType> norman_pubBinaryValTypes {
    PubBinaryValType::HP,
    PubBinaryValType::MP
};

struct Norman_PvtBinary{
    int   tmp  {};
};


class Norman{
public:
    Norman() = default;

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
    inline Norman_PvtBinary *rebind_ptr( GameObj &goRef_ ){

        tprAssert( goRef_.species == Norman::specId );
        return reinterpret_cast<Norman_PvtBinary*>(goRef_.get_pvtBinaryPtr());
    }

    //======== tmp vals ========//
    //GameObj *goPtr {nullptr}; //- go实例指针，所有的操作都是为了 服务于它
                            //- 具象go类 被彻底 工厂化，它甚至不再存储 go实例。
                            //- 这大幅度降低了 具象go类实例 创建的成本
                            //（多数时间作为 临时对象，创建在一个 函数内）

    //Norman_PvtBinary  *pvtBp {nullptr}; //- 指向 goPtr->binary 
                            //- 通过这个指针来 简化调用
                            //  由于 具象go类实例的 生命周期很短（通常活不过一个函数）
                            //  所以，这个指针也是临时的

    //======== static ========//
    static  goSpecId_t  specId; //- 在 onGoSpecIds_SignUp() 中手动设置...


private:

    //--- callback ---//
    void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};

//---------- static ----------//
inline goSpecId_t  Norman::specId {0}; //- 具体值在 goSpecIds.cpp 中分配

//=====< 本类 唯一的 保留实例 >=====
inline Norman  norman {};

}//------------- namespace gameObjs: end ----------------
#endif 


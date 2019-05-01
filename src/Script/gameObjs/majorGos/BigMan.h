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
#ifndef _TPR_BIG_MAN_H_
#define _TPR_BIG_MAN_H_

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "GameObjMesh.h"
#include "AnimFrameSet.h"
#include "PubBinaryValType.h"
#include "Altitude.h"
#include "Density.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


//-- 定义了 go.binary 的数据格式 --
inline std::vector<PubBinaryValType> bigMan_pubBinaryValTypes {
    PubBinaryValType::HP,
    PubBinaryValType::MP
};

struct BigMan_PvtBinary{
    int   tmp;
};


class BigMan{
public:
    BigMan() = default;

    //--- 延迟init ---//
    void init_in_autoMod(   GameObj *_goPtr,
                            const IntVec2 &_mpos,
					        float _fieldWeight,
					        const Altitude &_alti,
					        const Density &_density );
                            
    void bind( GameObj *_goPtr );

    //--- 从硬盘读取到 go实例数据后，重bind callback
    void rebind( GameObj *_goPtr );

    //--- callback ---//
    void OnRenderUpdate( GameObj *_goPtr ); 
    void OnLogicUpdate( GameObj *_goPtr ); 

    //--  每次调用回调函数，都需要做的 指针重绑定 --
    inline void rebind_ptr( GameObj *_goPtr ){
        assert( _goPtr->species == BigMan::specId );
        //-- rebind ptr -----
        goPtr = _goPtr;
        pvtBp = (BigMan_PvtBinary*)goPtr->get_pvtBinaryPtr();
    }

    //======== tmp vals ========//
    GameObj *goPtr {nullptr}; //- go实例指针，所有的操作都是为了 服务于它
                            //- 具象go类 被彻底 工厂化，它甚至不再存储 go实例。
                            //- 这大幅度降低了 具象go类实例 创建的成本
                            //（多数时间作为 临时对象，创建在一个 函数内）

    BigMan_PvtBinary  *pvtBp {nullptr}; //- 指向 goPtr->binary 
                            //- 通过这个指针来 简化调用
                            //  由于 具象go类实例的 生命周期很短（通常活不过一个函数）
                            //  所以，这个指针也是临时的

    //======== static ========//
    static  u32_t  specId; //- 在 onGoSpecIds_SignUp() 中手动设置...

private:

    //--- callback ---//
    void OnActionSwitch( GameObj *_goPtr, ActionSwitchType _type );


};

//---------- static ----------//
inline u32_t  BigMan::specId {0}; //- 具体值在 goSpecIds.cpp 中分配

//=====< 本类 唯一的 保留实例 >=====
inline BigMan  big_man {};

}//------------- namespace gameObjs: end ----------------
#endif 


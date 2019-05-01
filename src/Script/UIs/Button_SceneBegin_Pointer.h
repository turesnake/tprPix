/*
 * ============= Button_SceneBegin_Pointer.h ================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_BUTTON_SCENE_BEGIN_POINTER_H_
#define _TPR_BUTTON_SCENE_BEGIN_POINTER_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "GameObjMesh.h"
#include "AnimFrameSet.h"


namespace uis{//------------- namespace uis ----------------



class Button_SceneBegin_Pointer{
public:
    Button_SceneBegin_Pointer() = default;

    //--- 延迟init ---//
    void init_in_autoMod(   GameObj *_goPtr,
                            const IntVec2 &_mpos );

    void bind( GameObj *_goPtr );

    //--- 从硬盘读取到 go实例数据后，重bind callback
    void rebind( GameObj *_goPtr );

    //--- callback ---//
    void OnRenderUpdate( GameObj *_goPtr ); 
    void OnLogicUpdate( GameObj *_goPtr ); 


    //--  每次调用回调函数，都需要做的 指针重绑定 --
    inline void rebind_ptr( GameObj *_goPtr ){
        assert( _goPtr->species == Button_SceneBegin_Pointer::specId );
        //-- rebind ptr -----
        goPtr = _goPtr;
    }

    //======== tmp vals ========//
    GameObj *goPtr {nullptr}; //- go实例指针，所有的操作都是为了 服务于它
                            //- 具象go类 被彻底 工厂化，它甚至不再存储 go实例。
                            //- 这大幅度降低了 具象go类实例 创建的成本
                            //（多数时间作为 临时对象，创建在一个 函数内）

    //======== static ========//
    static  goSpecId_t  specId; //- 在 onGoSpecIds_SignUp() 中手动设置...


private:

    //--- callback ---//
    void OnActionSwitch( GameObj *_goPtr, ActionSwitchType _type );
};

//---------- static ----------//
inline goSpecId_t  Button_SceneBegin_Pointer::specId {0}; //- 具体值在 uiSpecIds.cpp 中分配

//=====< 本类 唯一的 保留实例 >=====
inline Button_SceneBegin_Pointer  button_sceneBegin_pointer {};

}//------------- namespace uis: end ----------------
#endif 


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
#include "UIObj.h"
#include "UIMesh.h"
#include "AnimFrameSet.h"


namespace uis{//------------- namespace uis ----------------



class Button_SceneBegin_Pointer{
public:
    Button_SceneBegin_Pointer() = default;

    //--- 延迟init ---//
    void init_in_autoMod(   UIObj  *_uiObjPtr,
                            const glm::vec2 &_fpos );

    //--- callback ---//
    // 暂无...
    
    //--  每次调用回调函数，都需要做的 指针重绑定 --
    inline void rebind_ptr( UIObj *_uiObjPtr ){
        assert( _uiObjPtr->uiObjSpecId == Button_SceneBegin_Pointer::specId );
        //-- rebind ptr -----
        this->uiObjPtr = _uiObjPtr;
    }

    //======== tmp vals ========//
    UIObj *uiObjPtr {nullptr}; 

    //======== static ========//
    static  uiObjSpecId_t  specId; //- 在 onGoSpecIds_SignUp() 中手动设置...

private:
};

//---------- static ----------//
inline uiObjSpecId_t  Button_SceneBegin_Pointer::specId {0}; //- 具体值在 uiSpecIds.cpp 中分配

//=====< 本类 唯一的 保留实例 >=====
inline Button_SceneBegin_Pointer  button_sceneBegin_pointer {};

}//------------- namespace uis: end ----------------
#endif 


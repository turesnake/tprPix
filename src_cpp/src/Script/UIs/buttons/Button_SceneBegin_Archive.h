/*
 * ============= Button_SceneBegin_Archive.h ================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_BUTTON_SCENE_BEGIN_ARCHIVE_H_
#define TPR_BUTTON_SCENE_BEGIN_ARCHIVE_H_


//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "UIObj.h"
#include "UIMesh.h"
#include "AnimFrameSet.h"


namespace uis{//------------- namespace uis ----------------


class Button_SceneBegin_Archive{
public:
    Button_SceneBegin_Archive() = default;

    //--- 延迟init ---//
    void init_in_autoMod(   UIObj *uiObjPtr_,
                            const glm::vec2 &fpos_ );

    //--- callback ---//
    // 暂无...
    
    //--  每次调用回调函数，都需要做的 指针重绑定 --
    inline void rebind_ptr( UIObj *uiObjPtr_ ){
        tprAssert( uiObjPtr_->uiObjSpecId == Button_SceneBegin_Archive::specId );
        //-- rebind ptr -----
        this->uiObjPtr = uiObjPtr_;
    }

    //======== tmp vals ========//
    UIObj *uiObjPtr {nullptr}; 

    //======== static ========//
    static  uiObjSpecId_t  specId; //- 在 onGoSpecIds_SignUp() 中手动设置...

private:
};

//---------- static ----------//
inline uiObjSpecId_t  Button_SceneBegin_Archive::specId {0}; //- 具体值在 uiSpecIds.cpp 中分配

//=====< 本类 唯一的 保留实例 >=====
inline Button_SceneBegin_Archive  button_sceneBegin_archive {};

}//------------- namespace uis: end ----------------
#endif 


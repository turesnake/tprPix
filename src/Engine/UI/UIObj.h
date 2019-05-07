/*
 * ========================= UIObj.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.05.04
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_UI_OBJ_H_
#define _TPR_UI_OBJ_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- Engine --------------------//
#include "ID_Manager.h"
#include "UIMesh.h"
#include "UIObjType.h"


class UIObj{
public:
    UIObj() = default;

    void init();//-- MUST --

    void renderUpdate();

    UIMesh &creat_new_uiMesh(   //const std::string &_animFrameSetName,
                                    ShaderProgram     *_pixShaderPtr,
                                    ShaderProgram     *_shadowShaderPtr,
                                    float             _off_z,
                                    bool              _isVisible,
                                    bool              _isFlipOver );

    //===== set =====//
    //-- 将一个 uiObj 跳置到一个 fpos
    inline void skip_to_fpos( const glm::vec2 &_newFPos ){
        this->currentFPos = _newFPos;
        this->targetFPos = _newFPos;
        this->isMoving = false;
    }
    //-- 设置 targetFPos，让 uiObj 自己移动过去
    inline void drag_to_fpos( const glm::vec2 &_FPos ){
        if( this->currentFPos == _FPos ){
            return;
        }
        this->targetFPos = _FPos;
        this->isMoving = true;
    }

    
    //===== get =====//
    inline const glm::vec2 &get_currentFPos() const {
        return this->currentFPos;
    }

    //-- 临时措施 --
    inline UIMesh &get_uiMesh(){
        return this->uiMesh;
    }

    //===== vals =====//
    uiObjId_t     uiObjId      {NULLID};
    uiObjSpecId_t uiObjSpecId  {0};
    UIMesh        uiMesh  {};
    
    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 go_id ( 在.cpp文件中初始化 )

private:

    void renderUpdate_move();


    glm::vec2    currentFPos  {}; //-- 目前就是普通的 世界坐标，
                            //  在未来，要实现为 围绕 window 衡量的 坐标
    glm::vec2    targetFPos  {}; 

    //===== flags =====//
    bool   isMoving       {true}; //- 是否在移动


};

//============== static ===============//
inline ID_Manager  UIObj::id_manager { ID_TYPE::U32, 1};


#endif 


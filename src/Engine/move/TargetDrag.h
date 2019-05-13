/*
 * ========================= TargetDrag.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    特别适合 UI实例 的 位移运动: 目标pos拖动（联想下 camera）
 * ----------------------------
 */
#ifndef _TPR_TARGET_DRAG_H_
#define _TPR_TARGET_DRAG_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4


//class GameObj; //- tmp
class Move;
class GameObjPos;


/*
class TargetDrag{
public:
    TargetDrag() = default;

    void init(  Move *_movePtr, 
                GameObjPos *_goPosPtr );


    inline void set_targetFPos( const glm::vec2 &_FPos ){
        if( _FPos == this->targetFPos ){
            return;
        }
        this->targetFPos = _FPos;
        this->isMoving = true;
    }

    inline bool is_moving() const {
        return this->isMoving;
    }


    void RenderUpdate(); 


private:
    Move        *movePtr  {nullptr}; //- 每个 crawl实例 都属于一个 move实例, 强关联
    GameObjPos  *goPosPtr {nullptr}; 


    glm::vec2 targetFPos  {}; 

    //===== flags =====//
    bool   isMoving       {true}; //- 是否在移动
};
*/


#endif 


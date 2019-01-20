/*
 * ========================= GameObjPos.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.20
 *                                        修改 -- 2019.01.20
 * ----------------------------------------------------------
 *    
 * ----------------------------
 */
#ifndef _TPR_GAME_OBJ_POS_H_
#define _TPR_GAME_OBJ_POS_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptrs

//-------------------- Engine --------------------//
#include "config.h" 
#include "PixVec.h"
#include "MapCoord.h"
#include "NineBox.h"


//--- need ---//
class GameObj;

//-- based on go.rootAnchor 
class GameObjPos{
public:

    inline void init( GameObj *_goPtr ){
        goPtr = _goPtr;
        //---
        currentFPos = glm::vec2{ 0.0f, 0.0f };
        currentMCPos.clear_all();
    }
    
    //---- set ----//

    //-- 若要在map上“放置”go实例，请用本函数 
    void init_by_currentMCPos( const MapCoord &_mcpos );

    //-- 直接改写 currentFPos 是很罕见的（也是不推荐的）
    //   为了确保 go 与 mapent坐标 的对齐。不要随意使用本函数 ！！！
    inline void set_by_currentFPos( const glm::vec2 &_fpos ){
        currentFPos = _fpos;
        refresh_currentMCPos();
    }

    //-- 对 currentFPos 进行 累加累减 --
    inline void accum_currentFPos( const glm::vec2 &_fpos ){
        currentFPos += _fpos;
        refresh_currentMCPos();
    }
    inline void accum_currentFPos( float _x, float _y ){
        currentFPos.x += _x;
        currentFPos.y += _y;
        refresh_currentMCPos();
    }


    //---- get ----//
    inline const glm::vec2 &get_currentFPos() const {
        return currentFPos;
    }
    inline const MapCoord &get_currentMCPos() const {
        return currentMCPos;
    }


private:
    glm::vec2  currentFPos  {};  //- go.rootAnchor 当前 fpos，不一定对齐与mapent
    MapCoord   currentMCPos {}; 

    GameObj     *goPtr    {nullptr}; 

    //--- funcs ----//
    //-- 通过 currentFPos，计算出 currentMCPos
    inline void refresh_currentMCPos(){
        currentMCPos.set_by_mpos(   ((int)currentFPos.x)/PIXES_PER_MAPENT, 
                                    ((int)currentFPos.y)/PIXES_PER_MAPENT );
    }

};




#endif 


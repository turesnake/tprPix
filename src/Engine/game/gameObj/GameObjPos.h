/*
 * ========================= GameObjPos.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.20
 *                                        MODIFY -- 
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
//#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
//#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptrs

//-------------------- Engine --------------------//
#include "config.h" 
#include "IntVec.h"
#include "MapCoord.h"
#include "MapEntCompass.h"

//--- need ---//
class GameObj;


//-- based on go.rootAnchor 
class GameObjPos{
public:

    inline void init( GameObj *_goPtr ){
        this->goPtr = _goPtr;
        //---
        this->currentFPos = glm::vec2{ 0.0f, 0.0f };
        this->currentMCPos.clear_all();
    }
    
    //---- set ----//
    //-- 若要在map上“放置”go实例，请用本函数 
    void init_by_currentMPos( const IntVec2 &_mpos );

    //-- 直接改写 currentFPos 是很罕见的（也是不推荐的）
    //   为了确保 go 与 mapent坐标 的对齐。不要随意使用本函数 ！！！
    inline void set_by_currentFPos( const glm::vec2 &_fpos ){
        this->currentFPos = _fpos;
        this->currentMCPos = fpos_2_mcpos( calc_rootAnchor_midFPos() );
    }

    //-- 对 currentFPos 进行 累加累减 --
    inline void accum_currentFPos( const glm::vec2 &_fpos ){
        this->currentFPos += _fpos;
        this->currentMCPos = fpos_2_mcpos( calc_rootAnchor_midFPos() );
    }
    inline void accum_currentFPos( float _x, float _y ){
        this->currentFPos.x += _x;
        this->currentFPos.y += _y;
        this->currentMCPos = fpos_2_mcpos( calc_rootAnchor_midFPos() );
    }

    void align_currentFPos_by_currentMCPos();

    inline void set_alti( float _alti ){
        this->alti = _alti;
    }

    //---- get ----//
    inline const glm::vec2 &get_currentFPos() const {
        return this->currentFPos;
    }

    //-- 注意，这个函数返回的是 currentFPos 的整形值（将丢失小数）
    //  只可用于：明确知道 当前 rootAnchor所在 collient 已对齐于 mapent 时。
    //  比如在 crawl 节点帧
    inline const IntVec2 get_currentPPos() const {
        return IntVec2{ static_cast<int>(this->currentFPos.x), 
                        static_cast<int>(this->currentFPos.y) };
    }

    inline const MapCoord &get_currentMCPos() const {
        return this->currentMCPos;
    }
    inline const IntVec2 &get_currentMPos() const {
        return this->currentMCPos.get_mpos();
    }

    //- 获得 rootAnchor 所在的 collient 的 midFPos 
    glm::vec2 calc_rootAnchor_midFPos();

    inline float get_alti() const {
        return this->alti;
    }


private:
    GameObj     *goPtr    {nullptr}; 
    //---

    glm::vec2   currentFPos  {};  //- 基于 go.rootAnchor 的， 当前 fpos，无需对齐与mapent
    MapCoord    currentMCPos {};  //- rootAnchor所在的 collient 的中点， 当前所在的 mapent
                                  //  很多 rootAnchor 都不在 mapent的中心，所以无法直接代表 mapent的位置
                                  //- 此值往往在 crawl的 回合中段 发生切换。
    
    float       alti         {0.0f}; //- 腾空高度。
                
    //-- 这个值暂时 没被使用 --
    //glm::vec2   targetFPos  { 0.0f, 0.0f };  //- 基于 go.rootAnchor 的，目标 fpos，无需对齐与mapent
                                //- 此值的用途很多样，有待开发...
                                //  目前版本中，主要用于 crawl 节点帧 对齐。
                                //  用来记录 新回合的 最终 位移绝对地址
    
};


#endif 


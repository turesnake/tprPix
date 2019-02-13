/*
 * ========================= InputINS.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#ifndef _TPR_INPUT_INS_H_
#define _TPR_INPUT_INS_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2


//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "GameKey.h"


//-- 控制 所有 crawl模式的 go 的 按键指令集
//  - player_go 通过 按键输入
//  - npc_go    通过 代码输入
class InputINS{
public:

    //-- 并不清空 dir --
    inline void clear_allKeys(){
        keys   = 0;
        crossX = 0;
        crossY = 0;
    }

    inline void set_key( const GameKey &_key ){
        switch( _key ){
            case GameKey::LEFT:   crossX = -1;  break;
            case GameKey::RIGHT:  crossX = 1;   break;
            case GameKey::UP:     crossY = 1;   break;
            case GameKey::DOWN:   crossY = -1;  break;
            default: //- oth keys
                keys = keys | (1<<(int)_key);
                break;
        }
    }

    inline bool check_keyBit( const GameKey &_key ) const {
        int idx = (int)_key;
        assert( (idx>=0) && (idx<=16) );
        return (((keys>>idx) & 1)==1);
    }


    //======== vals ========//
    //  确保对齐，提高传输效率
    //  尽可能缩小尺寸。确保 值传递也能高效
    glm::vec2 dir   { 0.0f, 0.0f }; //-  go 单位方向向量(暂由鼠标实现)
    u16       keys     {0};    //- 16个功能按键， bit-map
    char      crossX   {0};    //- 十字键 [-1,0,1]
    char      crossY   {0};
};



#endif 


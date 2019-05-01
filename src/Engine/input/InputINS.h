/*
 * ========================= InputINS.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   游戏按键指令 
 *  -----
 *  物理鼠键 -> inputINS -> GameObjINS
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


//-- 存储 游戏识别的所有 按键信息，
//   但这里的 按键并不需要全部传输给 go，
//   比如 “打开状态面板” 这种按键，就会被 分发给对应的模块
//-- 不应将 此数据 直接传递给 go，而应该转换为  游戏动作指令 
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

    inline bool check_key( const GameKey &_key ) const {
        size_t idx = (size_t)_key;
        if(  idx < 16 ){
            return (((keys>>idx) & 1)==1);
        }else{
            switch( _key ){
                case GameKey::LEFT:  return (crossX == -1);
                case GameKey::RIGHT: return (crossX == 1);
                case GameKey::UP:    return (crossY == 1);
                case GameKey::DOWN:  return (crossY == -1);
                default:
                    assert(0);
                    return false;
            }
        }
    }

    //======== vals ========//
    //  确保对齐，提高传输效率
    //  尽可能缩小尺寸。确保 值传递也能高效
    glm::vec2 dir   { 0.0f, 0.0f }; //-  go 单位方向向量(暂由鼠标实现)
    u16_t     keys     {0};    //- 16个功能按键， bit-map
    char      crossX   {0};    //- 十字键 [-1,0,1]
    char      crossY   {0};
};



#endif 


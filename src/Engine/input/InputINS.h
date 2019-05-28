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

//--- glm - 0.9.9.5 ---
#include <glm/glm.hpp>
            //-- glm::vec2

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "GameKey.h"
#include "DirAxes.h"


//-- 存储 游戏识别的所有 按键信息，
//   但这里的 按键并不需要全部传输给 go，
//   比如 “打开状态面板” 这种按键，就会被 分发给对应的模块
//-- 不应将 此数据 直接传递给 go，而应该转换为  游戏动作指令 
class InputINS{
public:

    //-- 并不清空 dir --
    inline void clear_allKeys(){
        keys   = 0;
        dirAxes.clear_all();
    }

    inline void set_key_from_keyboard( const GameKey &_key ){
        switch( _key ){
            case GameKey::LEFT:   dirAxes.set_x( -1.0 );  break;
            case GameKey::RIGHT:  dirAxes.set_x(  1.0 );  break;
            case GameKey::UP:     dirAxes.set_y(  1.0 );  break;
            case GameKey::DOWN:   dirAxes.set_y( -1.0 );  break;
            default: //- oth keys
                keys = keys | (1<<(int)_key);
                break;
        }
    }

    inline void set_key_from_joystick( const GameKey &_key ){
        switch( _key ){
            //-- 暂时不允许 设置 方向键 button 
            case GameKey::LEFT:   assert(0);
            case GameKey::RIGHT:  assert(0);
            case GameKey::UP:     assert(0);
            case GameKey::DOWN:   assert(0);
            default: //- oth keys
                keys = keys | (1<<(int)_key);
                break;
        }
    }


    inline void set_dirAxes_from_joystick( float _x, float _y ){
        this->dirAxes.set(_x, _y);
    }

    inline void limit_dirAxes(){
        this->dirAxes.limit_vals();
    }

    //-- 目前被 sceneBegin 使用 --
    inline bool check_key( const GameKey &_key ) const {
        size_t idx = static_cast<size_t>(_key);
        if(  idx < 32 ){
            return (((keys>>idx) & 1)==1);
        }else{
            switch( _key ){
                case GameKey::LEFT:  return (dirAxes.get_x() < -0.0);
                case GameKey::RIGHT: return (dirAxes.get_x() >  0.0);
                case GameKey::UP:    return (dirAxes.get_y() >  0.0);
                case GameKey::DOWN:  return (dirAxes.get_y() < -0.0);
                default:
                    assert(0);
                    return false;
            }
        }
    }

    inline const DirAxes &get_dirAxes() const {
        return this->dirAxes;
    }
    

    
private:
    //======== vals ========//
    //  确保对齐，提高传输效率
    //  尽可能缩小尺寸。确保 值传递也能高效
    //glm::vec2  dir   { 0.0f, 0.0f }; //-  go 单位方向向量(暂由鼠标实现)
    u32_t      keys     {0};    //- 32个功能按键， bit-map

    DirAxes    dirAxes   {}; //- 方向键数据 [-1.0, 1.0]
};



#endif 


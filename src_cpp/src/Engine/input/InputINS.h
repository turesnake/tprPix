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
#ifndef TPR_INPUT_INS_H_
#define TPR_INPUT_INS_H_

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "tprAssert.h"
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

    inline void set_key_from_keyboard( const GameKey &key_ ){
        switch( key_ ){
            case GameKey::LEFT:   dirAxes.set_x( -1.0f );  break;
            case GameKey::RIGHT:  dirAxes.set_x(  1.0f );  break;
            case GameKey::UP:     dirAxes.set_y(  1.0f );  break;
            case GameKey::DOWN:   dirAxes.set_y( -1.0f );  break;
            default: //- oth keys
                keys = keys | (1<<(int)key_);
                break;
        }
    }

    inline void set_key_from_joystick( const GameKey &key_ ){
        switch( key_ ){
            //-- 暂时不允许 设置 方向键 button 
            case GameKey::LEFT:   tprAssert(0); break;
            case GameKey::RIGHT:  tprAssert(0); break;
            case GameKey::UP:     tprAssert(0); break;
            case GameKey::DOWN:   tprAssert(0); break;
            default: //- oth keys
                keys = keys | (1<<(int)key_);
                break;
        }
    }


    inline void set_dirAxes_from_joystick( float x_, float y_ ){
        this->dirAxes.set(x_, y_);
    }

    inline void limit_dirAxes(){
        this->dirAxes.limit_vals();
    }

    //-- 目前被 sceneBegin 使用 --
    inline bool check_key( const GameKey &key_ ) const {
        size_t idx = static_cast<size_t>(key_);
        if(  idx < 32 ){
            return (((keys>>idx) & 1)==1);
        }else{
            switch( key_ ){
                case GameKey::LEFT:  return (dirAxes.get_x() < -0.0f);
                case GameKey::RIGHT: return (dirAxes.get_x() >  0.0f);
                case GameKey::UP:    return (dirAxes.get_y() >  0.0f);
                case GameKey::DOWN:  return (dirAxes.get_y() < -0.0f);
                default:
                    tprAssert(0);
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


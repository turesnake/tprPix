/*
 * ========================= InputINS.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   游戏按键指令 
 *  -----
 *  物理鼠键 -> inputINS
 * ----------------------------
 */
#ifndef TPR_INPUT_INS_H
#define TPR_INPUT_INS_H

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
    inline void clear_allKeys()noexcept{
        this->keys   = 0;
        this->dirAxes.clear_all();
        this->tmpDir.x = 0.0;
        this->tmpDir.y = 0.0;
    }


    inline void set_key_from_keyboard( GameKey key_ )noexcept{
        // 并不直接写入 dirAxes，而是先记录在 临时变量上
        // 最后阶段统一 设置 dirAxes
        switch( key_ ){
            case GameKey::LEFT:   this->tmpDir.x = -1.0;  break;
            case GameKey::RIGHT:  this->tmpDir.x =  1.0;  break;
            case GameKey::UP:     this->tmpDir.y =  1.0;  break;
            case GameKey::DOWN:   this->tmpDir.y = -1.0;  break;
            default: //- oth keys
                keys = keys | (1<<(int)key_);
                break;
        }
    }


    inline void set_key_from_joystick( GameKey key_ )noexcept{
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

    // 并不直接写入 dirAxes，而是先记录在 临时变量上
    // 最后阶段统一 设置 dirAxes
    inline void collect_dirAxes_from_joystick( double x_, double y_ )noexcept{
        this->tmpDir.x = x_;
        this->tmpDir.y = y_;
    }

    // 将数据从 tmpDir 同步到 dirAxes
    inline void sync_dirAxes()noexcept{
        this->dirAxes.set( this->tmpDir );
    }
    

    //-- 目前被 sceneBegin 使用 --
    inline bool check_key( GameKey key_ ) const noexcept{
        size_t idx = gameKey_2_size_t(key_);
        if(  idx < 32 ){
            return (((keys>>idx) & 1)==1);
        }else{
            switch( key_ ){
                case GameKey::LEFT:  return (dirAxes.get_x() < -0.0);
                case GameKey::RIGHT: return (dirAxes.get_x() >  0.0);
                case GameKey::UP:    return (dirAxes.get_y() >  0.0);
                case GameKey::DOWN:  return (dirAxes.get_y() < -0.0);
                default:
                    tprAssert(0);
                    return false;
            }
        }
    }

    inline const DirAxes &get_dirAxes() const noexcept{
        return this->dirAxes;
    }
    

private:
    //======== vals ========//
    u32_t      keys     {0};    //- 32个功能按键， bit-map
    DirAxes    dirAxes   {};    //- 方向键数据 [-1.0, 1.0]

    //--- tmp vals ---//
    glm::dvec2  tmpDir {0.0, 0.0};
};



#endif 


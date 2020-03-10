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

//------------------- CPP --------------------//
#include <cstdint> // uint8_t


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
    }


    inline void set_key( GameKey key_ )noexcept{
        keys = keys | (1<<static_cast<int>(key_));
    }

    
    inline void collect_dirAxes_from_joystick( const glm::dvec2 &val_ )noexcept{
        //this->dirAxes.set( val_ );
        this->dirAxes = DirAxes{ val_ };
    }

    inline bool get_key( GameKey key_ )const noexcept{
        int idx = static_cast<int>(key_);
        return ( ((keys>>idx) & 1) == 1 );
    }

    inline const DirAxes &get_dirAxes()const noexcept{
        return this->dirAxes;
    }


    inline bool is_dir_up()const noexcept{
        return (this->dirAxes.get_originVal().y > 0.7);
    }
    inline bool is_dir_down()const noexcept{
        return (this->dirAxes.get_originVal().y < -0.7);
    }
    inline bool is_dir_left()const noexcept{
        return (this->dirAxes.get_originVal().x < -0.7);
    }
    inline bool is_dir_right()const noexcept{
        return (this->dirAxes.get_originVal().x > 0.7);
    }


private:
    //======== vals ========//
    uint32_t   keys     {0};    //- 15个功能按键， bit-map, 实际可存储 32 个
    DirAxes    dirAxes   {};    //- 方向键数据 [-1.0, 1.0]
};



#endif 


/*
 * ================== GameWorldCoord.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *    游戏世界坐标系
 *    游戏中的 主坐标系。
 *    每个 gameobj，都以此为 基础去完成 逻辑运算
 *    通过它，再转换为 gl世界坐标系，然后完成图形渲染。
 *  ------
 *    当前版本的 坐标系，是 以 “3*3像素 = 1个地面单位” 为标准 去换算的
 * ----------------------------
 */
#ifndef _TPR_GAME_WORLD_COORD_H_
#define _TPR_GAME_WORLD_COORD_H_


//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//------------------- Libs --------------------//
#include "tprDataType.h" 



class GameWorldCoord{

public:
    explicit GameWorldCoord( float _x,float _y )
        :
        coord({_x,_y})
        {}

    //-- 获得 gl 世界坐标值 --
    //-- 目前的换算规则是：
    //      3*3像素 = 1个地面单位
    inline glm::vec2 to_glWorldCoord() const {
        return (coord * 3.0f);
    }

    //-- 两个 gwc 的 整形部分是否相同 --
    inline bool is_equal_in_int( const GameWorldCoord &_gwc ){

        if( ((i64)coord.x==(i64)_gwc.coord.x) &&  
            ((i64)coord.y==(i64)_gwc.coord.y) ){
            return true;
        }else{
            return false;
        }
    }

    //---- vals -----
    glm::vec2  coord; //- 坐标数据本身

private:
    
};

#endif


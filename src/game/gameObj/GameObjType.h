/*
 * ===================== GameObjType.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.11
 *                                        修改 -- 2018.12.11
 * ----------------------------------------------------------
 *    GameObj 各种数据 结构
 * ----------------------------
 */
#ifndef _TPR_GAME_OBJ_TYPE_H_
#define _TPR_GAME_OBJ_TYPE_H_

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
            //-- glm::value_ptr

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 



//-- go 数据 [硬盘态] ---
struct diskGameObj{

    u64  id;
    u32  type; //- GameObjType --> u32
    

};


//-- 一种 go 需要的全部参数 ---
struct GameObjParams{
	std::string  type; //- go type_name
    //std::string  path;
    //...

}; 


//-- go move state／运动状态 --
enum class GameObjMoveState{

    AbsFixed  = 1, //- 固定于地面，绝对静止
    BeMovable = 2, //- 静止，但可被移动（通过外部 forse 施加的影响）
    Movable   = 3  //- 可移动。本go 可启动移动操作。
};


//-- go state / go 常规状态 --
enum class GameObjState{

    Sleep = 1, //- 休眠状态，不主动行动，（但可被动回应）
    Waked = 2  //- 活跃状态，主动发起行动。 
};


//-- go type -- 
enum class GameObjType : u32 {

    NullType = 0, //- 保留，表示 空类型 

    //-------  ----------//
    




    //------- tree ----------//
    Tree_Pine = 1000,  //- 松树
    Tree_Oak  = 1001,  //- 橡树




    //------- end ----------//
    End = (u32)(-1) //- tmp, 不会被真的用到 
};








#endif


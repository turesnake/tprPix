/*
 * ========================= GameObj.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *    GameObj 是游戏中的 一等公民。
 *    可以作为一个 独立的单位，存在于 游戏中
 * ----------------------------
 */
#ifndef _TPR_GAME_OBJ_H_
#define _TPR_GAME_OBJ_H_

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

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- SELF --------------------//
#include "GameObjtype.h" 
#include "Mesh.h" 
#include "ID_Manager.h" 
 

class GameObj{
public:

    explicit GameObj( const std::string &_typename )
        //:
        //type(_typename)
        {}


    void init();


    void  d2m( diskGameObj *_dgo );
    diskGameObj  m2d();



    //----------------- vals ---------------//
    u64          id;     //- go实例 在程序中的 主要搜索依据。
    GameObjType  type {GameObjType::NullType};   //- 代表类型的 字符串

    bool is_top_go {true}; //- 是否为 顶层 go (有些go只是 其他go 的一部分)
    u64   id_parent;    //- 不管是否为顶层go，都可以有自己的 父go。
                        //- 如果没有，此项写 NULLID


    GameObjMoveState  moveState {GameObjMoveState::BeMovable}; //- 运动状态
    GameObjState      state     {GameObjState::Sleep}; //- 常规状态

    PixVec2  targetPos {}; //- 目标pos，对齐与mapent 
    glm::vec2  currentPos {};  //- 当前帧 pos，float，不一定对齐与mapent

    glm::vec2  velocity; //- 运动速度

    float  weight; //- go重量 （影响自己是否会被 一个 force 推动）


    //------------ static ----------//
    static ID_Manager  id_manager; //- 负责生产 go_id

private:
    
    

    
    
    std::vector<Mesh> meshes;



};














#endif


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
#include <functional>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- SELF --------------------//
#include "GameObjType.h" 
#include "Mesh.h" 
#include "ID_Manager.h" 



//--- 最基础的 go 类，就像一个 "伪接口" ----//
// 并不用继承这个类，而是 包含它，
// 通过 class指针 动态绑定 go插件实例
// 通过 function / bind 动态绑定各种回调函数
// 在 主引擎中，只认识这个类。也只通过这个 类来 访问 一切 go实例
class GameObj{

    using F_V_V = std::function<void()>;

public:
    explicit GameObj(){}


    void init();

    //-- disl <-> mem --//
    void  d2m( diskGameObj *_dgo );
    diskGameObj  m2d();

    //-----------------//
    //-- 几个经典的回调函数，tmp...
    F_V_V  Awake {nullptr};  //- 初始化阶段执行的 内容
    F_V_V  Start {nullptr};  //- 游戏在进入 主循环之前，执行的内容
    F_V_V  Update {nullptr}; //- 每一游戏帧，被主程序调用


    //----------------- vals ---------------//
    goid_t         id      {NULLID};    //- go实例 在程序中的 主要搜索依据。
    GameObjSpecies species {GameObjSpecies::NullSpecies}; //- go类型
    GameObjFamily  family  {GameObjFamily::Major};        //- go 类群


    bool   is_top_go  {true}; //- 是否为 顶层 go (有些go只是 其他go 的一部分)
    goid_t id_parent {NULLID}; //- 不管是否为顶层go，都可以有自己的 父go。
                             //- 如果没有，此项写 NULLID

    //--- go 状态 ----//
    bool              is_active {false}; //- 是否进入激活圈. 
                                        //   未进入激活圈的go，不参与任何逻辑运算，也不会被渲染
    GameObjState      state     {GameObjState::Sleep};         //- 常规状态
    GameObjMoveState  moveState {GameObjMoveState::BeMovable}; //- 运动状态
    

    PixVec2  targetPos {}; //- 目标pos，对齐与mapent 
    glm::vec2  currentPos {};  //- 当前帧 pos，float，不一定对齐与mapent

    glm::vec2  velocity {}; //- 运动速度

    float  weight {0}; //- go重量 （影响自己是否会被 一个 force 推动）


    bool is_default {true}; //- 是否为 默认go
                            //- “默认go” 意味着这个 go没有被游戏改写过。
                            //- 当它跟着 mapSection 存入硬盘时，会被转换为 go_species 信息。
                            //- 以便少存储 一份 go实例，节省 硬盘空间。


    std::vector<Mesh> meshes;



    //------------ static ----------//
    static ID_Manager  id_manager; //- 负责生产 go_id ( 在.cpp文件中初始化 )

private:
};






#endif


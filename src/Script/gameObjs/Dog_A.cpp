/*
 * ========================= Dog_A.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.23
 *                                        修改 -- 2018.12.23
 * ----------------------------------------------------------
 *   具象GameObj类 示范。
 * 
 * ----------------------------
 */
#include "Script/gameObjs/Dog_A.h"


//-------------------- CPP --------------------//
#include <functional>

//-------------------- Engine --------------------//
#include "srcs_engine.h" 

//-------------------- Script --------------------//
#include "Script/actionHandler/Cycle.h"
#include "Script/resource/srcs_script.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------

/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 * -- 应该在 调用 init 之前，创建 基础go实例（并添加到 全局容器中）
 * -- 尽量把 具象go类 做得 “工厂化” 一些。
 */
void Dog_A::init( GameObj *_goPtr ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;

    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->Update   = std::bind( &Dog_A::Update, &dog_a );   
    goPtr->BeAffect = std::bind( &Dog_A::BeAffect, &dog_a ); 

    //-------- go self vals ---------//
    goPtr->species = ssrc::go_name_specIds.at("Dog_A");

    //-------- go.binary ---------//
    goPtr->binary.resize( sizeof(Dog_A_Binary) );
    binaryPtr = (Dog_A_Binary*)&(goPtr->binary[0]); //- 绑定到本地指针

    binaryPtr->HP = 100;
    binaryPtr->MP = 95;

    //-------- action／actionHandler/mesh ---------//
    goPtr->actionNames.push_back( "human_1" ); //- 待机动画

        //-- 制作唯一的 mesh 实例 --
        Mesh  mesh;
        mesh.set_shader_program( &esrc::rect_shader );
        mesh.init(); 
        //-- bind actionHandler --
        actionHdlr::cycle.bind( &mesh.actionHandlr, 4, 0, 3 );

    goPtr->meshs.push_back( mesh ); //- copy
}




/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Dog_A::bind( GameObj *_goPtr ){



}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Dog_A::rebind( GameObj *_goPtr ){



}




/* ===========================================================
 *                        Update
 * -----------------------------------------------------------
 */
void Dog_A::Update(){

}



/* ===========================================================
 *                       BeAffect
 * -----------------------------------------------------------
 */
void Dog_A::BeAffect(){

}







}//------------- namespace gameObjs: end ----------------


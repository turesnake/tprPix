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

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <functional>
//#include <iostream>
#include <string>

//-------------------- Engine --------------------//
#include "srcs_engine.h" 

//-------------------- Script --------------------//
#include "Script/actionHandle/Cycle.h"
#include "Script/resource/srcs_script.h" 

using namespace std::placeholders;

using std::string;
//using std::cout;
//using std::endl;


namespace gameObjs{//------------- namespace gameObjs ----------------


//---------- static ----------//
u32  Dog_A::specId {0};

//static void push_a_empty_mesh( GameObj *_goPtr );
static void creat_new_mesh( GameObj *_goPtr ); //- tmp
//static void creat_single_mesh( GameObj *_goPtr ); //- tmp

/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 * -- 应该在 调用 init 之前，创建 基础go实例（并添加到 全局容器中）
 * -- 尽量把 具象go类 做得 “工厂化” 一些。
 */
void Dog_A::init( GameObj *_goPtr ){

    assert( _goPtr != nullptr );

    GameObj *goPtr = _goPtr;

    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &Dog_A::RenderUpdate, &dog_a, _1 );   
    goPtr->LogicUpdate  = std::bind( &Dog_A::LogicUpdate,  &dog_a, _1 );
    goPtr->BeAffect     = std::bind( &Dog_A::BeAffect,     &dog_a, _1 ); 

    //-------- go self vals ---------//
    goPtr->species = Dog_A::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->is_top_go = true;
    goPtr->id_parent = NULLID;
    goPtr->is_active = true;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::Movable;
    goPtr->targetPos = PixVec2{ 0, 0 };
    goPtr->currentPos = glm::vec2{ 0.0f, 0.0f };
    goPtr->velocity = glm::vec2{ 0.0f, 0.0f };
    goPtr->weight = 5.0f;
    goPtr->is_dirty = false;

    //-------- action／actionHandle/mesh ---------//

            //-- 这一步意义不大，未来可能被 去除 --
            //  go实例 是否要存储自己的 actionNames ???
            //goPtr->actionNames.push_back( "human_1" ); 

        //-- 制作唯一的 mesh 实例 --
        //push_a_empty_mesh( goPtr );
            //goPtr->meshs.reserve(1);
        creat_new_mesh( goPtr );  
        //creat_single_mesh( goPtr );       


    //-------- go.binary ---------//
    Dog_A_Binary  *bp;
    goPtr->binary.resize( sizeof(Dog_A_Binary) );
    bp = (Dog_A_Binary*)&(goPtr->binary[0]); //- 绑定到本地指针

    bp->HP = 100;
    bp->MP = 95;
}

/*
void push_a_empty_mesh( GameObj *_goPtr ){

    Mesh mesh;
    mesh.is_visible = false;
    _goPtr->meshs.push_back( mesh ); //- 为空。占位用
}
*/



void creat_new_mesh( GameObj *_goPtr ){

    // ***| INSERT FIRST, INIT LATER  |***
        
        Mesh  tmp_mesh;
        _goPtr->meshs.push_back( tmp_mesh ); //- copy

            //     *** 巨型BUG ***
            //  只要添加此行，就正常运行
            //  只要删除此行，画面中的图元 就会消失
            //cout << "_goPtr->meshs.size() = " << _goPtr->meshs.size() << endl;
    

            Mesh  &rmesh = _goPtr->meshs.back(); //- 获得 尾部元素 的引用
            rmesh.set_shader_program( &esrc::rect_shader );
            rmesh.init(); 
            //-- bind actionHandle --
            actionHdle::ah_cycle.bind( &(rmesh.actionHandle), 4, 0, 6 );
            //-- bind texName --
            rmesh.bind_actionPtr( "human_1" );
            rmesh.refresh_texName();
            rmesh.pos = glm::vec2{ 0.0f, 0.0f }; //- 此mesh 在 go 中的 坐标偏移
}


/*
void creat_single_mesh( GameObj *_goPtr ){
 
            int tmp_tmp_1 = _goPtr->meshs.size();
            int tmp_tmp_2 = _goPtr->meshs.size();

            _goPtr->mesh.set_shader_program( &esrc::rect_shader );
            _goPtr->mesh.init(); 
            //-- bind actionHandle --
            actionHdle::ah_cycle.bind( &(_goPtr->mesh.actionHandle), 4, 0, 6 );
            //-- bind texName --
            _goPtr->mesh.bind_actionPtr( "human_1" );
            _goPtr->mesh.refresh_texName();
            _goPtr->mesh.pos = glm::vec2{ 0.0f, 0.0f }; //- 此mesh 在 go 中的 坐标偏移
}
*/



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
 *                      RenderUpdate
 * -----------------------------------------------------------
 */
void Dog_A::RenderUpdate( GameObj *_goPtr ){
    //cout << "Dog_A::RenderUpdate();" << endl;
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    assert( _goPtr->species == Dog_A::specId );
    //-- rebind ptr -----
    GameObj *goPtr = _goPtr;
    Dog_A_Binary  *bp = (Dog_A_Binary*)&(goPtr->binary[0]);
    //=====================================//


    
    //======= 渲染 一组 meshs ========//
    auto it = goPtr->meshs.begin();
    assert( it != goPtr->meshs.end() );
    for( ; it!=goPtr->meshs.end(); it++ ){
        //-- 调用每个 mesh.actionHandle.update()

        if( it->is_visible == false ){
            continue;
        }

        //=== 传参到 scriptBuf : [无参数] ===
        it->actionHandle.funcs.at("update")(&(it->actionHandle), 0);
        //=== 从 scriptBuf 取返回值 : [无返回值] ===

        //-- bind texName --
        it->refresh_texName();

        it->set_translate( goPtr->currentPos );
        const PixVec2 &p = it->get_pixes_per_frame();
        it->set_scale(glm::vec3{ (float)p.x, (float)p.y, 1.0f });
        it->draw();
    }
    
    
        /*
        //======= 渲染 单个 mesh ========//
        goPtr->mesh.actionHandle.funcs.at("update")(&(goPtr->mesh.actionHandle), 0);
        goPtr->mesh.refresh_texName();
        goPtr->mesh.set_translate( goPtr->currentPos );
        const PixVec2 &p = goPtr->mesh.get_pixes_per_frame();
        goPtr->mesh.set_scale(glm::vec3{ (float)p.x, (float)p.y, 1.0f });
        goPtr->mesh.draw();
        */


}



/* ===========================================================
 *                        LogicUpdate
 * -----------------------------------------------------------
 */
void Dog_A::LogicUpdate( GameObj *_goPtr ){
    /*
    //cout << "Dog_A::Update();" << endl;
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    assert( _goPtr->species == Dog_A::specId );
    //-- rebind ptr -----
    GameObj * goPtr = _goPtr;
    Dog_A_Binary  *bp = (Dog_A_Binary*)&(goPtr->binary[0]);
    //=====================================//

    // 暂时什么也没做...
    */

}



/* ===========================================================
 *                       BeAffect
 * -----------------------------------------------------------
 */
void Dog_A::BeAffect( GameObj *_goPtr ){
}







}//------------- namespace gameObjs: end ----------------


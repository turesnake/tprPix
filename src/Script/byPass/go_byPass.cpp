



#include "Script/gameObjs/Dog_A.h" 

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
//#include <iostream>
#include <string>

//-------------------- Engine --------------------//
#include "srcs_engine.h" 
#include "GameObj.h" 

using std::string;
//using std::cout;
//using std::endl;

//-- 试验
static gameObjs::Dog_A  dog_1 {};
static gameObjs::Dog_A  dog_2 {};


void creat_a_go( glm::vec2 _pos );


/* ===========================================================
 *                       go_byPass
 * -----------------------------------------------------------
 * -- 跳过 section，硬生成一个／组 go实例（Dog_A）
 */
void go_byPass(){

    //-------------------------//
    //  手动创建 若干个 go/Dog_A 实例
    //--------------------------//
    creat_a_go( glm::vec2{ 10.0f, 0.0f } );
    creat_a_go( glm::vec2{ -10.0f, 0.0f } );
    creat_a_go( glm::vec2{ 0.0f, 10.0f } );
    creat_a_go( glm::vec2{ 0.0f, -10.0f } );
    
    //-------------------------//
    //  手动将所有 现存的 go实例，
    //  添加入  goids_active 容器
    //--------------------------//
    /*
    for( const auto &p : esrc::memGameObjs ){
        //esrc::goids_active.insert( p.first );
    }
    */

}


/* ===========================================================
 *                      creat_a_go
 * -----------------------------------------------------------
 * -- 
 */
void creat_a_go( glm::vec2 _pos ){

    // ***| INSERT FIRST, INIT LATER  |***
    GameObj  tmp_go {};
    //goid_t goid = esrc::insert_new_gameObj( &tmp_go );

    goid_t goid = GameObj::id_manager.apply_a_u64_id();
    tmp_go.id = goid;
    esrc::memGameObjs.insert({ goid, tmp_go }); //- copy


        //GameObj  *goPtr1 = (GameObj*)&(esrc::memGameObjs.at(goid)); //- 获得目标go指针
        //gameObjs::dog_a.init( goPtr1 );
        //dog_1.init( goPtr1 );
        //goPtr1->currentPos = glm::vec2{ 10.0f, 0.0f };

        GameObj &rgo = esrc::memGameObjs.at(goid); //- 获得 目标go的 引用
        gameObjs::dog_a.init( &rgo );
        rgo.currentPos = _pos;
}






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
    auto it = esrc::memGameObjs.begin();
    for( ; it!=esrc::memGameObjs.end(); it++ ){
        esrc::goids_active.insert( it->first );
    }

}


/* ===========================================================
 *                      creat_a_go
 * -----------------------------------------------------------
 * -- 
 */
void creat_a_go( glm::vec2 _pos ){

    goid_t goid = esrc::insert_new_gameObj();

    GameObj *gop = (GameObj*)&(esrc::memGameObjs.at(goid)); //- 获取目标go指针
    gameObjs::dog_a.init( gop );
    gop->currentPos = _pos;
}


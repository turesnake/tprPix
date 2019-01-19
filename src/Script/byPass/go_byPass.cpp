



#include "Script/gameObjs/Dog_A.h" 

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "srcs_engine.h" 
#include "GameObj.h" 

using std::string;

//#include "debug.h" //- tmp

//-- 试验
//static gameObjs::Dog_A  dog_1 {};
//static gameObjs::Dog_A  dog_2 {};


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
    
    creat_a_go( glm::vec2{ 0.0f, 0.0f } );
    creat_a_go( glm::vec2{ 30.0f, 48.0f } );
    creat_a_go( glm::vec2{ 45.0f, 57.0f } );
    creat_a_go( glm::vec2{ 48.0f, 30.0f } );
    
    
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
    GameObj *goPtr = esrc::find_memGameObjs( goid ); //- 获取目标go指针

    gameObjs::dog_a.init( goPtr );
    goPtr->move.set_currentFPos( _pos );
}






#include "Script/gameObjs/Norman.h" 
#include "Script/gameObjs/BigMan.h" 
//#include "Script/gameObjs/Grass_1.h" 

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "srcs_engine.h" 
#include "GameObj.h" 
#include "MapCoord.h"
#include "IntVec.h"

using std::string;

//#include "debug.h" //- tmp


void create_a_Norman( const IntVec2 &_ppos );
void create_a_BigMan( const IntVec2 &_ppos );
//void create_a_Grass_1( const IntVec2 &_ppos );

/* ===========================================================
 *                       go_byPass
 * -----------------------------------------------------------
 * -- 跳过 section，硬生成一个／组 go实例（Dog_A）
 */
void go_byPass(){

    //-------------------------//
    //  手动CREATE 若干个 go/Dog_A 实例
    //--------------------------//
    

    //create_a_Norman( IntVec2{ 640, 640 } ); //- 初始section 中心位置 (256*256)
    //create_a_Norman( IntVec2{ 160, 160 } ); //- 初始section 中心位置 (64*64)

    create_a_Norman( IntVec2{ 40, 0 } );

    
    create_a_BigMan( IntVec2{ 0, 0 } );
    //create_a_BigMan( IntVec2{ 145, 155 } );
    //create_a_BigMan( IntVec2{ 150, 130 } );

    

    /*
    create_a_Norman( IntVec2{ 130, 145 } );
    create_a_Norman( IntVec2{ 145, 155 } );
    create_a_Norman( IntVec2{ 150, 130 } );
    */
    

    
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
 *                      create_a_Dog_A
 * -----------------------------------------------------------
 * -- 
 */

void create_a_Norman( const IntVec2 &_ppos ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjs( goid ); //- 获取目标go指针
    gameObjs::norman.init( goPtr );
    goPtr->goPos.init_by_currentMCPos( MapCoord{ ppos_2_mpos(_ppos) } );
            //-- 这是一个严格检查，_ppos 必须对齐于 mapent
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );
}


void create_a_BigMan( const IntVec2 &_ppos ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjs( goid ); //- 获取目标go指针
    gameObjs::big_man.init( goPtr );
    goPtr->goPos.init_by_currentMCPos( MapCoord{ ppos_2_mpos(_ppos) } );
            //-- 这是一个严格检查，_ppos 必须对齐于 mapent
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );
}

/*
void create_a_Grass_1( const IntVec2 &_ppos ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjs( goid ); //- 获取目标go指针
    gameObjs::grass_1.init( goPtr );
    goPtr->goPos.init_by_currentMCPos( MapCoord{ ppos_2_mpos(_ppos) } );
            //-- 这是一个严格检查，_ppos 必须对齐于 mapent
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );
}
*/


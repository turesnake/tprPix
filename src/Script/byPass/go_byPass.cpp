



#include "Script/gameObjs/Dog_A.h" 

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


void creat_a_go( const IntVec2 &_ppos );


/* ===========================================================
 *                       go_byPass
 * -----------------------------------------------------------
 * -- 跳过 section，硬生成一个／组 go实例（Dog_A）
 */
void go_byPass(){

    //-------------------------//
    //  手动创建 若干个 go/Dog_A 实例
    //--------------------------//
    
    creat_a_go( IntVec2{  0,  0 } );
    
    creat_a_go( IntVec2{ 30, 48 } );
    creat_a_go( IntVec2{ 45, 57 } );
    creat_a_go( IntVec2{ 48, 30 } );
    

    
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
void creat_a_go( const IntVec2 &_ppos ){

    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::find_memGameObjs( goid ); //- 获取目标go指针

    gameObjs::dog_a.init( goPtr );

    goPtr->goPos.init_by_currentMCPos( MapCoord{ ppos_2_mpos(_ppos) } );
}


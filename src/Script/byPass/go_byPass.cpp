



#include "Script/gameObjs/Norman.h" 
#include "Script/gameObjs/BigMan.h" 
#include "Script/gameObjs/OakTree.h" 


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


/* ===========================================================
 *                       go_byPass
 * -----------------------------------------------------------
 * -- 跳过 section，硬生成一个／组 go实例（Dog_A）
 */
void go_byPass(){

    //-------------------------//
    //  手动CREATE 若干个 go/Dog_A 实例
    //--------------------------//

    goid_t majorGoId = gameObjs::create_a_Norman( IntVec2{ 5, 0 } );
    esrc::player.bind_goid( majorGoId );

    
    gameObjs::create_a_BigMan( IntVec2{ 0, 0 } );

    //gameObjs::create_a_OakTree( IntVec2{8, 5}, 3, true );
    


    
    //-------------------------//
    //  手动将所有 现存的 go实例，
    //  添加入  goids_active 容器
    //--------------------------//

        // 放在这个位置并不好...

    auto it = esrc::memGameObjs.begin();
    for( ; it!=esrc::memGameObjs.end(); it++ ){
        //esrc::goids_active.insert( it->first );
    }

}



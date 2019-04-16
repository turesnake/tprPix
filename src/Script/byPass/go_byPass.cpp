



#include "Script/gameObjs/create_goes.h" 

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "srcs_engine.h" 
#include "GameObj.h" 
#include "MapCoord.h"
#include "IntVec.h"

//-------------------- Script --------------------//
#include "Script/resource/srcs_script.h"
#include "Altitude.h"
#include "Density.h"

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

    goid_t majorGoId = gameObjs::create_a_Go(   ssrc::get_goSpecId( "norman" ),
                                                IntVec2{ 5, 0 },
                                                0.0,
                                                Altitude {},
                                                Density {} );
    esrc::player.bind_goid( majorGoId );

    
    gameObjs::create_a_Go(  ssrc::get_goSpecId( "bigMan" ),
                            IntVec2{ 0, 0 },
                            0.0,
                            Altitude {},
                            Density {} );
    

}


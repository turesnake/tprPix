/*
 * ========================= esrc_state.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.29
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_state.h"


/* exist strs: -----
colorTableSet
coordinate
renderPool
behaviour
blueprint
camera
viewingBox
canvas
chunk
job_chunk
chunkMemState
colorTableSet
ecoObj
ecoObjMemState
field
gameArchive
gameObj
gameSeed
player
shader
time
VAOVBO
ubo

json_animFrameSet
json_colorTableSet
json_ecoSysPlan
json_gameObj
json_uiGo
json_windowConfig
*/


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace state_inn {//-------- namespace: state_inn  --------------//

    std::unordered_set<std::string> ents {};

}//------------- namespace: state_inn end --------------//

void insertState( const std::string &str_ ){
    auto [insertIt, insertBool] = state_inn::ents.insert( str_ );
    tprAssert( insertBool );
}

bool is_setState( const std::string &str_ ){
    return ( state_inn::ents.find(str_) != state_inn::ents.end() );
}


}//---------------------- namespace: esrc -------------------------//



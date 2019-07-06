/*
 * ================= esrc_animFrameSet.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理:  animFrameSeta
 * ----------------------------
 */
//-------------------- CPP --------------------//
#include <string>
#include <sstream>
#include <memory>

//-------------------- tpr --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_animFrameSet.h"


//#include "tprDebug.h" //- tmp

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace animFrameSet_inn {//----------- namespace: animFrameSet_inn -------------//

    std::unordered_map<std::string, std::unique_ptr<AnimFrameSet>> animFrameSets {};

}//--------------- namespace: animFrameSet_inn end -----------------//



/* ===========================================================
 *               insert_new_animFrameSet
 * -----------------------------------------------------------
 */
AnimFrameSet &insert_new_animFrameSet( const std::string &name_ ){
        tprAssert( animFrameSet_inn::animFrameSets.find(name_) == animFrameSet_inn::animFrameSets.end() );
    animFrameSet_inn::animFrameSets.insert({ name_, std::make_unique<AnimFrameSet>(name_) });
    return *(animFrameSet_inn::animFrameSets.at(name_).get());
}


/* ===========================================================
 *                      getnc_animActionPtr
 * -----------------------------------------------------------
 * -- 用法：
 *    getnc_animActionPtr( "dog", "jump" );
 */
AnimAction *getnc_animActionPtr(  const std::string &animFrameSetName_,
                                const std::string &actionName_ ){

    tprAssert( animFrameSet_inn::animFrameSets.find(animFrameSetName_) != animFrameSet_inn::animFrameSets.end() );
    return  animFrameSet_inn::animFrameSets.at(animFrameSetName_)->getnc_animActionPtr( actionName_ );
}


}//---------------------- namespace: esrc end -------------------------//


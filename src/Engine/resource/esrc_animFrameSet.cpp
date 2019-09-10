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

namespace esrc {//------------------ namespace: esrc -------------------------//
namespace anim_inn {//----------- namespace: anim_inn -------------//
    std::unordered_map<std::string, std::unique_ptr<AnimFrameSet>> animFrameSets {};
    std::unordered_map<animSubspeciesId_t, std::unique_ptr<AnimSubspecies>> animSubs {};

}//--------------- namespace: anim_inn end -----------------//


AnimSubspecies &find_or_insert_new_animSubspecies( animSubspeciesId_t id_ ){
    if( anim_inn::animSubs.find(id_) == anim_inn::animSubs.end() ){
        anim_inn::animSubs.insert({ id_, std::make_unique<AnimSubspecies>() });
    }
    return *(anim_inn::animSubs.at(id_).get());
}




AnimFrameSet &insert_new_animFrameSet( const std::string &name_ ){
        tprAssert( anim_inn::animFrameSets.find(name_) == anim_inn::animFrameSets.end() );
    anim_inn::animFrameSets.insert({ name_, std::make_unique<AnimFrameSet>(name_) });
    return *(anim_inn::animFrameSets.at(name_).get());
}



AnimSubspecies &get_animSubspeciesRef( animSubspeciesId_t id_ ){
        tprAssert( anim_inn::animSubs.find(id_) != anim_inn::animSubs.end() );
    return *(anim_inn::animSubs.at(id_).get());
}



animSubspeciesId_t apply_a_random_animSubspeciesId( const std::string  &animFrameSetName_,
                                                    const std::string  &subspeciesName_,
                                                    double              randVal_ ){
        tprAssert( anim_inn::animFrameSets.find(animFrameSetName_) != anim_inn::animFrameSets.end() );
    return anim_inn::animFrameSets.at(animFrameSetName_)->apply_a_random_animSubspeciesId( subspeciesName_, randVal_ );
}


AnimAction *get_animActionPtr(  animSubspeciesId_t subId_, 
                                const std::string  &actionName_ ){
        tprAssert( anim_inn::animSubs.find(subId_) != anim_inn::animSubs.end() );
    return anim_inn::animSubs.at(subId_)->get_animActionPtr( actionName_ );
}


}//---------------------- namespace: esrc end -------------------------//


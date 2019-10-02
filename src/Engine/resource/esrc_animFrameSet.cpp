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

    //======= special ids =========//
    animSubspeciesId_t emptyPixId {};
    animSubspeciesId_t fieldRimId {};


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
                                                    const std::vector<AnimLabel> &labels_,
                                                    double              randVal_ ){
        tprAssert( anim_inn::animFrameSets.find(animFrameSetName_) != anim_inn::animFrameSets.end() );
    return anim_inn::animFrameSets.at(animFrameSetName_)->apply_a_random_animSubspeciesId( labels_, randVal_ );
}


AnimAction *get_animActionPtr(  animSubspeciesId_t subId_, 
                                NineDirection      dir_,
                                const std::string  &actionName_ ){
        tprAssert( anim_inn::animSubs.find(subId_) != anim_inn::animSubs.end() );
    return anim_inn::animSubs.at(subId_)->get_animActionPtr( dir_, actionName_ );
}




//======= special ids =========//
void set_emptyPixId( animSubspeciesId_t id_ )noexcept{
    anim_inn::emptyPixId = id_;
}
animSubspeciesId_t get_emptyPixId()noexcept{
    return anim_inn::emptyPixId;
}

void set_fieldRimId( animSubspeciesId_t id_ )noexcept{
    anim_inn::fieldRimId = id_;
}
animSubspeciesId_t get_fieldRimId()noexcept{
    return anim_inn::fieldRimId;
}




}//---------------------- namespace: esrc end -------------------------//


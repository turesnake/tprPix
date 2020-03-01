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

#include "tprDebug.h"

namespace esrc {//------------------ namespace: esrc -------------------------//
namespace anim_inn {//----------- namespace: anim_inn -------------//


    std::unordered_map<std::string, std::unique_ptr<AnimFrameSet>> animFrameSets {};
    std::unordered_map<animSubspeciesId_t, std::unique_ptr<AnimSubspecies>> animSubs {};

    //======= special ids =========//
    animSubspeciesId_t nilCollide_emptyPixId {};
    animSubspeciesId_t squareCollide_emptyPixId {};
    animSubspeciesId_t fieldRimId {};


}//--------------- namespace: anim_inn end -----------------//


AnimFrameSet &insert_new_animFrameSet( const std::string &name_ ){
    auto [insertIt, insertBool] = anim_inn::animFrameSets.insert({ name_, std::make_unique<AnimFrameSet>(name_) });

    if( !insertBool ){
        cout << "find duplicate afsName: " << name_ << endl; 
    }
    tprAssert( insertBool );
    return *(insertIt->second);
}


// support multi-thread
animSubspeciesId_t apply_a_random_animSubspeciesId( const std::string  &animFrameSetName_,
                                                    const std::string  &label_,
                                                    size_t              uWeight_ ){
        // debug
        if( anim_inn::animFrameSets.find(animFrameSetName_) == anim_inn::animFrameSets.end() ){
            cout << "animFrameSetName_: " <<  animFrameSetName_ 
                << endl;
        }
        tprAssert( anim_inn::animFrameSets.find(animFrameSetName_) != anim_inn::animFrameSets.end() );
    return anim_inn::animFrameSets.at(animFrameSetName_)->apply_a_random_animSubspeciesId( label_, uWeight_ );
}


// 仅用于 创建阶段 
AnimSubspecies &find_or_insert_new_animSubspecies( animSubspeciesId_t id_ ){
    // if target key is existed, insert will not happen. but still ret target ent-it
    auto [insertIt, insertBool] = anim_inn::animSubs.insert({ id_, std::make_unique<AnimSubspecies>() });
    return *(insertIt->second);
}


AnimSubspecies &get_animSubspeciesRef( animSubspeciesId_t id_ ){
        tprAssert( anim_inn::animSubs.find(id_) != anim_inn::animSubs.end() );
    return *(anim_inn::animSubs.at(id_));
}



AnimAction *get_animActionPtr(  animSubspeciesId_t subId_, 
                                NineDirection      dir_,
                                BrokenLvl           brokenLvl_,
                                AnimActionEName    actionEName_ ){

    tprAssert( anim_inn::animSubs.find(subId_) != anim_inn::animSubs.end() );

    auto ret = anim_inn::animSubs.at(subId_)->get_animActionPtr( dir_, brokenLvl_, actionEName_ );
    if( !ret.has_value() ){
        // debug
        cout << "\ndir_ = " << nineDirection_2_str(dir_)
            << "\nbrokenLvl_ = " << brokenLvl_2_str( brokenLvl_ )
            << "\nactionEName_ = " << animActionEName_2_str( actionEName_ )
            << endl;
        tprAssert(0);
    }

    return ret.value();
}


//======= special ids =========//
void set_nilCollide_emptyPixId( animSubspeciesId_t id_ )noexcept{
    anim_inn::nilCollide_emptyPixId = id_;
}
void set_squareCollide_emptyPixId( animSubspeciesId_t id_ )noexcept{
    anim_inn::squareCollide_emptyPixId = id_;
}
animSubspeciesId_t get_nilCollide_emptyPixId()noexcept{
    return anim_inn::nilCollide_emptyPixId;
}
animSubspeciesId_t get_squareCollide_emptyPixId()noexcept{
    return anim_inn::squareCollide_emptyPixId;
}

void set_fieldRimId( animSubspeciesId_t id_ )noexcept{
    anim_inn::fieldRimId = id_;
}
animSubspeciesId_t get_fieldRimId()noexcept{
    return anim_inn::fieldRimId;
}




}//---------------------- namespace: esrc end -------------------------//


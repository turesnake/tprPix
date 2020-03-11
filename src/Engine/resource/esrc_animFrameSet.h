/*
 * ========================= esrc_animFrameSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_ANIM_FRAME_SET_H
#define TPR_ESRC_ANIM_FRAME_SET_H

//-------------------- CPP --------------------//
#include <unordered_map>
#include <string>

//-------------------- Engine --------------------//
#include "AnimFrameSet.h"

namespace esrc {//------------------ namespace: esrc -------------------------//



AnimSubspecies &find_or_insert_new_animSubspecies( animSubspeciesId_t id_ );

//AnimSubspecies &get_animSubspeciesRef( animSubspeciesId_t id_ ); // not used

AnimFrameSet &insert_new_animFrameSet( const std::string &name_ );

animSubspeciesId_t apply_a_random_animSubspeciesId( const std::string  &animFrameSetName_,
                                                    const std::string  &label_,
                                                    size_t     uWeight_ );

AnimAction *get_animActionPtr(  animSubspeciesId_t subId_, 
                                NineDirection      dir_,
                                BrokenLvl           brokenLvl_,
                                AnimActionEName    actionEName_ );


//-- special SubspeciesId --
void set_nilCollide_emptyPixId( animSubspeciesId_t id_ )noexcept;
void set_squareCollide_emptyPixId( animSubspeciesId_t id_ )noexcept;
animSubspeciesId_t get_nilCollide_emptyPixId()noexcept;
animSubspeciesId_t get_squareCollide_emptyPixId()noexcept;

void set_fieldRimId( animSubspeciesId_t id_ )noexcept;
animSubspeciesId_t get_fieldRimId()noexcept;



}//---------------------- namespace: esrc -------------------------//
#endif


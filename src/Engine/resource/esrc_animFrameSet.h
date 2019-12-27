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



AnimSubspec &find_or_insert_new_animSubspec( animSubspecId_t id_ );

AnimSubspec &get_animSubspecRef( animSubspecId_t id_ );

AnimFrameSet &insert_new_animFrameSet( const std::string &name_ );

animSubspecId_t apply_a_random_animSubspecId( const std::string  &animFrameSetName_,
                                                    const std::vector<AnimLabel> &labels_,
                                                    size_t              uWeight_ );

AnimAction *get_animActionPtr(  animSubspecId_t subId_, 
                                NineDirection      dir_,
                                BrokenLvl           brokenLvl_,
                                const std::string  &actionName_ );


//-- special SubspecId --
void set_emptyPixId( animSubspecId_t id_ )noexcept;
animSubspecId_t get_emptyPixId()noexcept;

void set_fieldRimId( animSubspecId_t id_ )noexcept;
animSubspecId_t get_fieldRimId()noexcept;



}//---------------------- namespace: esrc -------------------------//
#endif


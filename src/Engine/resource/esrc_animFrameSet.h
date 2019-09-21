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

AnimSubspecies &get_animSubspeciesRef( animSubspeciesId_t id_ );

AnimFrameSet &insert_new_animFrameSet( const std::string &name_ );

animSubspeciesId_t apply_a_random_animSubspeciesId( const std::string  &animFrameSetName_,
                                                    const std::vector<AnimLabel> &labels_,
                                                    double              randVal_ );

AnimAction *get_animActionPtr(  animSubspeciesId_t subId_, 
                                NineDirection      dir_,
                                const std::string  &actionName_ );


}//---------------------- namespace: esrc -------------------------//
#endif


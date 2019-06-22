/*
 * ========================= esrc_animFrameSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_ANIM_FRAME_SET_H_
#define TPR_ESRC_ANIM_FRAME_SET_H_

//-------------------- CPP --------------------//
#include <unordered_map>
#include <string>

//-------------------- Engine --------------------//
#include "AnimFrameSet.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


void load_animFrameSets();

AnimAction *getnc_animActionPtr(const std::string &_animFrameSetName,
                                const std::string &_actionName );


}//---------------------- namespace: esrc -------------------------//
#endif


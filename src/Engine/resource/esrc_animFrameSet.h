/*
 * ========================= esrc_animFrameSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_ANIM_FRAME_SET_H_
#define _TPR_ESRC_ANIM_FRAME_SET_H_

//-------------------- CPP --------------------//
#include <unordered_map>
#include <string>

//-------------------- Engine --------------------//
#include "AnimFrameSet.h"

namespace esrc{ //------------------ namespace: esrc -------------------------//

inline std::unordered_map<std::string, AnimFrameSet> animFrameSets {};
void load_animFrameSets();


}//---------------------- namespace: esrc -------------------------//
#endif


/*
 * ========================= esrc_renderPool.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_RENDER_POOL_H
#define TPR_ESRC_RENDER_POOL_H

//-------------------- Engine --------------------//
#include "RenderPool.h"
#include "GroundRenderPool.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


//-- Must after esrc::init_colorTableSet !!!
void init_renderPools()noexcept;

void debug_for_renderPools()noexcept;

RenderPool &get_renderPool( RenderPoolType type_ )noexcept;
GroundRenderPool &get_groundRenderPool()noexcept;

void clear_all_renderPool()noexcept;


}//---------------------- namespace: esrc -------------------------//
#endif


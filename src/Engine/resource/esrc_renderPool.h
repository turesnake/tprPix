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


namespace esrc {//------------------ namespace: esrc -------------------------//



void init_renderPools()noexcept;

RenderPool &get_renderPool( RenderPoolType type_ )noexcept;
void clear_all_renderPool()noexcept;


}//---------------------- namespace: esrc -------------------------//
#endif


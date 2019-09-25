/*
 * ==================== esrc_colorTableSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.23
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_COLOR_TABLE_SET_H
#define TPR_ESRC_COLOR_TABLE_SET_H

//-------------------- Engine --------------------//
#include "ColorTable.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


void init_colorTableSet()noexcept;
ColorTableSet &get_colorTabelSet()noexcept;
CurrentColorTable &get_currentColorTabel()noexcept;

void rebind_currentColorTabel_target( colorTableId_t id_ )noexcept;


}//---------------------- namespace: esrc -------------------------//
#endif


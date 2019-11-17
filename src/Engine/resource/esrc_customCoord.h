/*
 * ========================= esrc_customCoord.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.15
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_CUSTOM_COORD_H
#define TPR_ESRC_CUSTOM_COORD_H

//-------------------- Engine --------------------//
#include "CustomCoord.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


void init_customCoord();

const CustomCoord &get_artifactCoordRef()noexcept;


}//---------------------- namespace: esrc -------------------------//
#endif


/*
 * ======================= UBO_Window.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_UBO_WINDOW_H
#define TPR_UBO_WINDOW_H

//-------------------- Engine --------------------//
#include "FloatVec.h"


namespace ubo{//------------- namespace ubo ----------------

// [tmp once]
struct UBO_Window{
    FloatVec2 gameSZ {}; // equal to ViewingBox::gameSZ
    //....
};

// only called in:
// -1- app.init
// -2- windowSZ changed (never yet...)
void write_ubo_Window();


}//------------- namespace ubo: end ----------------
#endif 


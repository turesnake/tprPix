/*
 * ======================= UBO_Time.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_UBO_TIME_H
#define TPR_UBO_TIME_H

//-------------------- Engine --------------------//
#include "FloatVec.h"


namespace ubo{//------------- namespace ubo ----------------

// [tmp once]
struct UBO_Time{
    float currentTime {};
    //....
};

// called every render frmae
void write_ubo_Time();


}//------------- namespace ubo: end ----------------
#endif 


/*
 * ======================= colorTableId.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.023
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_COLOR_TABLE_ID_H
#define TPR_COLOR_TABLE_ID_H

//------------------- Libs --------------------//
#include "tprDataType.h" 

using colorTableId_t = u32_t;

inline const colorTableId_t MaxColorTableId { static_cast<colorTableId_t>(-1) };
        //-- never reach

#endif 


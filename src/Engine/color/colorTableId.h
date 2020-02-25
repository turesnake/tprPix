/*
 * ======================= colorTableId.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.023
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_COLOR_TABLE_ID_H
#define TPR_COLOR_TABLE_ID_H

//------------------- CPP --------------------//
#include <cstdint> // uint8_t
#include <string>


using colorTableId_t = uint32_t;
        // 暂不适合用 hash 来生成id
        // 此id 肩负 vector idx 的职责


extern const colorTableId_t NilColorTableId;
        //-- never reach

#endif 


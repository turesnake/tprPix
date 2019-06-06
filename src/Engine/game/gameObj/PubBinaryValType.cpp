/*
 * ================== PubBinaryValType.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#include "PubBinaryValType.h"


namespace {//-------- namespace: --------------//

    //- 记载了每一种元素的 字节数（类型）--
    std::map<u32_t, u32_t> PubBinaryValSizes {
        { static_cast<u32_t>(PubBinaryValType::HP), static_cast<u32_t>(sizeof(int)) },
        { static_cast<u32_t>(PubBinaryValType::MP), static_cast<u32_t>(sizeof(int)) }
    //...
    };

}//------------- namespace: end --------------//


std::map<u32_t, u32_t> &get_PubBinaryValSizes(){
    return PubBinaryValSizes;
}





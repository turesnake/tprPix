/*
 * ==================== PubBinaryValType.h =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#ifndef TPR_PUB_BINARY_VAL_TYPE_H
#define TPR_PUB_BINARY_VAL_TYPE_H

//-------------------- CPP --------------------//
#include <map>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//- go.pubBinary 中 存储的变量 的类型 
//  千万不能 自定义 元素的值。应该让元素的值按照持续，被自动分配(从0开始增长)
//  元素值将被转换为 idx，用来访问 bitMap
enum class PubBinaryValType : u32_t{

    HP, //- int
    MP  //- int

};


//- 记载了每一种元素的 字节数（类型）--
/*
inline std::map<u32_t, u32_t> PubBinaryValSizes {
    { static_cast<u32_t>(PubBinaryValType::HP), static_cast<u32_t>(sizeof(int)) },
    { static_cast<u32_t>(PubBinaryValType::MP), static_cast<u32_t>(sizeof(int)) }
    //...
};
*/

std::map<u32_t, u32_t> &get_PubBinaryValSizes();


#endif 


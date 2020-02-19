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
#include <cstdint> // uint8_t



//- go.pubBinary 中 存储的变量 的类型 
//  千万不能 自定义 元素的值。应该让元素的值按照持续，被自动分配(从0开始增长)
//  元素值将被转换为 idx，用来访问 bitMap
enum class PubBinaryValType : uint32_t{

    HP, //- int
    MP  //- int

};


std::map<uint32_t, uint32_t> &get_PubBinaryValSizes();


#endif 


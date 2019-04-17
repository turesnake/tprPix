/*
 * ====================== EcoSysType.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    ecosystem type
 * ----------------------------
 */
#ifndef _TPR_ECOSYS_TYPE_H_
#define _TPR_ECOSYS_TYPE_H_

//------------------- C --------------------//
#include <cstddef> //- size_t

//------------------- Libs --------------------//
#include "tprDataType.h" 


using ecoSysId_t = u32_t;


//- 每一个type，都存在数个变种
enum class EcoSysType : u32_t{

    BegIdx = 0, //- 通过此值来计算 第一个 typeIdx 是多少。
    
    //-- 确保所有 type 连续存放 --
    Forest,
    DarkForest,
    Plain,  
    Swamp,
    Desert,

    //----
    EndIdx //- 通过此值来计算 最后一个 typeIdx 是多少。
};


inline const size_t EcoSysType_MinIdx { static_cast<size_t>(EcoSysType::BegIdx) + 1 }; //- 最小序号
inline const size_t EcoSysType_MaxIdx { static_cast<size_t>(EcoSysType::EndIdx) - 1 }; //- 最大序号
inline const size_t EcoSysType_Num  { EcoSysType_MaxIdx - EcoSysType_MinIdx + 1 }; //- type 个数

//-- 用来遍历 某些复合容器 --
inline size_t ecoSysType_2_idx( EcoSysType _type ){
    return (static_cast<size_t>(_type) - EcoSysType_MinIdx);
}


#endif 


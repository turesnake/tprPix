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


//------------------- Libs --------------------//
#include "tprDataType.h" 


enum class EcoSysType : u8_t{

    BegIdx = 0, //- 通过此值来计算 第一个 typeIdx 是多少。
    
    //-- 确保所有 type 连续存放 --
    Forst,
    DarkForst,
    Plain,  
    Swamp,
    Desert,

    //----
    EndIdx //- 通过此值来计算 最后一个 typeIdx 是多少。
};

inline const int EcoSysType_MinIdx { (int)(EcoSysType::BegIdx) + 1 }; //- 最小序号
inline const int EcoSysType_MaxIdx { (int)(EcoSysType::EndIdx) - 1 }; //- 最大序号


EcoSysType get_rand_EcoSysType();


#endif 
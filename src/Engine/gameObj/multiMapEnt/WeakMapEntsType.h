/*
 * ====================== WeakMapEntsType.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_WEAK_MAP_ENTS_TYPE_H
#define TPR_WEAK_MAP_ENTS_TYPE_H


//--------------- CPP ------------------//
#include <string>


// 一个 mp-go，自身所在的 mapent 为 rootMP，
// 同时还允许占据数个 weakMP
enum class WeakMapEntsType{
    Empty, // 不包含任何 weakMapEnt
    //---
    T_1m2,
    T_2m1,
    T_2m2,
    T_3m3,
    T_4m4,
    //...
};


std::string weakMapEntsType_2_str( WeakMapEntsType t_ )noexcept;
WeakMapEntsType str_2_weakMapEntsType( const std::string &str_ )noexcept;





#endif 


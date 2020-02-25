/*
 * ====================== SignInMapEnts_Square_Type.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_SIGN_IN_MAP_ENTS_SQUARE_TYPE_H
#define TPR_SIGN_IN_MAP_ENTS_SQUARE_TYPE_H


//--------------- CPP ------------------//
#include <string>


// 一个 mp-go，自身所在的 mapent 为 rootMP，
// 同时还允许占据数个 weakMP
enum class SignInMapEnts_Square_Type{
    T_1m1, // only have rootMapEnt
    //---
    T_1m2,
    T_2m1,
    T_2m2,
    T_3m3,
    T_4m4,
    //...
};


std::string signInMapEnts_square_type_2_str( SignInMapEnts_Square_Type t_ )noexcept;
SignInMapEnts_Square_Type str_2_signInMapEnts_square_type( const std::string &str_ )noexcept;





#endif 


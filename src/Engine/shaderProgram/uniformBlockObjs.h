/*
 * ===================== uniformBlockObjs.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_UNIFORM_BLOCK_OBJS_H
#define TPR_UNIFORM_BLOCK_OBJS_H
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h> 

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "UniformBlockObj.h"
#include "FloatVec.h"

namespace ubo{//------------- namespace ubo ----------------

// 1-ubo_instance, 1 enum-obj
enum class UBOType{

    Seeds,
    Camera,
    Window,
    Time,

    OriginColorTable,
    UnifiedColorTable,
    //...
};


inline std::unordered_map<UBOType, GLuint> uboBindPoints {
    {UBOType::Seeds,             1 },
    {UBOType::Camera,            2 },
    {UBOType::Window,            3 },
    {UBOType::Time,              4 },

    {UBOType::OriginColorTable,  5 },
    {UBOType::UnifiedColorTable, 6 }
    //...
};

inline GLuint get_bindPoint( UBOType type_ )noexcept{
    tprAssert( uboBindPoints.find(type_) != uboBindPoints.end() );
    return uboBindPoints.at(type_);
}



}//------------- namespace ubo: end ----------------
#endif 


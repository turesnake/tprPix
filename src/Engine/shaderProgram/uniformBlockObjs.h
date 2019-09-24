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


// 1-ubo_instance, 1 enum-obj
enum class UBOType{

    Seeds,
    Camera,

    OriginColorTable,
    UnifiedColorTable,
    //...
};



inline std::unordered_map<UBOType, GLuint> uboBindPoints {

    {UBOType::Seeds,             1 },
    {UBOType::Camera,            2 },

    {UBOType::OriginColorTable,  3 },
    {UBOType::UnifiedColorTable, 4 }
    //...
};


#endif 


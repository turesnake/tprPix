/*
 * ======================= esrc_VAOVBO.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_VAOVBO.h"

//-------------------- Engine --------------------//
#include "esrc_state.h"



namespace esrc {//------------------ namespace: esrc -------------------------//
namespace VAOVBO_inn {//------------------ namespace: VAOVBO_inn ---------------------//

    // 1 * 1 pixes
    const std::vector<float> rectPoints {
        //      pos            textuel 
        -0.5f,  -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f,  -0.5f, 0.0f,  1.0f, 0.0f,
         0.5f,   0.5f, 0.0f,  1.0f, 1.0f,
        //---
         0.5f,   0.5f, 0.0f,  1.0f, 1.0f,
        -0.5f,   0.5f, 0.0f,  0.0f, 1.0f,
        -0.5f,  -0.5f, 0.0f,  0.0f, 0.0f
    };
    const GLsizeiptr VBO_size = static_cast<GLsizeiptr>(sizeof(float) * rectPoints.size());
    const GLsizei pointNums { 6 }; 

    std::unique_ptr<VAOVBO> VAOVBOUPtr {nullptr}; // only one

}//--------------------- namespace: VAOVBO_inn end ------------------------//

void init_VAOVBO(){
    VAOVBO_inn::VAOVBOUPtr = std::make_unique<class VAOVBO>();
    VAOVBO_inn::VAOVBOUPtr->init(  static_cast<const GLvoid *>(&(VAOVBO_inn::rectPoints.at(0))) ,
                                VAOVBO_inn::VBO_size );
    esrc::insertState("VAOVBO");
}

void delete_VAOVBO()noexcept{
    VAOVBO_inn::VAOVBOUPtr = nullptr;
}

GLuint get_VAO()noexcept{
    return VAOVBO_inn::VAOVBOUPtr->get_VAO();
}
GLsizei get_pointNums()noexcept{
    return VAOVBO_inn::pointNums;
}










}//---------------------- namespace: esrc -------------------------//


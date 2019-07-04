/*
 * ====================== esrc_shader.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.26
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  shaders  在 内存中的 管理
 * ----------------------------
 */
//-------------------- CPP --------------------//
#include <memory>

//-------------------- Engine --------------------//
#include "esrc_shader.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace shader_inn {//-------- namespace: shader_inn --------------//

    std::unique_ptr<ShaderProgram> rect_shaderUPtr;

}//------------- namespace: shader_inn end --------------//


/* ===========================================================
 *                   get_rect_shader
 * -----------------------------------------------------------
 */
ShaderProgram &get_rect_shader(){
    return *(shader_inn::rect_shaderUPtr.get());
}


/* ===========================================================
 *                    init_shaders     [pure]
 * -----------------------------------------------------------
 * -- init 游戏中 所有 shader实例
 */
void init_shaders(){

    shader_inn::rect_shaderUPtr = std::make_unique<ShaderProgram>();

    //---- rect_sahder ----//
    shader_inn::rect_shaderUPtr->init( "/base.vs", "/base.fs" ); 
    shader_inn::rect_shaderUPtr->use_program();
    shader_inn::rect_shaderUPtr->add_new_uniform( "model" );
    shader_inn::rect_shaderUPtr->add_new_uniform( "view" );
    shader_inn::rect_shaderUPtr->add_new_uniform( "projection" );
    shader_inn::rect_shaderUPtr->add_new_uniform( "texture1" );
    shader_inn::rect_shaderUPtr->use_program();
}



}//---------------------- namespace: esrc -------------------------//

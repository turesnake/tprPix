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
    std::unique_ptr<ShaderProgram> playerGoCircle_shaderUPtr;
    std::unique_ptr<ShaderProgram> mapSurface_shaderUPtr;


}//------------- namespace: shader_inn end --------------//


/* ===========================================================
 *                   get_rect_shader
 * -----------------------------------------------------------
 */
ShaderProgram &get_rect_shader(){
    return *(shader_inn::rect_shaderUPtr.get());
}
ShaderProgram &get_playerGoCircle_shader(){
    return *(shader_inn::playerGoCircle_shaderUPtr.get());
}
ShaderProgram &get_mapSurface_shader(){
    return *(shader_inn::mapSurface_shaderUPtr.get());
}


/* ===========================================================
 *                    init_shaders     [pure]
 * -----------------------------------------------------------
 * -- init 游戏中 所有 shader实例
 */
void init_shaders(){

    //---- rect_sahder ----//
    shader_inn::rect_shaderUPtr = std::make_unique<ShaderProgram>();
    shader_inn::rect_shaderUPtr->init( "/base.vs", "/base.fs" ); 
    shader_inn::rect_shaderUPtr->use_program();
    shader_inn::rect_shaderUPtr->add_new_uniform( "model" );
    shader_inn::rect_shaderUPtr->add_new_uniform( "view" );
    shader_inn::rect_shaderUPtr->add_new_uniform( "projection" );
    shader_inn::rect_shaderUPtr->add_new_uniform( "texture1" );


    //---- playerGoCircle_sahder ----//
    shader_inn::playerGoCircle_shaderUPtr = std::make_unique<ShaderProgram>();
    shader_inn::playerGoCircle_shaderUPtr->init( "/playerGoCircle.vs", "/playerGoCircle.fs" ); 
    shader_inn::playerGoCircle_shaderUPtr->use_program();
    shader_inn::playerGoCircle_shaderUPtr->add_new_uniform( "model" );
    shader_inn::playerGoCircle_shaderUPtr->add_new_uniform( "view" );
    shader_inn::playerGoCircle_shaderUPtr->add_new_uniform( "projection" );
    shader_inn::playerGoCircle_shaderUPtr->add_new_uniform( "texture1" );


    //---- mapSurface_sahder ----//
    shader_inn::mapSurface_shaderUPtr = std::make_unique<ShaderProgram>();
    shader_inn::mapSurface_shaderUPtr->init( "/mapSurface.vs", "/mapSurface.fs" ); 
    shader_inn::mapSurface_shaderUPtr->use_program();
    shader_inn::mapSurface_shaderUPtr->add_new_uniform( "model" );
    shader_inn::mapSurface_shaderUPtr->add_new_uniform( "view" );
    shader_inn::mapSurface_shaderUPtr->add_new_uniform( "projection" );
    shader_inn::mapSurface_shaderUPtr->add_new_uniform( "texture1" );


}



}//---------------------- namespace: esrc -------------------------//

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
#include <unordered_map>
#include <memory>

//-------------------- Engine --------------------//
#include "esrc_shader.h"
#include "esrc_uniformBlockObj.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace shader_inn {//-------- namespace: shader_inn --------------//

    std::unordered_map<ShaderType, std::unique_ptr<ShaderProgram>> shaderUPtrs {};

}//------------- namespace: shader_inn end --------------//


ShaderProgram &get_shaderRef( ShaderType type_ )noexcept{
    tprAssert( shader_inn::shaderUPtrs.find(type_) != shader_inn::shaderUPtrs.end() );
    return *(shader_inn::shaderUPtrs.at(type_).get());
}
ShaderProgram *get_shaderPtr( ShaderType type_ )noexcept{
    tprAssert( shader_inn::shaderUPtrs.find(type_) != shader_inn::shaderUPtrs.end() );
    return shader_inn::shaderUPtrs.at(type_).get();
}

ShaderProgram &insert_new_shader( ShaderType type_ )noexcept{
    tprAssert( shader_inn::shaderUPtrs.find(type_) == shader_inn::shaderUPtrs.end() );
    shader_inn::shaderUPtrs.insert({ type_, std::make_unique<ShaderProgram>() });
    return *(shader_inn::shaderUPtrs.at(type_).get());
}


/* ===========================================================
 *                    init_shaders 
 * -----------------------------------------------------------
 * make sure ubo is inited !!!
 */
void init_shaders(){

    //------ ubos --------//
    auto &ubo_seeds = esrc::get_uniformBlockObjRef( UBOType::Seeds );
    auto &ubo_camera = esrc::get_uniformBlockObjRef( UBOType::Camera );
    auto &ubo_originColorTable = esrc::get_uniformBlockObjRef( UBOType::OriginColorTable );
    auto &ubo_unifiedColorTable = esrc::get_uniformBlockObjRef( UBOType::UnifiedColorTable );
    //...

    {//---- originColor_sahder ----//
        auto &sp = insert_new_shader( ShaderType::OriginColor );
        //---
        sp.init( "/originColor.vs", "/originColor.fs" ); //- test
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );
        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_originColorTable.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_unifiedColorTable.bind_2_shaderProgram(sp.get_shaderProgramObj());
        //...
    }

    {//---- unifiedColor_sahder ----//
        auto &sp = insert_new_shader( ShaderType::UnifiedColor );
        //---
        sp.init( "/unifiedColor.vs", "/unifiedColor.fs" ); //- test
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );
        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_originColorTable.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_unifiedColorTable.bind_2_shaderProgram(sp.get_shaderProgramObj());
        //...
    }


    {//---- playerGoCircle_sahder ----//
        auto &sp = insert_new_shader( ShaderType::PlayerGoCircle );
        //---
        sp.init( "/playerGoCircle.vs", "/playerGoCircle.fs" ); 
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );
        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
    }


    {//---- mapSurface_sahder ----//
        auto &sp = insert_new_shader( ShaderType::MapSurface );
        //---
        sp.init( "/mapSurface.vs", "/mapSurface.fs" ); 
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );
        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
    }

    {//---- canvas: ground ----//
        auto &sp = insert_new_shader( ShaderType::Ground );
        //---
        sp.init( "/groundCanvas.vs",  "/groundCanvas.fs" ); 
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );

        sp.add_new_uniform( "u_time" ); //- 1-float
        
        sp.add_new_uniform( "texSizeW" ); //- 1-float
        sp.add_new_uniform( "texSizeH" ); //- 1-float

        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_unifiedColorTable.bind_2_shaderProgram( sp.get_shaderProgramObj() );
    }


    {//---- canvas: water ----//
        auto &sp = insert_new_shader( ShaderType::Water );
        //---
        sp.init( "/waterAnimCanvas.vs", "/waterAnimCanvas.fs" ); 
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );

        sp.add_new_uniform( "u_time" ); //- 1-float

        sp.add_new_uniform( "texSizeW" ); //- 1-float
        sp.add_new_uniform( "texSizeH" ); //- 1-float

        //--- ubo --//
        ubo_seeds.bind_2_shaderProgram( sp.get_shaderProgramObj() );
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
    }


}



}//---------------------- namespace: esrc -------------------------//

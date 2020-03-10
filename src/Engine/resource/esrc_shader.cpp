/*
 * ====================== esrc_shader.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.26
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  shaders  在 内存中的 管理
 * ----------------------------
 */
#include "pch.h"

//-------------------- Engine --------------------//
#include "esrc_shader.h"
#include "esrc_uniformBlockObj.h"
#include "esrc_state.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace shader_inn {//-------- namespace: shader_inn --------------//

    std::unordered_map<ShaderType, std::unique_ptr<ShaderProgram>> shaderUPtrs {};

}//------------- namespace: shader_inn end --------------//


ShaderProgram &get_shaderRef( ShaderType type_ )noexcept{
    tprAssert( shader_inn::shaderUPtrs.find(type_) != shader_inn::shaderUPtrs.end() );
    return *(shader_inn::shaderUPtrs.at(type_));
}
ShaderProgram *get_shaderPtr( ShaderType type_ )noexcept{
    tprAssert( shader_inn::shaderUPtrs.find(type_) != shader_inn::shaderUPtrs.end() );
    return shader_inn::shaderUPtrs.at(type_).get();
}

ShaderProgram &insert_new_shader( ShaderType type_ )noexcept{
    auto [insertIt, insertBool] = shader_inn::shaderUPtrs.emplace( type_, std::make_unique<ShaderProgram>() );
    tprAssert( insertBool );
    return *(insertIt->second);
}


/* ===========================================================
 *                    init_shaders 
 * -----------------------------------------------------------
 */
void init_shaders(){

    tprAssert( esrc::is_setState("ubo") ); // make sure ubo is inited !!!

    //------ ubos --------//
    auto &ubo_seeds = esrc::get_uniformBlockObjRef( ubo::UBOType::Seeds );
    auto &ubo_camera = esrc::get_uniformBlockObjRef( ubo::UBOType::Camera );
    auto &ubo_window = esrc::get_uniformBlockObjRef( ubo::UBOType::Window );
    auto &ubo_time = esrc::get_uniformBlockObjRef( ubo::UBOType::Time );
    auto &ubo_worldCoord = esrc::get_uniformBlockObjRef( ubo::UBOType::WorldCoord );
    auto &ubo_originColorTable = esrc::get_uniformBlockObjRef( ubo::UBOType::OriginColorTable );
    auto &ubo_unifiedColorTable = esrc::get_uniformBlockObjRef( ubo::UBOType::UnifiedColorTable );
    auto &ubo_groundColorTable = esrc::get_uniformBlockObjRef( ubo::UBOType::GroundColorTable );
    auto &ubo_colorTableId = esrc::get_uniformBlockObjRef( ubo::UBOType::ColorTableId );
    auto &ubo_bioSoupColorTable = esrc::get_uniformBlockObjRef( ubo::UBOType::BioSoupColorTable );
    //...

    {//---- shadow ----//
        auto &sp = insert_new_shader( ShaderType::Shadow );
        //---
        sp.init( "/shadow.vs", "/shadow.fs" );
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );
        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
        //...
    }

    {//---- originColor_shader ----//
        auto &sp = insert_new_shader( ShaderType::OriginColor );
        //---
        sp.init( "/originColor.vs", "/originColor.fs" );
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );
        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_originColorTable.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_unifiedColorTable.bind_2_shaderProgram(sp.get_shaderProgramObj());
        //...
    }

    {//---- unifiedColor_shader ----//
        auto &sp = insert_new_shader( ShaderType::UnifiedColor );
        //---
        sp.init( "/unifiedColor.vs", "/unifiedColor.fs" );
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );
        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_originColorTable.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_unifiedColorTable.bind_2_shaderProgram(sp.get_shaderProgramObj());
        //...
    }

    {//---- GroundColor_shader ----//
        auto &sp = insert_new_shader( ShaderType::GroundColor );
        //---
        sp.init( "/groundColor.vs", "/groundColor.fs" );
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );
        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_groundColorTable.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_colorTableId.bind_2_shaderProgram(sp.get_shaderProgramObj());
        //...
    }


    {//---- playerGoCircle_shader ----//
        auto &sp = insert_new_shader( ShaderType::PlayerGoCircle );
        //---
        sp.init( "/playerGoCircle.vs", "/playerGoCircle.fs" ); 
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
        
        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_window.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_time.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_unifiedColorTable.bind_2_shaderProgram( sp.get_shaderProgramObj() );
    }

    
    {//---- bioSoup_base ----//
        auto &sp = insert_new_shader( ShaderType::BioSoupBase );
        //---
        sp.init( "/bioSoupBase.vs", "/bioSoupBase.fs" );
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );
        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_bioSoupColorTable.bind_2_shaderProgram(sp.get_shaderProgramObj());
    }

    {//---- bioSoup_particle ----//
        auto &sp = insert_new_shader( ShaderType::BioSoupParticle );
        //---
        sp.init( "/bioSoupParticle.vs", "/bioSoupParticle.fs" );
        sp.use_program();

        sp.add_new_uniform( "model" );
        sp.add_new_uniform( "texture1" );
        //--- ubo --//
        ubo_camera.bind_2_shaderProgram(sp.get_shaderProgramObj());
        ubo_bioSoupColorTable.bind_2_shaderProgram(sp.get_shaderProgramObj());
    }



    //=====//
    esrc::insertState("shader");
}



}//---------------------- namespace: esrc -------------------------//

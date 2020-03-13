/*
 * ===================== GameObjMeshSet.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "GameObjMeshSet.h"

//-------------------- CPP --------------------//


//-------------------- Engine --------------------//
#include "GameObj.h"


#include "esrc_shader.h"



GameObjMesh &GameObjMeshSet::creat_new_goMesh( 
                                    const std::string       &name_,
                                    animSubspeciesId_t      subspeciesId_,
                                    AnimActionEName         actionEName_,
                                    RenderLayerType         layerType_,
                                    ShaderType              shaderType_,
                                    const glm::vec2         pposOff_,
                                    double                  zOff_,
                                    size_t                  uWeight,
                                    bool                    isVisible_
                                ){

    auto gmUPtr = std::make_unique<GameObjMesh>( this->goRef, pposOff_, zOff_,isVisible_ );
    auto [insertIt, insertBool] = this->goMeshs.emplace( name_, std::make_unique<GameObjMeshSet::Data>( std::move(gmUPtr) ) ); 
    tprAssert( insertBool );
    GameObjMesh &gmesh = *(insertIt->second->goMesh);   

    //-- bind_animAction --//
    //-- 确保提前设置好了 go.direction ！！！
    gmesh.set_animSubspeciesId( subspeciesId_ );
    gmesh.set_animActionEName( actionEName_ );
    gmesh.bind_animAction(); // Must Before Everything!!!

    //----- init -----//
    gmesh.set_uWeight( uWeight );
    gmesh.set_pic_renderLayer( layerType_ ); 
    gmesh.set_pic_shader_program( &esrc::get_shaderRef(shaderType_) );
    if( gmesh.isHaveShadow ){
        gmesh.set_shadow_shader_program( &esrc::get_shaderRef( ShaderType::Shadow ) ); //- 暂时自动选配 tmp
    }
    
    return gmesh;
}


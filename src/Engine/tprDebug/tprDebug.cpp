/*
 * ========================= tprDebug.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    MOST  POOR DEBUG SYSTEM
 * ----------------------------
 */
#include "tprDebug.h"

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- Engine --------------------//
#include "config.h"
#include "slicePic.h"
#include "create_texNames.h"
#include "IntVec.h"
#include "ViewingBox.h"
#include "esrc_shader.h"
#include "esrc_camera.h"


namespace tprDebug {//---------- namespace: tprDebug --------------//

namespace _inn {//-------- namespace: _inn ----------//

    //-- 简易开关，用来快速关闭 mes显示功能
    bool  is_mapEntSlice_show  {true}; 
    bool  is_pointPic_show     {false};

    std::vector<Mesh> renderPool_mapEntSlices {};
                //-- 因为所有 mapEntSlice.mesh 都统一放在 同一 z-depp层，
                //   所以不需要做 渲染排序
                //   每一 节点帧 CREATE的 mesh实例 直接存储在此处。
                //   下一次再删掉重建


    std::vector<Mesh> renderPool_pointPics {}; 
                //-- 很简单的 一个亮点 2*2像素
                // 坐标不对齐，用来辅助显示数据


}//-------------- namespace: _inn end --------//

/* ===========================================================
 *                    init_debug
 * -----------------------------------------------------------
 * -- 
 */
void init_debug(){

    //-- init slicePic
    slicePic.clear();
    for( int h=0; h<PIXES_PER_MAPENT; h++ ){
        for( int w=0; w<PIXES_PER_MAPENT; w++ ){
            if( (h==0) || (h==PIXES_PER_MAPENT-1) || (w==0) || (w==PIXES_PER_MAPENT-1) ){
                slicePic.push_back( CO );
            }else{
                slicePic.push_back( e_ );
            }
        }
    }



    //-- 生成 mapEntSlice 唯一的 texName
    texName_slice = create_a_texName( slicePicSize,
                                      (const GLvoid*)&(slicePic.at(0)) );

    //-- 生成 point 唯一的 texName
    texName_pointPic = create_a_texName( pointPicSize,
                                      (const GLvoid*)&(pointPic.at(0)) );

    //...
}


/* ===========================================================
 *               clear_mapEntSlices
 * -----------------------------------------------------------
 */
void clear_mapEntSlices(){
    if( _inn::is_mapEntSlice_show == true ){
        _inn::renderPool_mapEntSlices.clear();
    }
}

/* ===========================================================
 *               clear_pointPics
 * -----------------------------------------------------------
 */
void clear_pointPics(){
    if( _inn::is_pointPic_show == true ){
        _inn::renderPool_pointPics.clear();
    }
}


/* ===========================================================
 *               insert_new_mapEntSlice
 * -----------------------------------------------------------
 */
void insert_new_mapEntSlice( const MapCoord &mcpos_ ){

    if( _inn::is_mapEntSlice_show == false ){
        return;
    }

    // ***| INSERT FIRST, INIT LATER  |***
    Mesh  mesh {};
    _inn::renderPool_mapEntSlices.push_back( mesh ); //- copy
    Mesh &meshRef = _inn::renderPool_mapEntSlices.back();

    glm::vec2 fpos = glm_dvec2_2_vec2( mcpos_.get_dpos() );

    meshRef.init( texName_slice );
    meshRef.isVisible = true;
    meshRef.set_shader_program( &(esrc::get_shaderRef(ShaderType::OriginColor)) );
    meshRef.set_scale(glm::vec3{ static_cast<float>(PIXES_PER_MAPENT), 
                                static_cast<float>(PIXES_PER_MAPENT), 
                                1.0f });
    meshRef.set_translate(glm::vec3{ fpos.x,
                                     fpos.y,
                                    esrc::get_camera().get_zFar()+ViewingBox::debug_zOff
                                        });
}

/* ===========================================================
 *                insert_new_pointPic
 * -----------------------------------------------------------
 */
void insert_new_pointPic( const glm::vec2 &fpos_ ){

    if( _inn::is_pointPic_show == false ){
        return;
    }

    // ***| INSERT FIRST, INIT LATER  |***
    Mesh  mesh {};
    _inn::renderPool_pointPics.push_back( mesh ); //- copy
    Mesh &meshRef = _inn::renderPool_pointPics.back();

    meshRef.init( texName_pointPic );
    meshRef.isVisible = true;
    meshRef.set_shader_program( &(esrc::get_shaderRef(ShaderType::OriginColor)) );
    meshRef.set_scale(glm::vec3{ static_cast<float>(pointPicSize.x), 
                                static_cast<float>(pointPicSize.y), 
                                1.0f });
    meshRef.set_translate(glm::vec3{ fpos_.x,
                                     fpos_.y,
                                    esrc::get_camera().get_zFar()+ViewingBox::debug_zOff
                                        });
}



/* ===========================================================
 *                draw_renderPool_mapEntSlices
 * -----------------------------------------------------------
 */
void draw_renderPool_mapEntSlices(){
    if( _inn::is_mapEntSlice_show == false ){
        return;
    }
    for( auto &m : _inn::renderPool_mapEntSlices ){
        m.draw();
    }
}


/* ===========================================================
 *              draw_renderPool_pointPics
 * -----------------------------------------------------------
 */
void draw_renderPool_pointPics(){
    if( _inn::is_pointPic_show == false ){
        return;
    }
    for( auto &m : _inn::renderPool_pointPics ){
        m.draw();
    }
}


}//-------------------- namespace: tpr_debug end --------------//


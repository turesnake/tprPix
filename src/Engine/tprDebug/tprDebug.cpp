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
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//-------------------- Engine --------------------//
#include "config.h"
#include "slicePic.h"
#include "create_texNames.h"
#include "IntVec.h"
#include "ViewingBox.h"
#include "esrc_shader.h"
#include "esrc_camera.h"


namespace tprDebug {//---------- namespace: tprDebug --------------//

namespace {//-------- namespace ----------//

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


}//-------------- namespace: end --------//

/* ===========================================================
 *                    init_debug
 * -----------------------------------------------------------
 * -- 
 */
void init_debug(){

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
    if( is_mapEntSlice_show == true ){
        renderPool_mapEntSlices.clear();
    }
}

/* ===========================================================
 *               clear_pointPics
 * -----------------------------------------------------------
 */
void clear_pointPics(){
    if( is_pointPic_show == true ){
        renderPool_pointPics.clear();
    }
}


/* ===========================================================
 *               insert_new_mapEntSlice
 * -----------------------------------------------------------
 */
void insert_new_mapEntSlice( const MapCoord &_mcpos ){

    if( is_mapEntSlice_show == false ){
        return;
    }

    // ***| INSERT FIRST, INIT LATER  |***
    Mesh  mesh;
    renderPool_mapEntSlices.push_back( mesh ); //- copy
    Mesh &meshRef = renderPool_mapEntSlices.back();

    meshRef.init( texName_slice );
    meshRef.isVisible = true;
    meshRef.set_shader_program( &esrc::rect_shader );
    meshRef.set_translate(glm::vec3{ _mcpos.get_fpos().x,
                                     _mcpos.get_fpos().y,
                                    esrc::camera.get_zFar()+ViewingBox::debug_zOff
                                        });
    meshRef.set_scale(glm::vec3{ (float)PIXES_PER_MAPENT, 
                                (float)PIXES_PER_MAPENT, 
                                1.0f });
}

/* ===========================================================
 *                insert_new_pointPic
 * -----------------------------------------------------------
 */
void insert_new_pointPic( const glm::vec2 &_fpos ){

    if( is_pointPic_show == false ){
        return;
    }

    // ***| INSERT FIRST, INIT LATER  |***
    Mesh  mesh;
    renderPool_pointPics.push_back( mesh ); //- copy
    Mesh &meshRef = renderPool_pointPics.back();

    meshRef.init( texName_pointPic );
    meshRef.isVisible = true;
    meshRef.set_shader_program( &esrc::rect_shader );
    meshRef.set_translate(glm::vec3{ _fpos.x,
                                     _fpos.y,
                                    esrc::camera.get_zFar()+ViewingBox::debug_zOff
                                        });
    meshRef.set_scale(glm::vec3{ (float)(pointPicSize.x), 
                                (float)(pointPicSize.y), 
                                1.0f });
}



/* ===========================================================
 *                draw_renderPool_mapEntSlices
 * -----------------------------------------------------------
 */
void draw_renderPool_mapEntSlices(){
    if( is_mapEntSlice_show == false ){
        return;
    }
    for( auto &m : renderPool_mapEntSlices ){
        m.draw();
    }
}


/* ===========================================================
 *              draw_renderPool_pointPics
 * -----------------------------------------------------------
 */
void draw_renderPool_pointPics(){
    if( is_pointPic_show == false ){
        return;
    }
    for( auto &m : renderPool_pointPics ){
        m.draw();
    }
}


}//-------------------- namespace: tpr_debug end --------------//


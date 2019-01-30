/*
 * ========================= debug.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.23
 *                                        修改 -- 2019.01.23
 * ----------------------------------------------------------
 *    MOST  POOR DEBUG SYSTEM
 * ----------------------------
 */
#include "debug.h"

//--- glm - 0.9.8 ---
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
#include "srcs_engine.h"
#include "ViewingBox.h"


namespace debug {//---------- namespace: debug --------------//

namespace{//-------- namespace ----------//

    bool  is_mapEntSlice_show  {false}; //-- 简易开关，用来快速关闭 mes显示功能

    std::vector<Mesh> renderPool_mapEntSlices {};
                //-- 因为所有 mapEntSlice.mesh 都统一放在 同一 z-depp层，
                //   所以不需要做 渲染排序
                //   每一 节点帧 创建的 mesh实例 直接存储在此处。
                //   下一次再删掉重建
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

    //...
}


/* ===========================================================
 *               insert_new_mapEntSlice
 * -----------------------------------------------------------
 */
void clear_mapEntSlices(){
    
    if( is_mapEntSlice_show == false ){
        return;
    }

    renderPool_mapEntSlices.clear();
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
                                    esrc::camera.get_zFar()+ViewingBox::mapEntSlices_zOff
                                        });
    meshRef.set_scale(glm::vec3{ (float)PIXES_PER_MAPENT, 
                                (float)PIXES_PER_MAPENT, 
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





}//-------------------- namespace: debug end --------------//

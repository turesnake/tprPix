
//-------------------- Engine --------------------//
#include "MapSection.h"
#include "srcs_engine.h"


/* ===========================================================
 *                       map_byPass
 * -----------------------------------------------------------
 * -- 临时性的 创建一个 mapSection 实例。
 */
void map_byPass(){


    MapCoord  initPos { IntVec2{ 0,0 } }; //- 目标 mapSection 坐标
    MapSection *sectionPtr = esrc::insert_new_mapSection( initPos );

    sectionPtr->mapTex.init(); //- 自动通过 mapBuilder 函数来实现 mapTex 合成

    //-- bind mapTex - mesh --
    sectionPtr->mesh.init( sectionPtr->mapTex.get_texName() );
    sectionPtr->mesh.set_shader_program( &(esrc::rect_shader) );
    sectionPtr->mesh.is_visible = true;

    //- mapTex 直接坐标于 camera 的 远平面上
    //  此值 需要跟随 camera 每一帧都调整。主要是 camera.get_zFar() 这个值
    sectionPtr->refresh_translate_auto();
    
    sectionPtr->init();

}
















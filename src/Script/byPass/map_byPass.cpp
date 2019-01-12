
//-------------------- Engine --------------------//
#include "MapSection.h"
#include "srcs_engine.h"


/* ===========================================================
 *                       map_byPass
 * -----------------------------------------------------------
 * -- 临时性的 创建一个 mapSection 实例。
 */
void map_byPass(){

    // ***| INSERT FIRST, INIT LATER  |***

    MapSection section {};
    section.set_by_ppos( glm::vec2{ 0.0f, 0.0f } );
    u64 key = section.get_key();
    esrc::mapSections.insert({ key, section }); //- copy
    MapSection *sectionPtr = (MapSection*)&(esrc::mapSections.at(key));

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

















//-------------------- Engine --------------------//
#include "MapSection.h"
#include "srcs_engine.h"


/* ===========================================================
 *                       map_byPass
 * -----------------------------------------------------------
 * -- 
 */
void map_byPass(){

    /*
    // ***| INSERT FIRST, INIT LATER  |***
    MapTexture  mapTex;
    esrc::mapTexes.push_back( mapTex ); //- copy
    MapTexture *mapTexPtr = (MapTexture*)&(esrc::mapTexes.back());
    mapTexPtr->init();
    */

    // ***| INSERT FIRST, INIT LATER  |***
    MapSection section {};
    section.set_pos( glm::vec2{ 0.0f, 0.0f } );
    u64 key = section.get_key();
    esrc::mapSections.insert({ key, section }); //- copy
    MapSection *sectionPtr = (MapSection*)&(esrc::mapSections.at(key));

    sectionPtr->mapTex.init();

    //-- bind mapTex - mesh --
    sectionPtr->mesh.init( sectionPtr->mapTex.get_texName() );
    sectionPtr->mesh.set_shader_program( &(esrc::rect_shader) );
    sectionPtr->mesh.is_visible = true;

    //- mapTex 直接坐标于 camera 的 远平面上
    //  此值 需要跟随 camera 每一帧都调整。主要是 camera.get_zFar() 这个值
    sectionPtr->mesh.set_translate(glm::vec3{   sectionPtr->get_pixPos().x,
                                                sectionPtr->get_pixPos().y,
                                                esrc::camera.get_zFar() + 1.0f //-- 重要 --
                                                });
    
    //-- 此值一旦设置，不用再修改 --
    sectionPtr->mesh.set_scale(glm::vec3{   (float)(SECTION_W_ENTS * PIXES_PER_MAPENT),
                                            (float)(SECTION_H_ENTS * PIXES_PER_MAPENT),
                                            1.0f
                                            });


}
















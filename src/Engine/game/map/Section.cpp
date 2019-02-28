/*
 * ====================== Section.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    map section 一个地图区域。 左下坐标系
 *    ----------
 *    
 * ----------------------------
 */
#include "Section.h"

//-------------------- C --------------------//
//#include <cassert>

//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "srcs_engine.h"


//======== static vals ========//
int  Section::entSideLen { SECTION_SIDE_ENTS };
int  Section::pixSideLen { SECTION_SIDE_ENTS * PIXES_PER_MAPENT };


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void Section::init(){

    //--- mesh.scale ---
    mesh.set_scale(glm::vec3{   (float)(SECTION_SIDE_ENTS * PIXES_PER_MAPENT),
                                (float)(SECTION_SIDE_ENTS * PIXES_PER_MAPENT),
                                1.0f });

}


/* ===========================================================
 *                  refresh_translate_auto
 * -----------------------------------------------------------
 */
void Section::refresh_translate_auto(){
    const IntVec2 &ppos = mcpos.get_ppos();
    mesh.set_translate(glm::vec3{   (float)ppos.x,
                                    (float)ppos.y,
                                    esrc::camera.get_zFar() + ViewingBox::sections_zOff //-- MUST --
                                    });
}



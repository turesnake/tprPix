/*
 * ====================== MapSection.cpp =======================
 *                          -- tpr --
 *                                        创建 -- 2018.12.09
 *                                        修改 -- 2018.12.09
 * ----------------------------------------------------------
 *    map section 一个地图区域。 左下坐标系
 *    ----------
 *    
 * ----------------------------
 */
#include "MapSection.h"

//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "srcs_engine.h"


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void MapSection::init(){

    //--- mesh.scale ---
    mesh.set_scale(glm::vec3{   (float)(SECTION_W_ENTS * PIXES_PER_MAPENT),
                                (float)(SECTION_H_ENTS * PIXES_PER_MAPENT),
                                1.0f });

}


/* ===========================================================
 *                  refresh_translate_auto
 * -----------------------------------------------------------
 */
void MapSection::refresh_translate_auto(){
    const IntVec2 &ppos = pos.get_ppos();
    mesh.set_translate(glm::vec3{   (float)ppos.x,
                                    (float)ppos.y,
                                    esrc::camera.get_zFar() + ViewingBox::mapSections_zOff //-- MUST --
                                    });
}





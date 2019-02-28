/*
 * ====================== Chunk.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    map chunk 一个地图区域。 左下坐标系
 *    ----------
 *    
 * ----------------------------
 */
#include "Chunk.h"

//-------------------- C --------------------//
//#include <cassert>

//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "srcs_engine.h"


//======== static vals ========//
int  Chunk::entSideLen { ENTS_PER_CHUNK };
int  Chunk::pixSideLen { ENTS_PER_CHUNK * PIXES_PER_MAPENT };


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void Chunk::init(){

    //--- mesh.scale ---
    mesh.set_scale(glm::vec3{   (float)(ENTS_PER_CHUNK * PIXES_PER_MAPENT),
                                (float)(ENTS_PER_CHUNK * PIXES_PER_MAPENT),
                                1.0f });

}


/* ===========================================================
 *                  refresh_translate_auto
 * -----------------------------------------------------------
 */
void Chunk::refresh_translate_auto(){
    const IntVec2 &ppos = mcpos.get_ppos();
    mesh.set_translate(glm::vec3{   (float)ppos.x,
                                    (float)ppos.y,
                                    esrc::camera.get_zFar() + ViewingBox::chunks_zOff //-- MUST --
                                    });
}



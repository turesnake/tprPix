/*
 * ========================= renderPools_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "esrc_renderPool.h"

//#include "debug.h" //- tmp

namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                draw_renderPool_goMeshs_pic
 * -----------------------------------------------------------
 */
void draw_renderPool_goMeshs_pic(){

    for( auto &pairRef : esrc::renderPool_goMeshs_pic ){
        //-- 渲染持续，pos.z值小的 先渲染 --
        pairRef.second->draw();
    }
}

/* ===========================================================
 *                draw_renderPool_uiMeshs_pic
 * -----------------------------------------------------------
 */
void draw_renderPool_uiMeshs_pic(){

    for( auto &pairRef : esrc::renderPool_uiMeshs_pic ){
        //-- 渲染持续，pos.z值小的 先渲染 --
        pairRef.second->draw();
    }
}


/* ===========================================================
 *                draw_renderPool_goMeshs_shaodw
 * -----------------------------------------------------------
 */
void draw_renderPool_goMeshs_shadow(){
    
    for( auto &ptr : esrc::renderPool_goMeshs_shadow ){
        ptr->draw();
    }
}

/* ===========================================================
 *               draw_renderPool_mapSurfaces
 * -----------------------------------------------------------
 */
void draw_renderPool_mapSurfaces(){

    for( auto &ptr : esrc::renderPool_mapSurfaces ){
        ptr->draw();
    }
}


/* ===========================================================
 *                draw_renderPool_meshs
 * -----------------------------------------------------------
 */
void draw_renderPool_meshs(){

    for( auto &pairRef : esrc::renderPool_meshs ){
        pairRef.second->draw();
    }
}



}//---------------------- namespace: esrc -------------------------//


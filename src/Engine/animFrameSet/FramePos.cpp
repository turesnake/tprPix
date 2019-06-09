/*
 * ========================= FramePos.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.15
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "FramePos.h"

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- C --------------------//
//#include <cassert>

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h" 
#include "esrc_colliEntSet.h"

//#include "tprDebug.h" 


/* ===========================================================
 *          calc_ceh_mposOff_from_cesLB_2_centerMPos
 * -----------------------------------------------------------
 */
void FramePos::calc_ceh_mposOff_from_cesLB_2_centerMPos(){
    tprAssert( this->is_rootAnchorPos_set == true );
    this->colliEntHead.mposOff_from_cesLB_2_centerMPos = 
            esrc::get_colliEntSetRef( this->colliEntHead.colliEntSetIdx ).get_centerMPos();
}


/* ===========================================================
 *        calc_ceh_rootAnchorCompass_and_off_from_rootAnchor_2_mapEntMid
 * -----------------------------------------------------------
 */
void FramePos::calc_ceh_rootAnchorCompass_and_off_from_rootAnchor_2_mapEntMid(){

    const MapEntCompass &compass = esrc::get_colliEntSetRef( this->colliEntHead.colliEntSetIdx ).get_centerCompass();
    this->colliEntHead.rootAnchorCompass = compass;
    this->colliEntHead.off_from_rootAnchor_2_mapEntMid = glm::vec2{
                static_cast<float>(HALF_PIXES_PER_MAPENT - compass.x),
                static_cast<float>(HALF_PIXES_PER_MAPENT - compass.y) };
}


/* ===========================================================
 *                      check
 * -----------------------------------------------------------
 */
void FramePos::check(){

    //--- 确保关键数据已载入 ---//
    tprAssert( this->is_rootAnchorPos_set &&
            this->is_colliEntHead_set );

    //   目前什么都没做 ...
    
    //-- 检测 animFrameSet.rootAnchor 与 ceh 是否对齐 --//
    //const MapEntCompass &centerCompass = esrc::get_colliEntSetRef( this->colliEntHead.colliEntSetIdx ).get_centerCompass();
    //tprAssert( centerCompass == this->rootAnchorPos.compass ); //- 确保对齐 ！！！ --
                // 这一对齐，将使的，所有 frame中，rootAnchorPos点 都必须正好位于 其 ces 指定的 center 点上...

}


/*
 * ====================== MapTexture.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    texture for map sys
 * ----------------------------
 */
#include "MapTexture.h" 

//------------------- C --------------------//
#include <cassert>

//------------------- Engine --------------------//
#include "RGBA.h"
#include "create_texNames.h"


//----- static ------//
IntVec2 MapTexture::pixSize {   SECTION_SIDE_ENTS*PIXES_PER_MAPENT,  
                                SECTION_SIDE_ENTS*PIXES_PER_MAPENT};


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 * -- before calling init(). 
 * -- Please bind MapTexture::mapBuilder FIRST
 */
void MapTexture::creat_texName(){

    //---------------------------------------//
    //            mapTex build
    //---------------------------------------//
    //texBuf.resize( pixSize.x * pixSize.y );
    //assert( MapTexture::mapBuilder != nullptr );
    
    
    //---------------------------------------//
    //        gl --> texName
    //---------------------------------------//
    texName = create_a_texName( pixSize, (GLvoid*)&(texBuf.at(0)) );

    //-- 可以选择性释放 texBuf 资源 ---
    // 未定...
}




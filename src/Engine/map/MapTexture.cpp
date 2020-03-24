/*
 * ====================== MapTexture.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    texture for map sys
 * ----------------------------
 */
#include "pch.h"
#include "MapTexture.h" 

//------------------- Engine --------------------//
#include "RGBA.h"
#include "create_texNames.h"


namespace mapTexture_inn {//----------- namespace: mapTexture_inn ---------------//

    IntVec2  pixWH {    PIXES_PER_CHUNK_IN_TEXTURE<>, 
                        PIXES_PER_CHUNK_IN_TEXTURE<> };

}//-------------- namespace : end ---------------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 * -- before calling init(). 
 */
void MapTexture::creat_texName(){

    //---------------------------------------//
    //            mapTex build
    //---------------------------------------//
    //texBuf.resize( pixSize.x * pixSize.y );
    
    //---------------------------------------//
    //        gl --> texName
    //---------------------------------------//
    texName = create_a_texName( mapTexture_inn::pixWH, (GLvoid*)&(texBuf.at(0)) );

    //-- 可以选择性释放 texBuf 资源 ---
    // 未定...
}


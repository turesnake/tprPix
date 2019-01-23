/*
 * ====================== MapTexture.cpp =======================
 *                          -- tpr --
 *                                        创建 -- 2019.01.06
 *                                        修改 -- 2019.01.06
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
MapTexture::F_MAP_BUILDER  MapTexture::mapBuilder  {nullptr};


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 * -- before calling init(). 
 * -- Please bind MapTexture::mapBuilder FIRST
 */
void MapTexture::init(){

    //---------------------------------------//
    //            mapTex build
    //---------------------------------------//
    texBuf.resize( pixSize.x * pixSize.y );
    assert( MapTexture::mapBuilder != nullptr );
    
    MapTexture::mapBuilder( &(texBuf.at(0)),
                            pixSize.x,
                            pixSize.y );
    
    //---------------------------------------//
    //        gl --> texName
    //---------------------------------------//
    texName = create_a_texName( pixSize, (GLvoid*)&(texBuf.at(0)) );

    //-- 可以选择性释放 texBuf 资源 ---
    // 未定...
}


/* ===========================================================
 *                  bind_mapBuilder   [static]
 * -----------------------------------------------------------
 */
void MapTexture::bind_mapBuilder( F_MAP_BUILDER _fp ){
    mapBuilder = _fp;
}



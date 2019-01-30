/*
 * ====================== mapBuilder.cpp =======================
 *                          -- tpr --
 *                                        创建 -- 2019.01.06
 *                                        修改 -- 2019.01.06
 * ----------------------------------------------------------
 *   map texture builder
 * ----------------------------
 */

//-------------------- CPP --------------------//
#include <vector>
#include <functional>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//------------------- Engine --------------------//
#include "config.h"
#include "RGBA.h" 
#include "IntVec.h"


namespace{//---------- namespace ---------------//

    RGBA    *pixBufPtr;
    IntVec2  pixWH {0,0}; 

    using F_PIX  = std::function<void(int,int)>;
    void foreach_pix( F_PIX _fp );
    bool _is_black( int _w, int _h );

    //------
    RGBA color1 { 146, 129,  87, 255 }; //- 中黄
    RGBA color2 { 147, 138, 125, 255 }; //- 中灰
    RGBA red    { 240, 100, 100, 255 };

}//---------------- namespace: end ---------------//


/* ===========================================================
 *                     map_builder
 * -----------------------------------------------------------
 * -- tmp
 * -- 外部代码负责 _texBuf 已分配空间（resize）
 */
void map_builder( RGBA *_texBuf, int _pixes_w, int _pixes_h ){

    pixBufPtr = _texBuf;  //-- 需要改进...
    pixWH.set( _pixes_w, _pixes_h );
    //------

    foreach_pix(
        []( int w, int h ){
            if( _is_black( w,h ) == true ){
                pixBufPtr[ pixWH.x*h + w ] = color1;
            }else{
                pixBufPtr[ pixWH.x*h + w ] = color2;
            }
        }
    );
}


namespace{//---------- namespace ---------------//

    
/* ===========================================================
 *                     foreach_pix
 * -----------------------------------------------------------
 */
void foreach_pix( F_PIX _fp ){

    for( int h=0; h<pixWH.y; h++ ){
        for( int w=0; w<pixWH.x; w++ ){
            _fp( w, h );
        }
    }
}


/* ===========================================================
 *                     _is_black
 * -----------------------------------------------------------
 * -- 实现一个 棋盘格, 基于 mapEnt
 */
bool _is_black( int _w, int _h ){

    int w = _w/PIXES_PER_MAPENT;
    int h = _h/PIXES_PER_MAPENT;
    //---------
    if(h%2==0){
        if(w%2==0){
            return true;
        }
    }else{
        if(w%2!=0){
            return true;
        }
    }
    return false;
}




}//---------------- namespace: end ---------------//






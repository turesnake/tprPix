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
#include "RGBA.h" 
#include "IntVec.h"


namespace{//---------- namespace ---------------//

    RGBA    *pixBufPtr;
    IntVec2  pixWH {0,0}; 

    using F_PIX  = std::function<void(int,int)>;
    void foreach_pix( F_PIX _fp );
    bool _is_black( int _w, int _h );

    //------
    RGBA color1 { 150, 150, 200, 255 };
    RGBA color2 { 150, 200, 150, 255 };
    RGBA red    { 240, 100, 100, 255 };

}//---------------- namespace: end ---------------//


/* ===========================================================
 *                     map_builder
 * -----------------------------------------------------------
 * -- tmp
 * -- 外部代码负责 _texBuf 已分配空间（resize）
 */
void map_builder( u8*_texBuf, int _pixes_w, int _pixes_h ){

    pixBufPtr = (RGBA*)_texBuf;
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

    int w = _w/3;
    int h = _h/3;
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






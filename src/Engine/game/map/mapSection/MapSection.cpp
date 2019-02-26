/*
 * ====================== MapSection.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
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
#include "sectionBuilderTools.h" //- tmp


//======== static vals ========//
IntVec2  MapSection::entWH { SECTION_SIDE_ENTS, SECTION_SIDE_ENTS };
IntVec2  MapSection::pixWH { SECTION_SIDE_ENTS*PIXES_PER_MAPENT, 
                             SECTION_SIDE_ENTS*PIXES_PER_MAPENT };


namespace{//---------- namespace ---------------//

    RGBA *pixBufHeadPtr;     //- 放在此处是为了 支持 lambda

    using F_PIX  = std::function<void(int,int)>;
    void foreach_pix( F_PIX _fp );
    bool _is_black( int _w, int _h );

    //------
    RGBA color1 { 146, 129,  87, 255 }; //- 中黄
    RGBA color2 { 147, 138, 125, 255 }; //- 中灰
    RGBA red    { 240, 100, 100, 255 };

}//---------------- namespace: end ---------------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void MapSection::init(){

    //--- mesh.scale ---
    mesh.set_scale(glm::vec3{   (float)(SECTION_SIDE_ENTS * PIXES_PER_MAPENT),
                                (float)(SECTION_SIDE_ENTS * PIXES_PER_MAPENT),
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



/* ===========================================================
 *                  build_new_section
 * -----------------------------------------------------------
 * section 生成器 很可以要被拓展为一个 巨大的模块...
 * 暂时 先放在这。
 * -----------
 *  在最初版本，此函数不考虑性能
 * -----------
 * 生成器主要处理的两个对象：
 *  -- memMapEnts （先逐个生成数据）
 *  -- mapTex     （后一股脑制作）
 */ 
void MapSection::build_new_section(){


    mapTex.resize_texBuf();
    pixBufHeadPtr = mapTex.get_texBufHeadPtr(); 

    init_sectionBuilderTools( entWH, pixWH );

    

    //---------------------------//
    //  为每一个 mapent，生成 alti
    //---------------------------//


    
    //---------------------------//
    //  为每一个 mapent，划分 ecoSysType
    //  (确保晚于 alti 生成)
    //---------------------------//
    //...


    //---------------------------//
    //  为每一个 mapent，生成 Land-waters
    //  (确保晚于 生态群落)，因为不同的生态群落
    //  其水域生成方式不同
    //---------------------------//
    build_landOrWaters();
    

    
    //------


    /*
    for( int h=0; h<entWH.y; h++ ){
        for( int w=0; w<entWH.x; w++ ){
        }
    }
    */


    //---------------------------//
    //    一股脑生成 mapTex 
    //---------------------------//
    //...

    RGBA *pixPtr; //- texBuf 中每个像素点RGBA 的指针。

    size_t   pixIdx;
    size_t   entIdx;
    IntVec2  pixPPos; //- 像素所在 ppos
    IntVec2  pixMPos; //- 像素所在 entMPos 
    u8_t singleColor;
    //---
    for( int h=0; h<pixWH.y; h++ ){ //-- each pixel in texure
        for( int w=0; w<pixWH.x; w++ ){
            
            pixPPos.set( w, h );
            pixMPos = floorDiv( pixPPos, PIXES_PER_MAPENT );
            pixIdx = h*pixWH.x + w; 
            entIdx = pixMPos.y*entWH.x + pixMPos.x;
            //---
            (pointMap.at(entIdx)==LAND) ?
                singleColor = 160 :
                singleColor = 50;
            
            //mapTex
            pixPtr = pixBufHeadPtr + pixIdx;
            pixPtr->set( singleColor,
                         singleColor,
                         singleColor,
                         255 );
        
        }
    }
    


    //---------------------------//
    //   正式用 texture 生成 name
    //---------------------------//
    mapTex.creat_texName();

}




namespace{//---------- namespace ---------------//

/* ===========================================================
 *                     foreach_pix
 * -----------------------------------------------------------
 */
/*
void foreach_pix( F_PIX _fp ){
    for( int h=0; h<pixWH.y; h++ ){
        for( int w=0; w<pixWH.x; w++ ){
            _fp( w, h );
        }
    }
}
*/


/* ===========================================================
 *                     _is_black
 * -----------------------------------------------------------
 * -- 实现一个 棋盘格, 基于 mapEnt
 * -- 这个函数 仅用于 测试地图
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


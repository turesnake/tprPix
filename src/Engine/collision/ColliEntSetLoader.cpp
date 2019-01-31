/*
 * ========================= ColliEntSetLoader.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    
 * ----------------------------
 */
#include "ColliEntSetLoader.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//------------------- Libs --------------------//
#include "tprFileSys.h" 

//------------------- Engine --------------------//
#include "global.h"
#include "ColliEntSet_RGBAHandle.h"

//#include "debug.h" //- tmp


extern void load_and_divide_png( const std::string &_path,
                                const IntVec2 &_pixNum_per_frame,
                                const IntVec2 &_frameNum,
                                int            _totalFrameNum,
        std::vector< std::vector<RGBA>> &_frame_data_ary );

/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 * -- 解析数据，存入实例自身的 容器中
 */
void ColliEntSetLoader::init(){

    //----------------------------------------//
    //  load & divide png数据，存入每个 帧容器中
    //----------------------------------------//

    //-- 图元帧 数据容器组。帧排序为 [left-top] --
    std::vector< std::vector<RGBA> > frame_data_ary {}; 
    load_and_divide_png( tpr::path_combine( path_colliEntSet, lpath ),
                        pixNum_per_frame,
                        frameNum,
                        totalFrameNum,
                        frame_data_ary );
    
    //----------------------------//
    //   parse each frame data
    //----------------------------//
    int pixNum = pixNum_per_frame.x * pixNum_per_frame.y; //- 一帧有几个像素点
    ColliEntSet_RGBAHandle  ch {5};
    collientSets.resize( totalFrameNum );

    for( int f=0; f<totalFrameNum; f++ ){ //- each frame

        IntVec2 pixPPos; //- tmp. pos for each rgba Pix

        for( int p=0; p<pixNum; p++ ){ //- each frame.pix [left-bottom]

            ch.set_rgba( frame_data_ary.at(f).at(p) );
            if( ch.is_emply() == true ){
                continue; //- next frame.pix
            }

            pixPPos.set( p%pixNum_per_frame.x,
                        p/pixNum_per_frame.x );

            if( ch.is_center() == true ){
                collientSets.at(f).set_centerPPos( pixPPos );
                collientSets.at(f).set_radius( ch.get_radius_10() );
            }

            if( ch.is_colliEnt() == true ){
                collientSets.at(f).add_colliEnt( pixPPos );
            }
        }

        //-- 生成 crawl 的“新增集”／“减少集” --
        collientSets.at(f).create_adds_dels();
    }
}




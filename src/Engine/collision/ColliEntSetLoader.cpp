/*
 * ========================= ColliEntSetLoader.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.13
 *                                        修改 -- 2019.01.13
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

using std::string;
using std::vector;

//#include "debug.h" //- tmp

extern void load_and_divide_png( const std::string &_path,
                                 const PixVec2 &_frames,
                                 const PixVec2 &_pixes_per_frame,
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
                        frames,
                        pixes_per_frame,
                        frame_data_ary );
    
    //----------------------------//
    //   parse each frame data
    //----------------------------//
    int pixNums = pixes_per_frame.x * pixes_per_frame.y; //- 一帧有几个像素点
    ColliEntSet_RGBAHandle  ch {5};
    collientSets.resize( totalFrames );

    for( int j=0; j<totalFrames; j++ ){ //- each frame

        PixVec2 pix; //- tmp. pos for each rgba Pix

        for( int i=0; i<pixNums; i++ ){ //- each frame.pix [left-bottom]

            ch.set_rgba( frame_data_ary[j][i] );
            if( ch.is_emply() == true ){
                continue; //- next frame.pix
            }

            pix.x = i%pixes_per_frame.x;
            pix.y = i/pixes_per_frame.x;

            if( ch.is_center() == true ){
                collientSets.at(j).set_center( pix );
                collientSets.at(j).set_radius( ch.get_radius_10() );
            }

            if( ch.is_colliEnt() == true ){
                collientSets.at(j).add_colliEnt( pix );
            }
        }

        //-- 生成 crawl 的“新增集”／“减少集” --
        collientSets.at(j).create_adds_dels();
    }
    //--- 基础查错 -----
    //...
}




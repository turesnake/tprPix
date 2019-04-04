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
#include <cassert>

//------------------- Libs --------------------//
#include "tprFileSys.h" 

//------------------- Engine --------------------//
#include "global.h"
#include "ColliEntSet_RGBAHandle.h"

//#include "debug.h" //- tmp


extern IntVec2 load_and_divide_png( const std::string &_path,
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
    this->pixNum_per_frame = load_and_divide_png( tpr::path_combine( path_colliEntSet, this->lpath ),
                                            this->frameNum,
                                            this->totalFrameNum,
                                            frame_data_ary );

    assert( (this->pixNum_per_frame.x==5*PIXES_PER_MAPENT) && 
            (this->pixNum_per_frame.y==5*PIXES_PER_MAPENT) ); //- tmp
        //cout << "frame_data_ary.size() = " << frame_data_ary.size();
        
    //----------------------------//
    //   parse each frame data
    //----------------------------//
    int pixNum = this->pixNum_per_frame.x * 
                 this->pixNum_per_frame.y; //- 一帧有几个像素点
    ColliEntSet_RGBAHandle  ch {5};
    this->collientSets.resize( this->totalFrameNum );

    IntVec2 pixPPos; //- tmp. pos for each rgba Pix

    for( int f=0; f<this->totalFrameNum; f++ ){ //- each frame
        for( int p=0; p<pixNum; p++ ){ //- each frame.pix [left-bottom]

            ch.set_rgba( frame_data_ary.at(f).at(p) );
            if( ch.is_emply() == true ){
                continue; //- next frame.pix
            }

            pixPPos.set( p%this->pixNum_per_frame.x,
                         p/this->pixNum_per_frame.x );

            if( ch.is_center() == true ){
                this->collientSets.at(f).set_centerPPos( pixPPos );
                this->collientSets.at(f).set_radius( ch.get_radius_10() );
            }

            if( ch.is_colliEnt() == true ){
                this->collientSets.at(f).add_colliEnt( pixPPos );
            }
        }

        //-- 生成 crawl 的“新增集”／“减少集” --
        this->collientSets.at(f).create_adds_dels();
    }
}




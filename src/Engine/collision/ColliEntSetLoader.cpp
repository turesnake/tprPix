/*
 * ========================= ColliEntSetLoader.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "ColliEntSetLoader.h"

//------------------- Libs --------------------//
#include "tprGeneral.h"

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"
#include "ColliEntSet_RGBAHandle.h"
#include "load_and_divide_png.h"

#include "tprCast.h"

//#include "tprDebug.h" //- tmp


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
    this->pixNum_per_frame = load_and_divide_png( tprGeneral::path_combine( path_colliEntSet, this->lpath ),
                                            this->frameNum,
                                            this->totalFrameNum,
                                            frame_data_ary );

    tprAssert( (this->pixNum_per_frame.x==4*PIXES_PER_MAPENT) && 
            (this->pixNum_per_frame.y==3*PIXES_PER_MAPENT) ); //- tmp
        
    //----------------------------//
    //   parse each frame data
    //----------------------------//
    size_t pixNum = cast_2_size_t( this->pixNum_per_frame.x * 
                                    this->pixNum_per_frame.y ); //- 一帧有几个像素点

    ColliEntSet_RGBAHandle  ch {5};

    for( size_t i=0; i<this->totalFrameNum; i++ ){
        this->collientSets.push_back( std::make_shared<ColliEntSet>() );
    }

    IntVec2 pixPPos {}; //- tmp. pos for each rgba Pix

    for( size_t f=0; f<this->totalFrameNum; f++ ){ //- each frame
        for( size_t p=0; p<pixNum; p++ ){ //- each frame.pix [left-bottom]

            ch.set_rgba( frame_data_ary.at(f).at(p) );
            if( ch.is_emply() == true ){
                continue; //- next frame.pix
            }

            pixPPos.set( static_cast<int>(p)%this->pixNum_per_frame.x,
                         static_cast<int>(p)/this->pixNum_per_frame.x );

            if( ch.is_center() == true ){
                this->collientSets.at(f)->set_centerPPos( pixPPos );
            }

            if( ch.is_colliEnt() == true ){
                this->collientSets.at(f)->add_colliEnt( pixPPos );
            }
        }

        //-- 生成 crawl 的“新增集”／“减少集” --
        this->collientSets.at(f)->create_adds_dels();
    }
}



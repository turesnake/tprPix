/*
 * ==================== MapSurfaceRandSet.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "MapSurfaceRandSet.h"

//-------------------- CPP --------------------//
#include <string>


//------------------- Libs --------------------//
#include "tprDataType.h" 
#include "tprGeneral.h"


//-------------------- Engine --------------------//
#include "IntVec.h"
#include "RGBA.h"
#include "global.h"
#include "tprAssert.h"
#include "load_and_divide_png.h"
#include "tprCast.h"

#include "tprDebug.h"


namespace msRGBAH_inn{//---------- namespace ---------//
    
    RGBA  SmlPoint { 255, 255, 0, 255 };
    RGBA  MidPoint { 180, 100, 0, 255 };
    RGBA  BigPoint { 150,   0, 30, 255 };

}//------------ namespace: end ---------//



/* ===========================================================
 *                load_mapSurfaceRandSets
 * -----------------------------------------------------------
 * -- 每次游戏启动，从 png文件 读取数据，解析并存入 mem  [此函数不要求性能]
 */
void MapSurfaceRandSet::load_mapSurfaceRandSets(){

                cout << "---------- load_mapSurfaceRandSets: ----------\n";

    //-------------------------//
    //  手动设置 png 数据... tmp...
    //-------------------------//
    std::string lpath = "/mapSurfaceRandSet0.png";

    IntVec2 frameNum { 8, 4 };
    size_t totalFrameNum = cast_2_size_t( frameNum.x * frameNum.y );

    //----------------------------//
    //     读取 png 原始数据
    //----------------------------//
    std::vector< std::vector<RGBA> > frame_data_ary {};

    IntVec2 pixNum_per_frame = load_and_divide_png( tprGeneral::path_combine( path_mapSurfaceRandSet, lpath ),
                                                    frameNum,
                                                    totalFrameNum,
                                                    frame_data_ary );
    
    tprAssert(  (pixNum_per_frame.x==FIELDS_PER_CHUNK) &&
                (pixNum_per_frame.y==FIELDS_PER_CHUNK) );

    //----------------------------//
    //        parse
    //----------------------------//
    size_t                pixIdx {};
    mapSurfaceRandEntId_t id     {};

    for( const auto &frameRef : frame_data_ary ){ //- each frame in png

        id = this->id_manager.apply_a_u32_id();
        this->ents.insert({ id, MapSurfaceRandEnt{} });
        this->ids.push_back(id);
        auto &entRef = this->ents.at(id);
        //---
        for( size_t h=0; h<FIELDS_PER_CHUNK; h++ ){
            for( size_t w=0; w<FIELDS_PER_CHUNK; w++ ){//- each pix in png-frame
                pixIdx = h * FIELDS_PER_CHUNK + w;
                const RGBA &pix = frameRef.at(pixIdx);
                //---
                if( pix.a != 255 ){ continue; } //-- skip translucent --
                if(       pix.is_near(msRGBAH_inn::SmlPoint, 5) ){
                    entRef.datas.insert({ pixIdx, MapSurfaceRandLvl::Sml });
                    entRef.smlPointNum++;
                }else if( pix.is_near(msRGBAH_inn::MidPoint, 5) ){
                    entRef.datas.insert({ pixIdx, MapSurfaceRandLvl::Mid });
                    entRef.midPointNum++;
                }else if( pix.is_near(msRGBAH_inn::BigPoint, 5)  ){
                    entRef.datas.insert({ pixIdx, MapSurfaceRandLvl::Big });
                    entRef.bigPointNum++;
                }
            }
        } //-- each pix in png-frame --
                    /*
                    cout << "   datas.size() = " << entRef.datas.size()
                        << "; sml=" << entRef.smlPointNum
                        << "; mid=" << entRef.midPointNum
                        << "; big=" << entRef.bigPointNum
                        << endl;
                    */
    } //-- each frame in png end --
}



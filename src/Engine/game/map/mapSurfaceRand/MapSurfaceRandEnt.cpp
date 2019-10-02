/*
 * ==================== MapSurfaceRandEnt.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "MapSurfaceRandEnt.h"

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

namespace mapSurface {//------------------ namespace: mapSurface -------------------------//


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
void RandEntSet::load_mapSurfaceRandSets(){

                cout << "---------- load_mapSurfaceRandSets: ----------\n";

    //-------------------------//
    //  手动设置 png 数据... tmp...
    //-------------------------//
    std::string lpath = "/mapSurfaceRandSet2.png";

    IntVec2 frameNum { 4, 4 };
    size_t totalFrameNum = cast_2_size_t( frameNum.x * frameNum.y );

    //----------------------------//
    //     读取 png 原始数据
    //----------------------------//
    std::vector< std::vector<RGBA> > frame_data_ary {};

    IntVec2 pixNum_per_frame = load_and_divide_png( tprGeneral::path_combine( path_mapSurfaceRandSet, lpath ),
                                                    frameNum,
                                                    totalFrameNum,
                                                    frame_data_ary );
    
    tprAssert(  (pixNum_per_frame.x==ENTS_PER_CHUNK) &&
                (pixNum_per_frame.y==ENTS_PER_CHUNK) );

    //----------------------------//
    //        parse
    //----------------------------//
    randEntId_t id     {};
    size_t      fieldIdx {};
    size_t      entIdx {};
    size_t      entW {};
    size_t      entH {};
    glm::dvec2 dposOff {};

    for( const auto &frameRef : frame_data_ary ){ //- each frame in png

        id = RandEnt::id_manager.apply_a_u32_id();
        this->entUPtrs.insert({ id, std::make_unique<RandEnt>() });
        this->ids.push_back(id);
        auto &entRef = *(this->entUPtrs.at(id).get());
        //---
        // in png, each_pix map to each_mapent_in_chunk
        for( size_t fh=0; fh<FIELDS_PER_CHUNK; fh++ ){
            for( size_t fw=0; fw<FIELDS_PER_CHUNK; fw++ ){//- each field in chunk
                fieldIdx = fh * FIELDS_PER_CHUNK + fw;

                for( size_t eh=0; eh<ENTS_PER_FIELD; eh++ ){
                    for( size_t ew=0; ew<ENTS_PER_FIELD; ew++ ){//- each mapent in field
                        entW = fw * ENTS_PER_FIELD + ew;
                        entH = fh * ENTS_PER_FIELD + eh;
                        entIdx = entH * ENTS_PER_CHUNK + entW; // equal to pixIdx
                        const RGBA &pix = frameRef.at(entIdx);
                        dposOff.x = static_cast<double>(ew * PIXES_PER_MAPENT);
                        dposOff.y = static_cast<double>(eh * PIXES_PER_MAPENT);
                        //---
                        if( pix.a != 255 ){ continue; } //-- skip translucent --
                        if(       pix.is_near(msRGBAH_inn::SmlPoint, 5) ){

                            entRef.datas.insert({ fieldIdx, {RandEntLvl::Field_1f1, dposOff} });
                            entRef.smlPointNum++;
                        }else if( pix.is_near(msRGBAH_inn::MidPoint, 5) ){
                            entRef.datas.insert({ fieldIdx, {RandEntLvl::Field_2f2, dposOff} });
                            entRef.midPointNum++;
                        }else if( pix.is_near(msRGBAH_inn::BigPoint, 5)  ){
                            entRef.datas.insert({ fieldIdx, {RandEntLvl::Field_3f3, dposOff} });
                            entRef.bigPointNum++;
                        }

                    }
                }//- each mapent in field
            }
        }//- each field in chunk
    } //-- each frame in png end --


        //-- 在未来， mapSurfaceUpper 也需要 randEnt 分配样式。它们不应该和 low 的样式重叠
        //   我们可以在上诉 数据基础上，做一次翻转。生成一组完全不同的 数据，来服务于 uppre
        //   ...



}

}//---------------------- namespace: mapSurface -------------------------//


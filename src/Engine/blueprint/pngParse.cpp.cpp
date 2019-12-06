/*
 * ======================= pngParse.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "PlotBlueprint.h"

//------------------- CPP --------------------//
#include <vector>


//------------------- Libs --------------------//
#include "tprGeneral.h" 


//------------------- Engine --------------------//
#include "load_and_divide_png.h"
#include "tprMath.h"
#include "IntVec.h"
#include "RGBA.h"
#include "config.h"



namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace plotPng_inn {//-------- namespace: plotPng_inn --------------//


    //-- 图元帧 数据容器组。帧排序为 [left-top] --
    std::vector<std::vector<RGBA>> M_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> D_frame_data_ary {}; 


    std::string  path_M    {}; //-- map 
    std::string  path_D    {}; //-- dir & brokenLvl


    IntVec2  pixNum_per_frame_M {};  //- 单帧画面 的 长宽 像素值 （会被存到 animAction 实例中）
    IntVec2  pixNum_per_frame_D {};  // must equal to pixNum_per_frame_M !!!
    IntVec2  frameNum         {};  //- 画面中，横排可分为几帧，纵向可分为几帧
    size_t   totalFrameNum    {};  //- 目标png文件中，总 图元帧 个数

    //-- color --
    RGBA  uselessColor_1  { 179, 179, 179, 255 }; 
    // ...

    //===== funcs =====//
    void handle_frame(  MapData &mapDataRef_,
                    std::vector<RGBA> &M_frame_, 
                    std::vector<RGBA> &D_frame_,
                    bool isVillage_ );
    void build_two_paths( const std::string &path_M_ );


}//------------- namespace: plotPng_inn end --------------//


extern std::optional<std::pair<BrokenLvl,NineDirection>> rgba_2_DPngData( RGBA rgba_ )noexcept;


// 目前已支持 plot/yard 蓝图的 png 数据解析
// ret: 每一帧 长宽像素值
IntVec2 parse_png(  std::vector<MapData> &mapDatasRef_,
                    const std::string &pngPath_M_,
                    IntVec2 frameNum_,
                    size_t totalFrameNum_,
                    bool isVillage_ ){


    plotPng_inn::build_two_paths( pngPath_M_ );
    plotPng_inn::frameNum = frameNum_;
    plotPng_inn::totalFrameNum = totalFrameNum_;

    //----------------------------------------//
    //  load & divide png数据，存入每个 帧容器中
    //----------------------------------------//
    plotPng_inn::M_frame_data_ary.clear();
    plotPng_inn::D_frame_data_ary.clear();

    //-----------------------//
    //  read png data: M,D
    //-----------------------//       
    plotPng_inn::pixNum_per_frame_M = load_and_divide_png(plotPng_inn::path_M,
                                                        plotPng_inn::frameNum,
                                                        plotPng_inn::totalFrameNum,
                                                        plotPng_inn::M_frame_data_ary );

    plotPng_inn::pixNum_per_frame_D = load_and_divide_png(plotPng_inn::path_D,
                                                        plotPng_inn::frameNum,
                                                        plotPng_inn::totalFrameNum,
                                                        plotPng_inn::D_frame_data_ary );

    tprAssert( plotPng_inn::pixNum_per_frame_M == plotPng_inn::pixNum_per_frame_D );

    //-----------------------//
    //    parse png data
    //-----------------------// 
    for( size_t i=0; i<plotPng_inn::M_frame_data_ary.size(); i++  ){ // each frame
        auto &M_frameRef = plotPng_inn::M_frame_data_ary.at(i);
        auto &D_frameRef = plotPng_inn::D_frame_data_ary.at(i);
        //--
        mapDatasRef_.push_back( MapData{} ); // new empty ent
        auto &mdRef = mapDatasRef_.back();
        plotPng_inn::handle_frame( mdRef, M_frameRef, D_frameRef, isVillage_ );
    }

    //---
    return plotPng_inn::pixNum_per_frame_M;
}




namespace plotPng_inn {//-------- namespace: plotPng_inn --------------//



void handle_frame(  MapData &mapDataRef_,
                    std::vector<RGBA> &M_frame_, 
                    std::vector<RGBA> &D_frame_,
                    bool isVillage_ ){

    // 单帧 wh像素（每个像素，对应一个 mp）
    size_t W = cast_2_size_t(plotPng_inn::pixNum_per_frame_M.x);
    size_t H = cast_2_size_t(plotPng_inn::pixNum_per_frame_M.y);

    size_t pixIdx {};

    for( size_t j=0; j<H; j++ ){
        for( size_t i=0; i<W; i++ ){
            pixIdx = j * W + i;
            RGBA &m_rgba = M_frame_.at(pixIdx);
            RGBA &d_rgba = D_frame_.at(pixIdx);

            // M图为主，D图为辅。
            // 若M图判为 有效像素（非半透明，非辅助色）
            // 则要求 D图对应像素，也必须为 有效像素

            // 半透明，或者 辅助色，会被过滤掉
            if( !is_closeEnough<u8_t>(m_rgba.a, 255, 5) ||
                m_rgba.is_near(plotPng_inn::uselessColor_1, 5) ){
                continue; // skip
            }

            // D图对应像素，也要为有效色
            tprAssert(  !(!is_closeEnough<u8_t>(d_rgba.a, 255, 5) ||
                            d_rgba.is_near(plotPng_inn::uselessColor_1, 5)) );


            std::unique_ptr<MapDataEnt> entUPtr = std::make_unique<MapDataEnt>();
                
            auto outM = rgba_2_VariableTypeIdx( m_rgba );
            tprAssert( outM.has_value() );
            entUPtr->varTypeIdx = outM.value();
            entUPtr->mposOff = IntVec2{ i, j };

            if( isVillage_ ){
                // village png 的像素尺寸 是 field 为单位
                entUPtr->mposOff = IntVec2{ i*ENTS_PER_FIELD, j*ENTS_PER_FIELD };
            }else{
                // plot/yard png 的像素尺寸 是 mapent 为单位
                entUPtr->mposOff = IntVec2{ i, j };
            }

            auto outD = rgba_2_DPngData( d_rgba );
            tprAssert( outM.has_value() );
            entUPtr->brokenLvl = outD.value().first;
            entUPtr->direction = outD.value().second;

            //---
            mapDataRef_.data.push_back( std::move(entUPtr) ); // move
        }
    }
}




void build_two_paths( const std::string &path_M_ ){
    //- 注释 以 lpath_M = "/animal/dog_ack_01.M.png" 为例

    std::string lst {}; //- tmp, 尾部字符串，不停地被截断

    path_M = path_M_;

    //--------------------//
    //    生成 path_D
    //--------------------//
    auto point_idx = path_M.find( '.', 0 ); //- 指向第一个 '.'
    auto lastIt = path_M.begin();
    std::advance( lastIt, point_idx ); //- advance 并不防止 溢出
    //- lpath_D 暂时等于 "/animal/dog_ack_01"
    path_D.assign( path_M.begin(), lastIt );
    path_D += ".D.png";
}






}//------------- namespace: plotPng_inn end --------------//
}//--------------------- namespace: blueprint end ------------------------//


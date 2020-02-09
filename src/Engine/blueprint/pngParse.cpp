/*
 * ======================= pngParse.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "PlotBlueprint.h"
#include "YardBlueprint.h"

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
#include "blueprint_oth.h"

#include "tprDebug.h"

namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace plotPng_inn {//-------- namespace: plotPng_inn --------------//


    //-- 图元帧 数据容器组。帧排序为 [left-top] --
    std::vector<std::vector<RGBA>> M_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> D_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> FM_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> FD_frame_data_ary {}; 


    std::string  path_M    {}; //-- map 
    std::string  path_D    {}; //-- dir & brokenLvl
    std::string  path_FM   {}; //-- floor map 
    std::string  path_FD   {}; //-- floor dir & brokenLvl
    

    IntVec2  frameNum         {};  //- 画面中，横排可分为几帧，纵向可分为几帧
    size_t   totalFrameNum    {};  //- 目标png文件中，总 图元帧 个数

    //-- color --
    const std::vector<RGBA> uselessColors{
        { 220, 220, 220, 255 },
        { 180, 180, 180, 255 },
        { 120, 120, 120, 255 },
        { 100, 100, 100, 255 }
    };

    // ...

    //===== funcs =====//
    void handle_frame(  MapData &mapDataRef_,
                        IntVec2  pixNum_per_frame_,
                        std::vector<RGBA> &M_frame_, 
                        std::vector<RGBA> &D_frame_,
                        BlueprintType blueprintType_,
                        bool    isFloorGoData_ );

    void build_paths( const std::string &path_M_, bool isFourPaths_ );

    bool is_uselessColor( RGBA rgba_ )noexcept;


}//------------- namespace: plotPng_inn end --------------//


extern std::optional<std::pair<NineDirection, std::variant<std::monostate, BrokenLvl, FloorGoLayer>>> 
rgba_2_DPngData( RGBA rgba_, bool isBrokenLvl_ )noexcept;


// 目前已支持 plot/village 蓝图的 png 数据解析
// ret: 每一帧 长宽像素值
IntVec2 parse_png(  std::vector<MapData> &mapDatasRef_,
                    const std::string &pngPath_M_,
                    IntVec2 frameNum_,
                    size_t totalFrameNum_,
                    BlueprintType blueprintType_ ){

    tprAssert( blueprintType_ != BlueprintType::Yard );

    plotPng_inn::build_paths( pngPath_M_, false );
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
    IntVec2 pixNum_per_frame   {};     
    IntVec2 pixNum_per_frame_M = load_and_divide_png(plotPng_inn::path_M,
                                                        plotPng_inn::frameNum,
                                                        plotPng_inn::totalFrameNum,
                                                        plotPng_inn::M_frame_data_ary );

    IntVec2 pixNum_per_frame_D = load_and_divide_png(plotPng_inn::path_D,
                                                        plotPng_inn::frameNum,
                                                        plotPng_inn::totalFrameNum,
                                                        plotPng_inn::D_frame_data_ary );

    tprAssert( pixNum_per_frame_M == pixNum_per_frame_D );
    pixNum_per_frame = pixNum_per_frame_M;


    //-----------------------//
    //    parse png data
    //-----------------------// 
    for( size_t i=0; i<plotPng_inn::M_frame_data_ary.size(); i++  ){ // each frame
        auto &M_frameRef = plotPng_inn::M_frame_data_ary.at(i);
        auto &D_frameRef = plotPng_inn::D_frame_data_ary.at(i);
        //--
        mapDatasRef_.push_back( MapData{} ); // new empty ent
        auto &mdRef = mapDatasRef_.back();
        plotPng_inn::handle_frame( mdRef, pixNum_per_frame, M_frameRef, D_frameRef, blueprintType_, false );
    }

    //---
    pixNum_per_frame.x--;
    pixNum_per_frame.y--;
    tprAssert( (pixNum_per_frame.x>0) && (pixNum_per_frame.y>0) );
                // 蓝图 png 数据，每一帧在 top/right 两方向，都会延伸出去 1像素，用来做 frames 之间的 间隔
                // 提高视觉识别度
    return pixNum_per_frame;
}


// yard 要么拥有 majorGos蓝图，要么拥有 foorGos 蓝图（二者至少有一个，或都有）

IntVec2 parse_png_for_yard(  YardBlueprint &yardRef_,
                        const std::string &pngPath_M_,
                        const std::vector<size_t> &frameAllocateTimes_,
                        IntVec2 frameNum_,
                        size_t totalFrameNum_,
                        size_t fstFrameIdx_,
                        size_t frameNums_ ){


    tprAssert( fstFrameIdx_+frameNums_ <= totalFrameNum_ );

    plotPng_inn::build_paths( pngPath_M_, true );
    plotPng_inn::frameNum = frameNum_;
    plotPng_inn::totalFrameNum = totalFrameNum_;

    //----------------------------------------//
    //  load & divide png数据，存入每个 帧容器中
    //----------------------------------------//
    plotPng_inn::M_frame_data_ary.clear();
    plotPng_inn::D_frame_data_ary.clear();
    plotPng_inn::FM_frame_data_ary.clear();
    plotPng_inn::FD_frame_data_ary.clear();

    //-----------------------//
    //  read png data: M,D
    //-----------------------// 
    IntVec2  pixNum_per_frame   {}; 
    IntVec2  pixNum_per_frame_M {};   //- 单帧画面 的 长宽 像素值 （会被存到 animAction 实例中）
    IntVec2  pixNum_per_frame_D {};   // must equal to pixNum_per_frame_M !!!
    IntVec2  pixNum_per_frame_FM {};  // must equal to pixNum_per_frame_M !!!
    IntVec2  pixNum_per_frame_FD {};  // must equal to pixNum_per_frame_M !!!

    std::set<IntVec2> pixNum_per_frames {};
    if( yardRef_.get_isHaveMajorGos() ){
         
        pixNum_per_frame_M = load_and_divide_png(plotPng_inn::path_M,
                                                            plotPng_inn::frameNum,
                                                            plotPng_inn::totalFrameNum,
                                                            plotPng_inn::M_frame_data_ary );

        pixNum_per_frame_D = load_and_divide_png(plotPng_inn::path_D,
                                                            plotPng_inn::frameNum,
                                                            plotPng_inn::totalFrameNum,
                                                            plotPng_inn::D_frame_data_ary );

        pixNum_per_frames.insert( pixNum_per_frame_M ); // maybe
        pixNum_per_frames.insert( pixNum_per_frame_D ); // maybe
    }
    if( yardRef_.get_isHaveFloorGos() ){

        pixNum_per_frame_FM = load_and_divide_png(plotPng_inn::path_FM,
                                                            plotPng_inn::frameNum,
                                                            plotPng_inn::totalFrameNum,
                                                            plotPng_inn::FM_frame_data_ary );

        pixNum_per_frame_FD = load_and_divide_png(plotPng_inn::path_FD,
                                                            plotPng_inn::frameNum,
                                                            plotPng_inn::totalFrameNum,
                                                            plotPng_inn::FD_frame_data_ary );

        pixNum_per_frames.insert( pixNum_per_frame_FM ); // maybe
        pixNum_per_frames.insert( pixNum_per_frame_FD ); // maybe                                           
    }

    //--- 压入的值 一定是相同的
    tprAssert( pixNum_per_frames.size() == 1 );
    pixNum_per_frame = *pixNum_per_frames.begin();

    
    //-----------------------//
    //    parse png data
    //-----------------------// 
    // 并不读取全部数据，仅读取 参数指定的 那几帧

    std::vector<std::pair<YardBlueprint::mapDataId_t, MapData*>> majorEnts {};
    std::vector<std::pair<YardBlueprint::mapDataId_t, MapData*>> floorEnts {};

    if( yardRef_.get_isHaveMajorGos() ){

        for( size_t i=fstFrameIdx_; i<fstFrameIdx_+frameNums_; i++ ){

            auto &M_frameRef = plotPng_inn::M_frame_data_ary.at(i);
            auto &D_frameRef = plotPng_inn::D_frame_data_ary.at(i);
            //--
            auto majorEnt = yardRef_.create_new_majorGo_mapData( frameAllocateTimes_.at(i) );
            plotPng_inn::handle_frame( *(majorEnt.second), pixNum_per_frame, M_frameRef, D_frameRef, BlueprintType::Yard, false );
            majorEnts.push_back( majorEnt );
        }
    }

    if( yardRef_.get_isHaveFloorGos() ){

        for( size_t i=fstFrameIdx_; i<fstFrameIdx_+frameNums_; i++ ){

            auto &FM_frameRef = plotPng_inn::FM_frame_data_ary.at(i);
            auto &FD_frameRef = plotPng_inn::FD_frame_data_ary.at(i);
            //--
            auto floorEnt = yardRef_.create_new_floorGo_mapData( frameAllocateTimes_.at(i) );
            plotPng_inn::handle_frame( *(floorEnt.second), pixNum_per_frame, FM_frameRef, FD_frameRef, BlueprintType::Yard, true );
            floorEnts.push_back( floorEnt );
        }
    }

    // 同时包含时，两份数据要做绑定 IMPORTANT !!!
    if( yardRef_.get_isHaveMajorGos() && yardRef_.get_isHaveFloorGos() ){
        tprAssert( majorEnts.size() == floorEnts.size() );
        for( size_t i=0; i<majorEnts.size(); i++ ){
            const auto &majorEnt = majorEnts.at(i);
            const auto &floorEnt = floorEnts.at(i);
            yardRef_.bind_floorId_2_majorId( majorEnt.first, floorEnt.first );
        }
    }

    //---
    pixNum_per_frame.x--;
    pixNum_per_frame.y--;
    tprAssert( (pixNum_per_frame.x>0) && (pixNum_per_frame.y>0) );
                // 蓝图 png 数据，每一帧在 top/right 两方向，都会延伸出去 1像素，用来做 frames 之间的 间隔
                // 提高视觉识别度
    return pixNum_per_frame;
}





namespace plotPng_inn {//-------- namespace: plotPng_inn --------------//



void handle_frame(  MapData &mapDataRef_,
                    IntVec2  pixNum_per_frame_, // 此值 包含了 多出来的那 1像素间隙
                    std::vector<RGBA> &M_frame_, 
                    std::vector<RGBA> &D_frame_,
                    BlueprintType blueprintType_,
                    bool    isFloorGoData_ ){

    // 单帧 wh像素（每个像素，对应一个 mp）
    size_t W = cast_2_size_t(pixNum_per_frame_.x);
    size_t H = cast_2_size_t(pixNum_per_frame_.y);

    size_t pixIdx {};

    for( size_t j=0; j<H; j++ ){
        for( size_t i=0; i<W; i++ ){
            
            // 略过 top/right 方向的 1像素间隙
            if( (j==H-1) || (i==W-1) ){
                continue;
            }

            pixIdx = j * W + i;
            RGBA &m_rgba = M_frame_.at(pixIdx);
            RGBA &d_rgba = D_frame_.at(pixIdx);

            // M图为主，D图为辅。
            // 若M图判为 有效像素（非半透明，非辅助色）
            // 则要求 D图对应像素，也必须为 有效像素

            // 半透明，或者 辅助色，会被过滤掉
            if( !is_closeEnough<u8_t>(m_rgba.a, 255, 5) ||
                is_uselessColor(m_rgba) ){
                continue; // skip
            }

            // D图对应像素，也要为有效色
            tprAssert(  !(!is_closeEnough<u8_t>(d_rgba.a, 255, 5) ||
                            is_uselessColor(d_rgba)) );


            std::unique_ptr<MapDataEnt> entUPtr = std::make_unique<MapDataEnt>();
                
            auto outM = rgba_2_VariableTypeIdx( m_rgba );

                    // debug
                    if( !outM.has_value() ){
                        cout << "Illegal Pix Color:" 
                            << m_rgba.to_string()
                            << endl;
                    }

            tprAssert( outM.has_value() );
            entUPtr->varTypeIdx = outM.value();
            entUPtr->mposOff = IntVec2{ i, j };

            if( blueprintType_ == BlueprintType::Village ){
                // village png 的像素尺寸 是 field 为单位
                entUPtr->mposOff = IntVec2{ i*ENTS_PER_FIELD, j*ENTS_PER_FIELD };
            }else{
                // plot/yard png 的像素尺寸 是 mapent 为单位
                entUPtr->mposOff = IntVec2{ i, j };
            }

            bool isBrokenLvl = !((blueprintType_==BlueprintType::Yard) && isFloorGoData_);
            auto retOpt = rgba_2_DPngData( d_rgba, isBrokenLvl );

            tprAssert( retOpt.has_value() );
            entUPtr->direction = retOpt.value().first;
            entUPtr->brokenLvl_or_floorGoLayer = retOpt.value().second;
            //---
            mapDataRef_.data.push_back( std::move(entUPtr) ); // move
        }
    }
}


void build_paths( const std::string &path_M_, bool isFourPaths_ ){
    //- 注释 以 lpath_M = "/animal/dog_ack_01.M.png" 为例

    //--------------------//
    //    path_M
    //--------------------//
    path_M = path_M_;

    //--------------------//
    //    path_D
    //--------------------//
    auto point_idx = path_M.find( '.', 0 ); //- 指向第一个 '.'
    auto lastIt = path_M.begin();
    std::advance( lastIt, point_idx ); //- advance 并不防止 溢出
    //- path_D 暂时等于 "/animal/dog_ack_01"
    path_D.assign( path_M.begin(), lastIt );
    path_D += ".D.png";

    //-- 只需要 创建 M / D --
    if( !isFourPaths_ ){
        return;
    }

    //--------------------//
    //    path_FM
    //--------------------//
    //- path_FM 暂时等于 "/animal/dog_ack_01"
    path_FM.assign( path_M.begin(), lastIt );
    path_FM += ".FM.png";

    //--------------------//
    //    path_FD
    //--------------------//
    //- path_FD 暂时等于 "/animal/dog_ack_01"
    path_FD.assign( path_M.begin(), lastIt );
    path_FD += ".FD.png";
}




bool is_uselessColor( RGBA rgba_ )noexcept{
    for( const auto &i : uselessColors ){
        if( rgba_.is_near(i, 5) ){
            return true;
        }
    }
    return false;
}



}//------------- namespace: plotPng_inn end --------------//
}//--------------------- namespace: blueprint end ------------------------//


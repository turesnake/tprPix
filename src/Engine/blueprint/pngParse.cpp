/*
 * ======================= pngParse.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "PlotBlueprint.h"
#include "YardBlueprint.h"

//------------------- Engine --------------------//
#include "load_and_divide_png.h"
#include "RGBA.h"
#include "blueprint_oth.h"



namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace pngParse_inn {//-------- namespace: pngParse_inn --------------//

    // frame 中，将pix坐标 合并为一个 key值，方便索引
    using pixKey_t = uint64_t;
    pixKey_t pixPos_2_key( IntVec2 pos_ )noexcept;
    IntVec2 pixKey_2_pixPos( pixKey_t key_ )noexcept;


    // village: road RM 数据
    using roadDirKey_t = uint32_t; // bitmap, {...|B|T|R|L}
    std::unordered_map<roadDirKey_t, NineDirection> roadDirs {};
    bool  isInit_for_roadDirs {false};
    void init_for_roadDirs()noexcept;
    roadDirKey_t calc_roadDirKey( bool l_, bool r_, bool t_, bool b_ )noexcept;


    //-- 图元帧 数据容器组。帧排序为 [left-top] --
    std::vector<std::vector<RGBA>> M_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> D_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> FM_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> FD_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> RM_frame_data_ary {}; // 没有 RD


    std::string  path_M    {}; //-- map 
    std::string  path_D    {}; //-- dir & brokenLvl
    std::string  path_FM   {}; //-- floor map 
    std::string  path_FD   {}; //-- floor dir & brokenLvl
    std::string  path_RM   {}; //-- road map 
    

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

    void handle_RD_frame_for_village(   MapData &mapDataRef_,
                                    IntVec2  pixNum_per_frame_, /* 此值 包含了 多出来的那 1像素间隙 */
                                    std::vector<RGBA> &RM_frame_ );

    void build_paths( const std::string &path_M_ );

    bool is_uselessColor( RGBA rgba_ )noexcept;


}//------------- namespace: pngParse_inn end --------------//


extern std::optional<std::pair<NineDirection, BrokenLvl>> rgba_2_DPngData( RGBA rgba_ )noexcept;


// plot 蓝图的 png 数据解析
// ret: 每一帧 长宽像素值
IntVec2 parse_png_for_plot(  std::vector<MapData> &mapDatasRef_,
                    const std::string &pngPath_M_,
                    IntVec2 frameNum_,
                    size_t totalFrameNum_ ){

    pngParse_inn::build_paths( pngPath_M_ );
    pngParse_inn::frameNum = frameNum_;
    pngParse_inn::totalFrameNum = totalFrameNum_;

    //----------------------------------------//
    //  load & divide png数据，存入每个 帧容器中
    //----------------------------------------//
    pngParse_inn::M_frame_data_ary.clear();
    pngParse_inn::D_frame_data_ary.clear();

    //-----------------------//
    //  read png data: M,D
    //-----------------------//  
    IntVec2 pixNum_per_frame   {};     
    IntVec2 pixNum_per_frame_M = load_and_divide_png(pngParse_inn::path_M,
                                                        pngParse_inn::frameNum,
                                                        pngParse_inn::totalFrameNum,
                                                        pngParse_inn::M_frame_data_ary );

    IntVec2 pixNum_per_frame_D = load_and_divide_png(pngParse_inn::path_D,
                                                        pngParse_inn::frameNum,
                                                        pngParse_inn::totalFrameNum,
                                                        pngParse_inn::D_frame_data_ary );

    tprAssert( pixNum_per_frame_M == pixNum_per_frame_D );
    pixNum_per_frame = pixNum_per_frame_M;


    //-----------------------//
    //    parse png data
    //-----------------------// 
    for( size_t i=0; i<pngParse_inn::M_frame_data_ary.size(); i++  ){ // each frame
        auto &M_frameRef = pngParse_inn::M_frame_data_ary.at(i);
        auto &D_frameRef = pngParse_inn::D_frame_data_ary.at(i);
        //--
        auto &mdRef = mapDatasRef_.emplace_back(); // new empty ent
        //auto &mdRef = mapDatasRef_.back();
        pngParse_inn::handle_frame( mdRef, pixNum_per_frame, M_frameRef, D_frameRef, BlueprintType::Plot, false );
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

    pngParse_inn::build_paths( pngPath_M_ );
    pngParse_inn::frameNum = frameNum_;
    pngParse_inn::totalFrameNum = totalFrameNum_;

    //----------------------------------------//
    //  load & divide png数据，存入每个 帧容器中
    //----------------------------------------//
    pngParse_inn::M_frame_data_ary.clear();
    pngParse_inn::D_frame_data_ary.clear();
    pngParse_inn::FM_frame_data_ary.clear();
    pngParse_inn::FD_frame_data_ary.clear();

    //-----------------------//
    //  read png data: M,D
    //-----------------------// 
    IntVec2  pixNum_per_frame   {}; 
    std::set<IntVec2> pixNum_per_frames {};
    if( yardRef_.get_isHaveMajorGos() ){
         
        IntVec2 pixNum_per_frame_M = load_and_divide_png(pngParse_inn::path_M,
                                                            pngParse_inn::frameNum,
                                                            pngParse_inn::totalFrameNum,
                                                            pngParse_inn::M_frame_data_ary );

        IntVec2 pixNum_per_frame_D = load_and_divide_png(pngParse_inn::path_D,
                                                            pngParse_inn::frameNum,
                                                            pngParse_inn::totalFrameNum,
                                                            pngParse_inn::D_frame_data_ary );

        pixNum_per_frames.insert( pixNum_per_frame_M ); // maybe
        pixNum_per_frames.insert( pixNum_per_frame_D ); // maybe
    }
    if( yardRef_.get_isHaveFloorGos() ){

        IntVec2 pixNum_per_frame_FM = load_and_divide_png(pngParse_inn::path_FM,
                                                            pngParse_inn::frameNum,
                                                            pngParse_inn::totalFrameNum,
                                                            pngParse_inn::FM_frame_data_ary );

        IntVec2 pixNum_per_frame_FD = load_and_divide_png(pngParse_inn::path_FD,
                                                            pngParse_inn::frameNum,
                                                            pngParse_inn::totalFrameNum,
                                                            pngParse_inn::FD_frame_data_ary );

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

            auto &M_frameRef = pngParse_inn::M_frame_data_ary.at(i);
            auto &D_frameRef = pngParse_inn::D_frame_data_ary.at(i);
            //--
            auto majorEnt = yardRef_.create_new_majorGo_mapData( frameAllocateTimes_.at(i) );
            pngParse_inn::handle_frame( *(majorEnt.second), pixNum_per_frame, M_frameRef, D_frameRef, BlueprintType::Yard, false );
            majorEnts.push_back( majorEnt );
        }
    }

    if( yardRef_.get_isHaveFloorGos() ){

        for( size_t i=fstFrameIdx_; i<fstFrameIdx_+frameNums_; i++ ){

            auto &FM_frameRef = pngParse_inn::FM_frame_data_ary.at(i);
            auto &FD_frameRef = pngParse_inn::FD_frame_data_ary.at(i);
            //--
            auto floorEnt = yardRef_.create_new_floorGo_mapData( frameAllocateTimes_.at(i) );
            pngParse_inn::handle_frame( *(floorEnt.second), pixNum_per_frame, FM_frameRef, FD_frameRef, BlueprintType::Yard, true );
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



// village 蓝图的 png 数据解析
// ret: 每一帧 长宽像素值
IntVec2 parse_png_for_village(  std::vector<MapData> &mapDatasRef_,
                    const std::string &pngPath_M_,
                    IntVec2 frameNum_,
                    size_t totalFrameNum_,
                    bool isHaveRoad
                    ){


    if( !pngParse_inn::isInit_for_roadDirs ){
        pngParse_inn::isInit_for_roadDirs = true;
        pngParse_inn::init_for_roadDirs();
    }

    pngParse_inn::build_paths( pngPath_M_ );
    pngParse_inn::frameNum = frameNum_;
    pngParse_inn::totalFrameNum = totalFrameNum_;

    //----------------------------------------//
    //  load & divide png数据，存入每个 帧容器中
    //----------------------------------------//
    pngParse_inn::M_frame_data_ary.clear();
    pngParse_inn::D_frame_data_ary.clear();
    pngParse_inn::RM_frame_data_ary.clear();

    //-----------------------//
    //  read png data: M,D
    //-----------------------//  
    IntVec2 pixNum_per_frame   {};  
    std::set<IntVec2> pixNum_per_frames {};   
    IntVec2 pixNum_per_frame_M = load_and_divide_png(pngParse_inn::path_M,
                                                        pngParse_inn::frameNum,
                                                        pngParse_inn::totalFrameNum,
                                                        pngParse_inn::M_frame_data_ary );

    IntVec2 pixNum_per_frame_D = load_and_divide_png(pngParse_inn::path_D,
                                                        pngParse_inn::frameNum,
                                                        pngParse_inn::totalFrameNum,
                                                        pngParse_inn::D_frame_data_ary );
    pixNum_per_frames.insert( pixNum_per_frame_M );
    pixNum_per_frames.insert( pixNum_per_frame_D );

    //-----------------------//
    //  read png data: RM
    if( isHaveRoad ){
        IntVec2 pixNum_per_frame_RM = load_and_divide_png(  pngParse_inn::path_RM,
                                                    pngParse_inn::frameNum,
                                                    pngParse_inn::totalFrameNum,
                                                    pngParse_inn::RM_frame_data_ary );
        pixNum_per_frames.insert( pixNum_per_frame_RM ); // maybe
    }

    //--- 压入的值 一定是相同的
    tprAssert( pixNum_per_frames.size() == 1 );
    pixNum_per_frame = *pixNum_per_frames.begin();

    //-----------------------//
    //    parse png data
    //-----------------------// 
    for( size_t i=0; i<pngParse_inn::M_frame_data_ary.size(); i++  ){ // each frame
        auto &M_frameRef = pngParse_inn::M_frame_data_ary.at(i);
        auto &D_frameRef = pngParse_inn::D_frame_data_ary.at(i);
        //--
        auto &mdRef = mapDatasRef_.emplace_back(); // new empty ent
        //auto &mdRef = mapDatasRef_.back();
        pngParse_inn::handle_frame( mdRef, pixNum_per_frame, M_frameRef, D_frameRef, BlueprintType::Village, false );

        if( isHaveRoad ){
            auto &RM_frameRef = pngParse_inn::RM_frame_data_ary.at(i);
            pngParse_inn::handle_RD_frame_for_village( mdRef, pixNum_per_frame, RM_frameRef );
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




namespace pngParse_inn {//-------- namespace: pngParse_inn --------------//



void handle_frame(  MapData &mapDataRef_,
                    IntVec2  pixNum_per_frame_, // 此值 包含了 多出来的那 1像素间隙
                    std::vector<RGBA> &M_frame_, 
                    std::vector<RGBA> &D_frame_,
                    BlueprintType blueprintType_,
                    bool    isFloorGoData_ ){

    // 单帧 wh像素（每个像素，对应一个 mp）
    size_t W = cast_2_size_t(pixNum_per_frame_.x);
    size_t H = cast_2_size_t(pixNum_per_frame_.y);
    // 真实数据 长宽值，剪去了 多出来的那 1像素间隙
    size_t DW = W - 1;
    size_t DH = H - 1;

    size_t pixIdx {};

    for( size_t j=0; j<H; j++ ){
        for( size_t i=0; i<W; i++ ){
            
            // 略过 top/right 方向的 1像素间隙
            if( (j==DH) || (i==DW) ){
                continue;
            }

            pixIdx = j * W + i;
            RGBA &m_rgba = M_frame_.at(pixIdx);
            RGBA &d_rgba = D_frame_.at(pixIdx);

            // M图为主，D图为辅。
            // 若M图判为 有效像素（非半透明，非辅助色）
            // 则要求 D图对应像素，也必须为 有效像素

            // 半透明，或者 辅助色，会被过滤掉
            if( !is_closeEnough<uint8_t>(m_rgba.a, 255, 5) || is_uselessColor(m_rgba) ){
                continue; // skip
            }

            // D图对应像素，也要为有效色
            tprAssert(  !(!is_closeEnough<uint8_t>(d_rgba.a, 255, 5) || is_uselessColor(d_rgba)) );


            auto entUPtr = std::make_unique<MapDataEnt>();
                
            auto outM = rgba_2_VariableTypeIdx( m_rgba );

                    // debug
                    if( !outM.has_value() ){
                        tprDebug::console( "Illegal Pix Color: {}", m_rgba.to_string() );
                    }

            tprAssert( outM.has_value() );
            entUPtr->varTypeIdx = outM.value();

            if( blueprintType_ == BlueprintType::Village ){
                // village png 的像素尺寸 是 field 为单位
                entUPtr->mposOff = IntVec2{ static_cast<int>(i)*ENTS_PER_FIELD<>, 
                                            static_cast<int>(j)*ENTS_PER_FIELD<> };
            }else{
                // plot/yard png 的像素尺寸 是 mapent 为单位
                entUPtr->mposOff = IntVec2{ i, j };
            }

            auto retOpt = rgba_2_DPngData( d_rgba );
            tprAssert( retOpt.has_value() );
            entUPtr->direction = retOpt.value().first;
            entUPtr->brokenLvl = retOpt.value().second;
            //---
            mapDataRef_.data.push_back( std::move(entUPtr) ); // move
        }
    }
}





void handle_RD_frame_for_village(   MapData &mapDataRef_,
                                    IntVec2  pixNum_per_frame_, /* 此值 包含了 多出来的那 1像素间隙 */
                                    std::vector<RGBA> &RM_frame_ ){

    // "Frame Size" - 单帧 wh像素（每个像素，对应一个 mp） 包含了 多出来的那 1像素间隙
    size_t FW = cast_2_size_t(pixNum_per_frame_.x);
    size_t FH = cast_2_size_t(pixNum_per_frame_.y);
    // "Data Size" - 真实数据 长宽值，剪去了 多出来的那 1像素间隙
    size_t DW = FW - 1;
    size_t DH = FH - 1;

    size_t mapPixIdx {};
    size_t fPixIdx {}; // in frame 

    // 记录 有效 road-yard node
    std::set<pixKey_t> nodes {};

    //-----------------------------------//
    // -1- 收集 RM_frame_ 中，所有有效点，进入 map
    for( size_t j=0; j<FH; j++ ){
        for( size_t i=0; i<FW; i++ ){

            // 略过 top/right 方向的 1像素间隙
            if( (j>=DH) || (i>=DW) ){
                continue;
            }

            mapPixIdx = j * DW + i;  // map 内 idx
            fPixIdx = j * FW + i;
            RGBA &m_rgba = RM_frame_.at(fPixIdx);

            // 半透明，或者 辅助色，会被过滤掉
            if( !is_closeEnough<uint8_t>(m_rgba.a, 255, 5) || is_uselessColor(m_rgba) ){
                continue; // skip
            }

            auto outM = rgba_2_VariableTypeIdx( m_rgba );
                    // debug
                    if( !outM.has_value() ){
                        tprDebug::console( "Illegal Pix Color: {}", m_rgba.to_string() );
                    }
            tprAssert( outM.has_value() );

            // 现在已经确定，目标点是 有效的 road-yard 点了

            // 禁止在 蓝图边缘一圈 绘制 road 点
            tprAssert( (i!=0) && (i!=DW-1) && (j!=0) && (j!=DH-1));

            nodes.insert( pixPos_2_key( IntVec2{ i,j } ) );
        }
    }

    //-----------------------------------//
    // -2- 为 map 中每一个有效点（true），计算 D值
    pixKey_t leftKey {};
    pixKey_t rightKey {};
    pixKey_t topKey {};
    pixKey_t bottomKey {};

    for( const auto &key : nodes ){
        IntVec2 pixPos = pixKey_2_pixPos( key );
        size_t i = cast_2_size_t( pixPos.x );
        size_t j = cast_2_size_t( pixPos.y );

        mapPixIdx = j * DW + i;  // map 内 idx
        fPixIdx = j * FW + i;    // 注意！这里还是要用 frame-size: FW

                // 双重检测，禁止在 蓝图边缘一圈 绘制 road 点
                tprAssert( (i!=0) && (i!=DW-1) && (j!=0) && (j!=DH-1)); // tmp

        leftKey     = pixPos_2_key( IntVec2{ i-1, j } );
        rightKey    = pixPos_2_key( IntVec2{ i+1, j } );
        topKey      = pixPos_2_key( IntVec2{ i, j+1 } );
        bottomKey   = pixPos_2_key( IntVec2{ i, j-1 } );

        roadDirKey_t roadDirKey = calc_roadDirKey(  (nodes.find(leftKey) != nodes.end()),
                                                    (nodes.find(rightKey) != nodes.end()),
                                                    (nodes.find(topKey) != nodes.end()),
                                                    (nodes.find(bottomKey) != nodes.end()) );
        NineDirection roadDir = roadDirs.at( roadDirKey );

        //------- 正式为这个 pix 制作数据 ----------
        auto entUPtr = std::make_unique<MapDataEnt>();

        RGBA &m_rgba = RM_frame_.at(fPixIdx);

        auto outM = rgba_2_VariableTypeIdx( m_rgba );
        tprAssert( outM.has_value() );

        entUPtr->varTypeIdx = outM.value();    
        entUPtr->mposOff = IntVec2{ static_cast<int>(i)*ENTS_PER_FIELD<>, 
                                    static_cast<int>(j)*ENTS_PER_FIELD<> }; // village png 的像素尺寸 是 field 为单位
        entUPtr->direction = roadDir;
        entUPtr->brokenLvl = BrokenLvl::Lvl_0; // road 的 BrokenLvl 值一律为 Lvl_0
        //---
        mapDataRef_.data.push_back( std::move(entUPtr) ); // move
    }
}


roadDirKey_t calc_roadDirKey( bool l_, bool r_, bool t_, bool b_ )noexcept{
    roadDirKey_t v {0};
    if( l_ ){ v = v | ( 1 << 1 ); }
    if( r_ ){ v = v | ( 1 << 2 ); }
    if( t_ ){ v = v | ( 1 << 3 ); }
    if( b_ ){ v = v | ( 1 << 4 ); }
    return v;
}


void init_for_roadDirs()noexcept{
    
    tprAssert( roadDirs.empty() ); // 确保不被重复调用

    //-- 1 val --
    roadDirs.insert({ calc_roadDirKey( true, false, false, false ), NineDirection::Bottom });
    roadDirs.insert({ calc_roadDirKey( false, true, false, false ), NineDirection::Bottom });
    roadDirs.insert({ calc_roadDirKey( false, false, true, false ), NineDirection::Left });
    roadDirs.insert({ calc_roadDirKey( false, false, false, true ), NineDirection::Left });

    //-- 2 vals --
    // 4 拐角
    roadDirs.insert({ calc_roadDirKey( true, false, true, false ), NineDirection::RightBottom });
    roadDirs.insert({ calc_roadDirKey( true, false, false, true ), NineDirection::RightTop });
    roadDirs.insert({ calc_roadDirKey( false, true, true, false ), NineDirection::LeftBottom });
    roadDirs.insert({ calc_roadDirKey( false, true, false, true ), NineDirection::LeftTop });
    // 2 直线
    roadDirs.insert({ calc_roadDirKey( true, true, false, false ), NineDirection::Bottom });
    roadDirs.insert({ calc_roadDirKey( false, false, true, true ), NineDirection::Left });

    //-- 3 vals --
    roadDirs.insert({ calc_roadDirKey( false, true, true, true ), NineDirection::Right });
    roadDirs.insert({ calc_roadDirKey( true, false, true, true ), NineDirection::Left });
    roadDirs.insert({ calc_roadDirKey( true, true, false, true ), NineDirection::Bottom });
    roadDirs.insert({ calc_roadDirKey( true, true, true, false ), NineDirection::Top });
}



void build_paths( const std::string &path_M_ ){
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

    //--------------------//
    //    path_RM
    //--------------------//
    //- path_RM 暂时等于 "/animal/dog_ack_01"
    path_RM.assign( path_M.begin(), lastIt );
    path_RM += ".RM.png";
}



// r/g/b 三色相同的颜色（纯灰）即被判定为 辅助色
bool is_uselessColor( RGBA rgba_ )noexcept{
    return (is_closeEnough<uint8_t>(rgba_.r, rgba_.g, 5) && is_closeEnough<uint8_t>(rgba_.r, rgba_.b, 5));
}



pixKey_t pixPos_2_key( IntVec2 pos_ )noexcept{
    pixKey_t key {};
    int *ptr = (int*)(&key); //- can't use static_cast<>
    *ptr = pos_.x;
    ptr++;
    *ptr = pos_.y; 
    //--------
    return key;
}

IntVec2 pixKey_2_pixPos( pixKey_t key_ )noexcept{
    IntVec2  pos {};
    int *ptr = (int*)&key_; //- can't use static_cast<>
    //---
    pos.x = *ptr;
    ptr++;
    pos.y = *ptr;
    //---
    return pos;
}



}//------------- namespace: pngParse_inn end --------------//
}//--------------------- namespace: blueprint end ------------------------//


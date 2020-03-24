/*
 * ==================== EcoObjBorder.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.14
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "EcoObjBorder.h"


//-------------------- Engine --------------------//
#include "global.h"
#include "load_and_divide_png.h"
#include "RGBA.h"


//======= static =======//
std::vector<std::unique_ptr<EcoObjBorder>> EcoObjBorder::frames {};



// 完全不考虑性能 
void EcoObjBorder::init(){

    //---------------//
    //  parse json
    //---------------//
    auto [frameNum, totalFrameNum, pngPath] = EcoObjBorder::parse_plotJsonFile();

    //---------------//
    //   read .png
    //---------------//
    std::vector<std::vector<RGBA>> frame_data_ary {};
    IntVec2  pixNum_per_frame {};  //- 单帧画面 的 长宽 像素值 （会被存到 animAction 实例中）

    pixNum_per_frame = load_and_divide_png( pngPath,
                                            frameNum,
                                            totalFrameNum,
                                            frame_data_ary );

    // 上侧，右侧 会额外多出来 1pix，用作 frame 间 间隙
    tprAssert(  pixNum_per_frame.x == (ENTS_PER_SECTION<>+1) && 
                pixNum_per_frame.y == (ENTS_PER_SECTION<>+1) );

    //---------------//
    //  parse png_data
    //---------------//
    RGBA leftBottomColor    { 153, 255, 153, 255 }; // V_10
    RGBA rightBottomColor   {   0, 170, 255, 255 }; // V_16
    RGBA leftTopColor       { 255, 128, 128, 255 }; // V_1
    RGBA rightTopColor      { 255, 255, 128, 255 }; // V_7

    NineDirection pixDir {};

    for( const auto &frameData : frame_data_ary ){

        EcoObjBorder &eobRef = EcoObjBorder::create_new_ecoObjBorder();

        for( int j=0; j<ENTS_PER_SECTION<>; j++ ){
            for( int i=0; i<ENTS_PER_SECTION<>; i++ ){ // eac pix

                size_t pixInIdx  = cast_2_size_t( j * (ENTS_PER_SECTION<>+1) + i ); // 输入：png帧中，pixIdx
                size_t pixOutIdx = cast_2_size_t( j * ENTS_PER_SECTION<> + i );     // 输出：成品数据帧中，pixIdx

                const RGBA &pix = frameData.at( pixInIdx );
                if(      pix.is_near(leftBottomColor, 5) ){  pixDir = NineDirection::LeftBottom; }
                else if( pix.is_near(rightBottomColor, 5) ){ pixDir = NineDirection::RightBottom; }
                else if( pix.is_near(leftTopColor, 5) ){     pixDir = NineDirection::LeftTop; }
                else if( pix.is_near(rightTopColor, 5) ){    pixDir = NineDirection::RightTop; }
                else{
                    tprAssert(0); // rightTop color
                }

                eobRef.frame.at(pixOutIdx) = pixDir;
            }
        }

    }


    //---------------//
    //    check
    //---------------//
    // 暂时不执行 shuffle ...

    tprAssert( !EcoObjBorder::frames.empty() );


}





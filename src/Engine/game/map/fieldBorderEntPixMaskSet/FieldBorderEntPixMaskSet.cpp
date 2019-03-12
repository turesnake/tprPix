/*
 * ==================== fieldBorderEntPixMasks.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "FieldBorderEntPixMaskSet.h"

//-------------------- C --------------------//
#include <cassert>

//------------------- Libs --------------------//
#include "tprFileSys.h" 

//-------------------- Engine --------------------//
#include "config.h"
#include "random.h"
#include "global.h"

#include "debug.h"


extern IntVec2 load_and_divide_png( const std::string &_path,
                          const IntVec2 &_frameNum,
                          int            _totalFrameNum,
        std::vector< std::vector<RGBA>> &_frame_data_ary );

namespace{//-------- namespace: --------------//


    std::default_random_engine  randEngine; //-通用 随机数引擎实例
    std::uniform_int_distribution<size_t> uDistribution( 0, 99 ); // [0,99]


    std::vector< std::vector<RGBA> > frame_data_ary {}; //- png原始数据

    //--- 单个 mapent 的 mask点集 容器 ---//
    //  以及经过 x轴，y轴 反转的 数据集
    std::vector<IntVec2> frameMasks; 
    std::vector<IntVec2> frameMasks_Xflip;
    std::vector<IntVec2> frameMasks_Yflip;
    std::vector<IntVec2> frameMasks_XYflip;


    RGBA maskColor { 255, 0, 0, 255 }; //- png数据中的 mask点色

}//------------- namespace: end --------------//




/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 */
void FieldBorderEntPixMaskSet::init(const std::string &_lpath,
                                    IntVec2  _frameNum,
                                    int      _totalFrameNum ){

    this->lpath = _lpath;
    this->frameNum = _frameNum;
    this->totalFrameNum = _totalFrameNum;
    //-----------
    randEngine.seed( get_new_seed() );

    //----------------------------//
    //     读取 png 原始数据
    //----------------------------//
    frame_data_ary.clear();
    this->pixNum_per_frame = load_and_divide_png( tpr::path_combine( path_fieldBorderEntPixMaskSet, this->lpath ),
                                                this->frameNum,
                                                this->totalFrameNum,
                                                frame_data_ary );

    assert( (this->pixNum_per_frame.x==PIXES_PER_MAPENT) &&
            (this->pixNum_per_frame.y==PIXES_PER_MAPENT) ); //- tmp

    this->maskSets_1_pix.clear();
    this->maskSets_2_pix.clear();
    this->maskSets_4_pix.clear();
    this->maskSets_7_pix.clear();


    //----------------------------//
    //       _1_pix 档 (用代码生成)
    //----------------------------//
    for( int h=0; h<PIXES_PER_MAPENT; h++ ){
        for( int w=0; w<PIXES_PER_MAPENT; w++ ){//- each pix in mapent
            frameMasks.clear();
            frameMasks.push_back( IntVec2{w,h} );//- copy
            this->maskSets_1_pix.push_back( frameMasks );//- copy             
        }
    }

    //----------------------------//
    //      
    //----------------------------//
    size_t      pixIdx;
    maskSet_t  *maskSetPtr;
    IntVec2     flipPixWH;   //- 原始数据 翻转后的 相对坐标

    for( const auto &frameRef : frame_data_ary ){ //- each frame in png

        frameMasks.clear();
        frameMasks_Xflip.clear();
        frameMasks_Yflip.clear();
        frameMasks_XYflip.clear();

        //--- 先将 png-frame 原始数据 收集起来 ---
        for( size_t h=0; h<PIXES_PER_MAPENT; h++ ){
            for( size_t w=0; w<PIXES_PER_MAPENT; w++ ){ //- each pix in png-frame (mapent) 
                pixIdx = h*PIXES_PER_MAPENT + w;

                    /*
                    const RGBA &rgbaRef = frameRef.at(pixIdx);
                    cout << "  " << (int)rgbaRef.r
                        << ", " << (int)rgbaRef.g
                        << ", " << (int)rgbaRef.b
                        << ", " << (int)rgbaRef.a
                        << endl;
                    */
                    

                if( is_rgba_near( frameRef.at(pixIdx), maskColor, 5 ) ){
                    frameMasks.push_back( IntVec2{ (int)w, (int)h } );
                }
            }
        }

        //--- 根据 点的数量，确定 主容器 ---//
        if( frameMasks.size() <= 2 ){        //- _2_pix
            maskSetPtr = &(this->maskSets_2_pix);
        }else if( frameMasks.size() <= 5 ){  //- _4_pix (暂时合并了5pix的) 
            maskSetPtr = &(this->maskSets_4_pix);
        }else{                               //- _7_pix
            maskSetPtr = &(this->maskSets_7_pix);
        }

        //---
        for( auto &pixWH : frameMasks ){
            flipPixWH.x = PIXES_PER_MAPENT - 1 - pixWH.x;
            flipPixWH.y = PIXES_PER_MAPENT - 1 - pixWH.y;
            //-----
            frameMasks_Xflip.push_back(  IntVec2{ flipPixWH.x, pixWH.y  } );     //- copy
            frameMasks_Yflip.push_back(  IntVec2{ pixWH.x,     flipPixWH.y  } ); //- copy
            frameMasks_XYflip.push_back( IntVec2{ flipPixWH.x, flipPixWH.y  } ); //- copy
        }

        maskSetPtr->push_back( frameMasks );        //- copy
        maskSetPtr->push_back( frameMasks_Xflip );  //- copy
        maskSetPtr->push_back( frameMasks_Yflip );  //- copy
        maskSetPtr->push_back( frameMasks_XYflip ); //- copy
    } //---- each frame in png end ----


            cout << "\nmaskSets_1_pix.size = " << this->maskSets_1_pix.size() 
                <<  "\nmaskSets_2_pix.size = " << this->maskSets_2_pix.size() 
                <<  "\nmaskSets_4_pix.size = " << this->maskSets_4_pix.size() 
                <<  "\nmaskSets_7_pix.size = " << this->maskSets_7_pix.size() 
                << endl;

}


/* ===========================================================
 *                get_rand_maskSet
 * -----------------------------------------------------------
 */
const std::vector<IntVec2> &FieldBorderEntPixMaskSet::get_rand_maskSet( FieldBorderType _borderType ){
    maskSet_t  *maskSetPtr;
    switch(_borderType){
        case FieldBorderType::Nearest:
            maskSetPtr = &(this->maskSets_7_pix);
            break;
        case FieldBorderType::Sec:
            maskSetPtr = &(this->maskSets_4_pix);
            break;
        case FieldBorderType::Thd:
            maskSetPtr = &(this->maskSets_2_pix);
            break;
        case FieldBorderType::Inner:
            maskSetPtr = &(this->maskSets_1_pix);
            break;
        default:
            assert(0); //- never touch
    }
    //---------
    size_t randVal = uDistribution( randEngine );
    size_t idx = randVal % (maskSetPtr->size()); //- mod
    return  maskSetPtr->at(idx);
}






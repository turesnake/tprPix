/*
 * ==================== fieldBorderSet_Handle.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "fieldBorderSet_Handle.h"


//-------------------- C --------------------//
//#include <cassert>


//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>

//------------------- Libs --------------------//
#include "tprDataType.h" 
//#include "tprFileSys.h" 
#include "tprGeneral.h"


//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "RGBA.h"
#include "global.h"
#include "config.h"
#include "IntVec.h"
#include "FieldBorderSet.h"
#include "load_and_divide_png.h"

#include "tprCast.h"



namespace {//-------- namespace: --------------//

    std::string  lpath {};
    IntVec2      pixNum_per_frame {};   //- 单帧画面 的 长宽 像素值
    IntVec2      frameNum {};         //- 画面中，横排可分为几帧，纵向可分为几帧
    size_t       totalFrameNum {};    //- 总 图元帧 个数 

    //- png原始数据
    std::vector< std::vector<RGBA> > frame_data_ary {};


    FieldBorderSet::quadContainer_t frameDatas {}; //- png文件中，每一帧 的数据


    RGBA pixColor { 0, 0, 255, 255 }; //- png数据 中的 主色



}//------------- namespace: end --------------//



/* ===========================================================
 *                   load_fieldBorderSets
 * -----------------------------------------------------------
 * -- 每次游戏启动，从 png文件 读取数据，解析并存入 mem  [此函数不要求性能]
 */
void load_fieldBorderSets(){

    //-------------------------//
    //  手动设置 png 数据... tmp...
    //-------------------------//
    lpath = "/fieldBorderSet-4.png";

    frameNum.set( 4, 2 );
    totalFrameNum = to_size_t_cast( frameNum.x * frameNum.y );

    //----------------------------//
    //     读取 png 原始数据
    //----------------------------//
    frame_data_ary.clear();

    pixNum_per_frame = load_and_divide_png( tprGeneral::path_combine( path_fieldBorderSet, lpath ),
                                                frameNum,
                                                totalFrameNum,
                                                frame_data_ary );
    
    tprAssert( (pixNum_per_frame.x==PIXES_PER_FIELD_BORDER_SET ) &&
            (pixNum_per_frame.y==PIXES_PER_FIELD_BORDER_SET) );

    //----------------------------//
    //        parse
    //----------------------------//
    size_t                pixIdx;

    for( const auto &frameRef : frame_data_ary ){ //- each frame in png

        frameDatas.clear();
        frameDatas.resize( to_size_t_cast(pixNum_per_frame.x * pixNum_per_frame.y) );

        //--- 将 原版数据 传入 预制件实例 ---//
        clear_for_fieldBorderSet();
        for( size_t h=0; h<PIXES_PER_FIELD_BORDER_SET; h++ ){
            for( size_t w=0; w<PIXES_PER_FIELD_BORDER_SET; w++ ){ //- each pix in png-frame (2*2chunk)
                pixIdx = h * PIXES_PER_FIELD_BORDER_SET + w;

                if( rgba::is_rgba_near(frameRef.at(pixIdx), pixColor, 5) ){ //- 主色点
                    frameDatas.at(pixIdx) = 1;
                }else{
                    frameDatas.at(pixIdx) = 0;
                }
            }
        } //-- each pix in png-frame (2*2chunk) end --

        copy_originData_for_fieldBorderSet( frameDatas );
        build_all_mutant_datas_for_fieldBorderSet();
    } //-- each frame in png end --
}




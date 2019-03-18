/*
 * ==================== landWaterPrefab_Handle.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "landWaterPrefab_Handle.h"

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>

//------------------- Libs --------------------//
#include "tprDataType.h" 
#include "tprFileSys.h" 


//-------------------- Engine --------------------//
#include "landWaterPrefabId_t.h"
#include "RGBA.h"
#include "global.h"
#include "config.h"
#include "LandWaterPrefabCorner.h"
#include "LandWaterPrefabEdge.h"

#include "debug.h"


extern IntVec2 load_and_divide_png( const std::string &_path,
                          const IntVec2 &_frameNum,
                          int            _totalFrameNum,
        std::vector< std::vector<RGBA>> &_frame_data_ary );

namespace{//-------- namespace: --------------//
    //-- corner png --
    std::string  lpath_corner {};
    IntVec2      pixNum_per_frame_corner {};   //- 单帧画面 的 长宽 像素值
    IntVec2      frameNum_corner {};         //- 画面中，横排可分为几帧，纵向可分为几帧
    int          totalFrameNum_corner {};    //- 总 图元帧 个数    

    //-- edge png --
    std::string  lpath_edge {};
    IntVec2      pixNum_per_frame_edge {};   //- 单帧画面 的 长宽 像素值
    IntVec2      frameNum_edge {};         //- 画面中，横排可分为几帧，纵向可分为几帧
    int          totalFrameNum_edge {};    //- 总 图元帧 个数 

    //- png原始数据
    std::vector< std::vector<RGBA> > frame_data_ary_corner {};
    std::vector< std::vector<RGBA> > frame_data_ary_edge {};


    RGBA fstColor { 255, 0, 0, 255 }; //- png数据 中的 主色
    RGBA secColor { 0, 0, 255, 255 }; //- png数据 中的 副色

}//------------- namespace: end --------------//




/* ===========================================================
 *                   load_landWaterPrefabs
 * -----------------------------------------------------------
 * -- 每次游戏启动，从 png文件 读取数据，解析并存入 mem  [此函数不要求性能]
 */
void load_landWaterPrefabs(){

    //-------------------------//
    //  手动设置 png 数据... tmp...
    //-------------------------//
    lpath_corner = "/landWater_corner_a.png";
    lpath_edge   = "/landWater_edge_2.png";

    frameNum_corner.set( 4, 2 );
    frameNum_edge.set( 4, 3 );

    totalFrameNum_corner = frameNum_corner.x * frameNum_corner.y;
    totalFrameNum_edge = frameNum_edge.x * frameNum_edge.y;

    //----------------------------//
    //     读取 png 原始数据
    //----------------------------//
    frame_data_ary_corner.clear();
    frame_data_ary_edge.clear();

    
    pixNum_per_frame_corner = load_and_divide_png( tpr::path_combine( path_landWater, lpath_corner ),
                                                frameNum_corner,
                                                totalFrameNum_corner,
                                                frame_data_ary_corner );
    
    assert( (pixNum_per_frame_corner.x==ENTS_PER_LANDWATERPREFAB) &&
            (pixNum_per_frame_corner.y==ENTS_PER_LANDWATERPREFAB) );

    
    pixNum_per_frame_edge = load_and_divide_png( tpr::path_combine( path_landWater, lpath_edge ),
                                                frameNum_edge,
                                                totalFrameNum_edge,
                                                frame_data_ary_edge );

    assert( (pixNum_per_frame_edge.x==ENTS_PER_LANDWATERPREFAB) &&
            (pixNum_per_frame_edge.y==ENTS_PER_LANDWATERPREFAB) );
    
                            
    //----------------------------//
    //        解析 corners
    //----------------------------//
    size_t                entIdx;
    LandWaterPrefabEnt      tmpPrefabEnt {};
    //-----
    for( const auto &frameRef : frame_data_ary_corner ){ //- each frame in png

        //--- 将 原版数据 传入 预制件实例 ---//
        clear_for_LandWaterPrefabCorner();
        for( int h=0; h<ENTS_PER_LANDWATERPREFAB; h++ ){
            for( int w=0; w<ENTS_PER_LANDWATERPREFAB; w++ ){ //- each mapent in png-frame (2*2chunk)
                entIdx = h*ENTS_PER_LANDWATERPREFAB + w;

                if( frameRef.at(entIdx).is_near( fstColor, 5 ) ){ //- 主色点
                    tmpPrefabEnt.lMPosOff.set(w-ENTS_PER_CHUNK, h-ENTS_PER_CHUNK );
                    tmpPrefabEnt.is_major = true;
                    push_back_originData_for_LandWaterPrefabCorner( tmpPrefabEnt ); //- copy

                }else if( frameRef.at(entIdx).is_near( secColor, 5 ) ){ //- 副色点
                    tmpPrefabEnt.lMPosOff.set(w-ENTS_PER_CHUNK, h-ENTS_PER_CHUNK );
                    tmpPrefabEnt.is_major = false;
                    push_back_originData_for_LandWaterPrefabCorner( tmpPrefabEnt ); //- copy
                }
            }
        } //-- each mapent in png-frame (2*2chunk) end --
        //--- 预制件实例 自己 生成所有 变种 数据 ---//
        build_all_mutant_datas_for_LandWaterPrefabCorner();
    } //-- each frame in png end --
    

    //----------------------------//
    //        解析 edges
    //----------------------------//
    for( const auto &frameRef : frame_data_ary_edge ){ //- each frame in png

        //--- 将 原版数据 传入 预制件实例 ---//
        clear_for_LandWaterPrefabEdge();
        for( int h=0; h<ENTS_PER_LANDWATERPREFAB; h++ ){
            for( int w=0; w<ENTS_PER_LANDWATERPREFAB; w++ ){ //- each mapent in png-frame (2*2chunk)
                entIdx = h*ENTS_PER_LANDWATERPREFAB + w;

                if( frameRef.at(entIdx).is_near( fstColor, 5) ){ //- 主色点
                    tmpPrefabEnt.lMPosOff.set(w-ENTS_PER_CHUNK, h-ENTS_PER_CHUNK );
                    tmpPrefabEnt.is_major = true;
                    push_back_originData_for_LandWaterPrefabEdge( tmpPrefabEnt ); //- copy
                }else if( frameRef.at(entIdx).is_near( secColor, 5) ){ //- 副色点
                    tmpPrefabEnt.lMPosOff.set(w-ENTS_PER_CHUNK, h-ENTS_PER_CHUNK );
                    tmpPrefabEnt.is_major = false;
                    push_back_originData_for_LandWaterPrefabEdge( tmpPrefabEnt ); //- copy
                }
            }
        } //-- each mapent in png-frame (2*2chunk) end --
        //--- 预制件实例 自己 生成所有 变种 数据 ---//
        build_all_mutant_datas_for_LandWaterPrefabEdge();
    }
}


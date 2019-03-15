/*
 * ==================== landWaterMask_Handle.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  
 * ----------------------------
 */
#include "landWaterMask_Handle.h"


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
#include "landWaterMaskId_t.h"
#include "RGBA.h"
#include "global.h"
#include "config.h"
#include "LandWaterMaskCorner.h"
#include "LandWaterMaskEdge.h"


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


    //   LandWaterMask 资源
    //  预制件，每次游戏启动时临时加载
    //std::unordered_map<landWaterMaskId_t, LandWaterMaskCorner> landWaterMaskCorners {};
    //std::unordered_map<landWaterMaskId_t, LandWaterMaskEdge> landWaterMaskEdges {};


    RGBA fstColor { 255, 0, 0, 255 }; //- png数据 中的 主色
    RGBA secColor { 0, 0, 255, 255 }; //- png数据 中的 副色


    //LandWaterMaskCorner *insert_new_LandWaterMaskCorner();
    //LandWaterMaskEdge *insert_new_LandWaterMaskEdge();

}//------------- namespace: end --------------//




/* ===========================================================
 *                   load_landWaterMasks
 * -----------------------------------------------------------
 * -- 每次游戏启动，从 png文件 读取数据，解析并存入 mem  [此函数不要求性能]
 */
void load_landWaterMasks(){

    //-------------------------//
    //  手动设置 png 数据... tmp...
    //-------------------------//
    lpath_corner = "/landWaterMaskSet_corner.png";
    lpath_edge   = "/landWaterMaskSet_edge.png";

    frameNum_corner.set( 5, 3 );
    frameNum_edge.set( 5, 6 );

    totalFrameNum_corner = 5*3;
    totalFrameNum_edge = 5*6;

    //----------------------------//
    //     读取 png 原始数据
    //----------------------------//
    frame_data_ary_corner.clear();
    frame_data_ary_edge.clear();

    
    pixNum_per_frame_corner = load_and_divide_png( tpr::path_combine( path_landWaterMaskSet, lpath_corner ),
                                                frameNum_corner,
                                                totalFrameNum_corner,
                                                frame_data_ary_corner );
    
    assert( (pixNum_per_frame_corner.x==ENTS_PER_LANDWATERMASK) &&
            (pixNum_per_frame_corner.y==ENTS_PER_LANDWATERMASK) );

    
    pixNum_per_frame_edge = load_and_divide_png( tpr::path_combine( path_landWaterMaskSet, lpath_edge ),
                                                frameNum_edge,
                                                totalFrameNum_edge,
                                                frame_data_ary_edge );

    assert( (pixNum_per_frame_edge.x==ENTS_PER_LANDWATERMASK) &&
            (pixNum_per_frame_edge.y==ENTS_PER_LANDWATERMASK) );
    
                            
    //----------------------------//
    //        解析 corners
    //----------------------------//
    size_t                entIdx;
    LandWaterMaskEnt      tmpMaskEnt {};
    //-----
    for( const auto &frameRef : frame_data_ary_corner ){ //- each frame in png

        //--- 将 原版数据 传入 预制件实例 ---//
        clear_for_LandWaterMaskCorner();
        for( int h=0; h<ENTS_PER_LANDWATERMASK; h++ ){
            for( int w=0; w<ENTS_PER_LANDWATERMASK; w++ ){ //- each mapent in png-frame (2*2chunk)
                entIdx = h*ENTS_PER_LANDWATERMASK + w;

                if( is_rgba_near(frameRef.at(entIdx), fstColor, 5) ){ //- 主色点
                    tmpMaskEnt.lMPosOff.set(w-ENTS_PER_CHUNK, h-ENTS_PER_CHUNK );
                    tmpMaskEnt.is_major = true;
                    push_back_originData_for_LandWaterMaskCorner( tmpMaskEnt ); //- copy
                }else if( is_rgba_near(frameRef.at(entIdx), secColor, 5) ){ //- 副色点
                    tmpMaskEnt.lMPosOff.set(w-ENTS_PER_CHUNK, h-ENTS_PER_CHUNK );
                    tmpMaskEnt.is_major = false;
                    push_back_originData_for_LandWaterMaskCorner( tmpMaskEnt ); //- copy
                }
            }
        } //-- each mapent in png-frame (2*2chunk) end --
        //--- 预制件实例 自己 生成所有 变种 数据 ---//
        build_all_mutant_datas_for_LandWaterMaskCorner();
    }
    

    //----------------------------//
    //        解析 edges
    //----------------------------//
    for( const auto &frameRef : frame_data_ary_corner ){ //- each frame in png

        //--- 将 原版数据 传入 预制件实例 ---//
        clear_for_LandWaterMaskEdge();
        for( int h=0; h<ENTS_PER_LANDWATERMASK; h++ ){
            for( int w=0; w<ENTS_PER_LANDWATERMASK; w++ ){ //- each mapent in png-frame (2*2chunk)
                entIdx = h*ENTS_PER_LANDWATERMASK + w;

                if( is_rgba_near(frameRef.at(entIdx), fstColor, 5) ){ //- 主色点
                    tmpMaskEnt.lMPosOff.set(w-ENTS_PER_CHUNK, h-ENTS_PER_CHUNK );
                    tmpMaskEnt.is_major = true;
                    push_back_originData_for_LandWaterMaskEdge( tmpMaskEnt ); //- copy
                }else if( is_rgba_near(frameRef.at(entIdx), secColor, 5) ){ //- 副色点
                    tmpMaskEnt.lMPosOff.set(w-ENTS_PER_CHUNK, h-ENTS_PER_CHUNK );
                    tmpMaskEnt.is_major = false;
                    push_back_originData_for_LandWaterMaskEdge( tmpMaskEnt ); //- copy
                }
            }
        } //-- each mapent in png-frame (2*2chunk) end --
        //--- 预制件实例 自己 生成所有 变种 数据 ---//
        build_all_mutant_datas_for_LandWaterMaskEdge();
    }
}


namespace{//-------- namespace: --------------//

/* ===========================================================
 *                insert_new_LandWaterMaskCorner
 * -----------------------------------------------------------
 */
/*
LandWaterMaskCorner *insert_new_LandWaterMaskCorner(){
    // ***| INSERT FIRST, INIT LATER  |***
    LandWaterMaskCorner  lwmc {};
    landWaterMaskId_t id_ = LandWaterMaskCorner::id_manager.apply_a_u32_id();
    lwmc.id = id_;
        assert( landWaterMaskCorners.find(id_) == landWaterMaskCorners.end() );//- must
    landWaterMaskCorners.insert({id_, lwmc}); //- copy
    return  (LandWaterMaskCorner*)&(landWaterMaskCorners.at(id_));
}
*/


/* ===========================================================
 *                insert_new_LandWaterMaskEdge
 * -----------------------------------------------------------
 */
/*
LandWaterMaskEdge *insert_new_LandWaterMaskEdge(){
    // ***| INSERT FIRST, INIT LATER  |***
    LandWaterMaskEdge  lwmc {};
    landWaterMaskId_t id_ = LandWaterMaskEdge::id_manager.apply_a_u32_id();
    lwmc.id = id_;
        assert( landWaterMaskEdges.find(id_) == landWaterMaskEdges.end() );//- must
    landWaterMaskEdges.insert({id_, lwmc}); //- copy
    return  (LandWaterMaskEdge*)&(landWaterMaskEdges.at(id_));
}
*/




}//------------- namespace: end --------------//


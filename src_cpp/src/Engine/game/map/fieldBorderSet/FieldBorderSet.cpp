/*
 * ==================== FieldBorderSet.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "FieldBorderSet.h"

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
//#include "random.h"


namespace fbs_inn {//-------- namespace: fbs_inn --------------//

    size_t frameSize = PIXES_PER_FIELD_BORDER_SET * PIXES_PER_FIELD_BORDER_SET;
    size_t quadSize = PIXES_PER_FIELD * PIXES_PER_FIELD;


    //- 原版数据 做xy轴翻转
    FieldBorderSet::quadContainer_t originData {};
    FieldBorderSet::quadContainer_t originData_Xflip {};
    FieldBorderSet::quadContainer_t originData_Yflip {};
    FieldBorderSet::quadContainer_t originData_XYflip {};
    //-- 不进行 90度 旋转，保持 纹理走向


    //-- 预制件 资源 ---
    std::unordered_map<fieldBorderSetId_t, FieldBorderSet> fieldBorderSets {};

    std::vector<fieldBorderSetId_t> fieldBorderSet_ids {};


    //====== funcs =======//
    void handle_each_container( const FieldBorderSet::quadContainer_t &container_ );


}//------------- namespace: fbs_inn end --------------//


/* ===========================================================
 *             clear_for_fieldBorderSet
 * -----------------------------------------------------------
 */
void clear_for_fieldBorderSet(){
    fbs_inn::originData.clear();
    fbs_inn::originData_Xflip.clear();
    fbs_inn::originData_Yflip.clear();
    fbs_inn::originData_XYflip.clear();
}


/* ===========================================================
 *       copy_originData_for_fieldBorderSet
 * -----------------------------------------------------------
 * -- 将原版数据，暂存在 文件容器中
 */
void copy_originData_for_fieldBorderSet( const FieldBorderSet::quadContainer_t &datas_  ){
    fbs_inn::originData.insert( fbs_inn::originData.end(), datas_.begin(), datas_.end() ); //- copy

}


/* ===========================================================
 *       build_all_mutant_datas_for_fieldBorderSet
 * -----------------------------------------------------------
 */
void build_all_mutant_datas_for_fieldBorderSet(){

    IntVec2   XYflipPixWH   {};  //- 原始数据 xy翻转后的 相对坐标 
    size_t    pixIdx_origin {};
    size_t    pixIdx_flip   {};

    fbs_inn::originData_Xflip.resize( fbs_inn::frameSize );
    fbs_inn::originData_Yflip.resize( fbs_inn::frameSize );
    fbs_inn::originData_XYflip.resize( fbs_inn::frameSize );

    for( int h=0; h<PIXES_PER_FIELD_BORDER_SET; h++ ){
        for( int w=0; w<PIXES_PER_FIELD_BORDER_SET; w++ ){//- each pix in frame
            pixIdx_origin = cast_2_size_t( h * PIXES_PER_FIELD_BORDER_SET + w );

            XYflipPixWH.x = PIXES_PER_FIELD_BORDER_SET - 1 - w;
            XYflipPixWH.y = PIXES_PER_FIELD_BORDER_SET - 1 - h;

            //-- X --
            pixIdx_flip = cast_2_size_t( h * PIXES_PER_FIELD_BORDER_SET + XYflipPixWH.x );
            fbs_inn::originData_Xflip.at(pixIdx_flip) = fbs_inn::originData.at(pixIdx_origin);

            //-- Y --
            pixIdx_flip = cast_2_size_t( XYflipPixWH.y * PIXES_PER_FIELD_BORDER_SET + w );
            fbs_inn::originData_Yflip.at(pixIdx_flip) = fbs_inn::originData.at(pixIdx_origin);

            //-- XY --
            pixIdx_flip = cast_2_size_t( XYflipPixWH.y * PIXES_PER_FIELD_BORDER_SET + XYflipPixWH.x );
            fbs_inn::originData_XYflip.at(pixIdx_flip) = fbs_inn::originData.at(pixIdx_origin);
        }
    }

    //-- 逐个处理8个容器，将每个容器的数据，拆分为 4份 --
    fbs_inn::handle_each_container( fbs_inn::originData );
    fbs_inn::handle_each_container( fbs_inn::originData_Xflip );
    fbs_inn::handle_each_container( fbs_inn::originData_Yflip );
    fbs_inn::handle_each_container( fbs_inn::originData_XYflip );
}


/* ===========================================================
 *              apply_a_fieldBorderSetId
 * -----------------------------------------------------------
 */
fieldBorderSetId_t apply_a_fieldBorderSetId( size_t randIdx_ ){

    size_t idx = randIdx_ % fbs_inn::fieldBorderSet_ids.size();
    return fbs_inn::fieldBorderSet_ids.at(idx);
}


/* ===========================================================
 *                get_fieldBorderSet
 * -----------------------------------------------------------
 */
const FieldBorderSet::quadContainer_t &get_fieldBorderSet( fieldBorderSetId_t id_, QuadType quad_ ){

    tprAssert( fbs_inn::fieldBorderSets.find(id_) != fbs_inn::fieldBorderSets.end() );
    switch(quad_){
        case QuadType::Left_Bottom:   return fbs_inn::fieldBorderSets.at(id_).leftBottoms;
        case QuadType::Right_Bottom:  return fbs_inn::fieldBorderSets.at(id_).rightBottoms;
        case QuadType::Left_Top:      return fbs_inn::fieldBorderSets.at(id_).leftTops;
        case QuadType::Right_Top:     return fbs_inn::fieldBorderSets.at(id_).rightTops;
        default:
            tprAssert(0);
            return fbs_inn::fieldBorderSets.at(id_).leftBottoms; //- never touch
    }   
}



namespace fbs_inn {//-------- namespace: fbs_inn --------------//


/* ===========================================================
 *             handle_each_container
 * -----------------------------------------------------------
 * -- 将目标容器中的数据 正式存储到 全局容器中。
 */
void handle_each_container( const FieldBorderSet::quadContainer_t &container_ ){

    // ***| INSERT FIRST, INIT LATER  |***
    FieldBorderSet  fbs {};
    fieldBorderSetId_t id_ = FieldBorderSet::id_manager.apply_a_u32_id();
    fbs.id = id_;
        tprAssert( fieldBorderSets.find(id_) == fieldBorderSets.end() );//- must
    fieldBorderSet_ids.push_back( id_ );
    fieldBorderSets.insert({id_, fbs}); //- copy
    FieldBorderSet &fbsRef = fieldBorderSets.at(id_);

    fbsRef.leftBottoms.reserve( quadSize );
    fbsRef.rightBottoms.reserve( quadSize );
    fbsRef.leftTops.reserve( quadSize );
    fbsRef.rightTops.reserve( quadSize );

    tprAssert( container_.size() == frameSize );


    IntVec2   pixWH        {};
    size_t    containerIdx {}; //- pix 在 原始数据中的 idx
    size_t    quadIdx      {}; //- pix 在具体 象限中的 idx
    
    for( size_t h=0; h<PIXES_PER_FIELD; h++ ){
        for( size_t w=0; w<PIXES_PER_FIELD; w++ ){ //- each pix in 1/4 container
            quadIdx = h * PIXES_PER_FIELD + w;
            
            //----- leftBottom ------//
            pixWH = IntVec2{w,h} + IntVec2{ 0, 0 };
            containerIdx = cast_2_size_t( pixWH.y * PIXES_PER_FIELD_BORDER_SET + pixWH.x );
            fbsRef.leftBottoms.push_back( container_.at(containerIdx) );

            //----- rightBottom ------//
            pixWH = IntVec2{w,h} + IntVec2{ PIXES_PER_FIELD, 0 };
            containerIdx = cast_2_size_t( pixWH.y * PIXES_PER_FIELD_BORDER_SET + pixWH.x );
            fbsRef.rightBottoms.push_back( container_.at(containerIdx) );

            //----- leftTop ------//
            pixWH = IntVec2{w,h} + IntVec2{ 0, PIXES_PER_FIELD };
            containerIdx = cast_2_size_t( pixWH.y * PIXES_PER_FIELD_BORDER_SET + pixWH.x );
            fbsRef.leftTops.push_back( container_.at(containerIdx) );

            //----- rightTop ------//
            pixWH = IntVec2{w,h} + IntVec2{ PIXES_PER_FIELD, PIXES_PER_FIELD };
            containerIdx = cast_2_size_t( pixWH.y * PIXES_PER_FIELD_BORDER_SET + pixWH.x );
            fbsRef.rightTops.push_back( container_.at(containerIdx) );
        }
    }

    tprAssert( fbsRef.leftBottoms.size() == quadSize );
    tprAssert( fbsRef.rightBottoms.size() == quadSize );
    tprAssert( fbsRef.leftTops.size() == quadSize );
    tprAssert( fbsRef.rightTops.size() == quadSize );
}


}//------------- namespace: fbs_inn end --------------//



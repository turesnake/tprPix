/*
 * =================== build_chunkData.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  job: build chunkData
 * ----------------------------
 */
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- C --------------------//
#include <cstring>
#include <cmath>

//-------------------- CPP --------------------//
#include <map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "jobs_all.h"
#include "Job.h"
#include "simplexNoise.h"
#include "config.h"
#include "MapTexture.h" 
#include "Quad.h"
#include "FieldBorderSet.h"   //- 即将被取代
#include "tprCast.h"

#include "esrc_chunkData.h"
#include "esrc_gameSeed.h"
#include "esrc_field.h"
#include "esrc_ecoObj.h"

/*
#include <iostream>
#include <string>
using std::cout;
using std::endl;
*/

namespace bcd_inn {//----------- namespace: bcd_inn ----------------//

    //-- 不要随意建立 static数据，本文件的代码，会被数个 job线程 调用 --

    //----------- 用于 calc_pixAltis ------------//
    //- 在未来，freq 这组值也会收到 ecosys 影响 --
    const double freqSeaLvl { 0.05 };
    const double freqBig    { 0.4 };
    const double freqMid    { 1.6 };
    const double freqSml    { 4.0 };


    const glm::dvec2  worldCenter { 0.0, 0.0 };

    //----------- 用于 calc_chunkData ------------//

    class FieldData{
    public:
        explicit FieldData( const MapFieldData_In_ChunkCreate &data_,
                            QuadType       quadType_ ){
            this->fieldKey = data_.fieldKey;
            this->quadContainerPtr = const_cast<FieldBorderSet::quadContainer_t*>( 
                                                    &get_fieldBorderSet( data_.fieldBorderSetId, quadType_) );
            this->densityIdx = data_.densityIdx;
            this->nodeMPos = data_.nodeMPos;
        }

        inline bool is_equal_2_nodeMPos( const IntVec2 &mpos_ ) const {
            return (this->nodeMPos == mpos_);
        }

        //====== vals ======//
        fieldKey_t               fieldKey {};
        FieldBorderSet::quadContainer_t  *quadContainerPtr {}; //-- fieldBorderSet 子扇区容器 --
    private:
        //====== vals ======//
        size_t                   densityIdx {};
        IntVec2                  nodeMPos {}; //- 从 field 复制来的，只读
    };


    class PixData{
    public:
        inline void init( const IntVec2 &ppos_ ){
            this->ppos = ppos_;
        }
        //====== vals ======//
        size_t     pixIdx_in_chunk    {};
        size_t     pixIdx_in_field    {};
        IntVec2    ppos      {};
        FieldData *fieldDataPtr {nullptr}; 
    };                          //   可能不需要这个 class 了，新版视觉中，只需要计算一个 pix 的数据
    


    class FieldInfo{
    public:
        IntVec2   mposOff {};
        QuadType  quad {}; //- 注意，这个值是反的，从 fieldBorderSet 角度去看 得到的 quad
    };
    //- 周边4个field 指示数据 --
    const std::vector<FieldInfo> nearFour_fieldInfos {
        FieldInfo{ IntVec2{ 0, 0 },                           QuadType::Right_Top },
        FieldInfo{ IntVec2{ ENTS_PER_FIELD, 0 },              QuadType::Left_Top  },
        FieldInfo{ IntVec2{ 0, ENTS_PER_FIELD },              QuadType::Right_Bottom  },
        FieldInfo{ IntVec2{ ENTS_PER_FIELD, ENTS_PER_FIELD }, QuadType::Left_Bottom  }
    };

    //===== funcs =====//
    double calc_pixAlti( const IntVec2 &pixPPos_ );

    void calc_chunkData(    const IntVec2 &chunkMPos_, 
                            ChunkData &chunkDataRef_ ); 
                //- 在未来，要改名

    const IntVec2 colloect_nearFour_fieldDatas( std::map<occupyWeight_t,FieldData> &container_,
                                            fieldKey_t fieldKey_ );

}//-------------- namespace: bcd_inn end ----------------//



/* ===========================================================
 *                build_chunkData_main
 * -----------------------------------------------------------
 * 在未来，这个函数需要写进 atom 函数中。
 */
void build_chunkData_main( const Job &job_ ){

    //-------------------//
    //   job.argBinary
    //-------------------//
    tprAssert( job_.argBinary.size() == sizeof(ArgBinary_Build_ChunkData) );
    ArgBinary_Build_ChunkData arg {};
    memcpy( (void*)&arg,
            (void*)&(job_.argBinary.at(0)),
            sizeof(ArgBinary_Build_ChunkData) );

    //-- 制作一个 ChunkData 数据实例 --
    ChunkData &chunkDataRef = esrc::atom_insert_new_chunkData( arg.chunkKey );

    IntVec2 chunkMPos = chunkKey_2_mpos( arg.chunkKey );

    //------------------------------//
    //           [1]
    // 收集 周边 4个 sectionKey
    // 创建它们的 第一阶段数据 ( section / ecoObj )
    //------------------------------//
    // 已经在 主线程 chunkBuild_1_push_job() 中 提前完成
    // 反正再糟糕也不过是，1帧内创建 5 个 ecoObj 实例
    // 这个开销可以接受

    //------------------------------//
    //            [2]
    //  集中生成 周边 4chunk 的 所有 fields
    //------------------------------//
    IntVec2  tmpFieldMPos {};
    for( int h=0; h<FIELDS_PER_CHUNK*2; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK*2; w++ ){ //- each field in 2*2chunks
            tmpFieldMPos.set(   chunkMPos.x + w*ENTS_PER_FIELD,
                                chunkMPos.y + h*ENTS_PER_FIELD );
            esrc::atom_try_to_insert_and_init_the_field_ptr( tmpFieldMPos );

                    // 立即暂存所有 field 的 ecoObjKey 


        }
    } //- each field in 2*2chunks

    //--------------------------//
    //      chunkData
    //--------------------------//
    bcd_inn::calc_chunkData( chunkMPos, chunkDataRef );

    //--------------------------//
    //-- chunkData 数据计算完成后，向 状态表 添加一个元素
    //   以此来提醒 主线程，这个 chunk 数据准备好了
    //--------------------------//
    esrc::atom_push_back_2_chunkDataFlags( arg.chunkKey );
}


namespace bcd_inn {//----------- namespace: bcd_inn ----------------//


/* ===========================================================
 *                 calc_pixAlti
 * -----------------------------------------------------------
 * 计算单个pix 的 alti
 * ---
 * 这部分算法，应当和 waterAnimCanvas 中的完全一致
 */
double calc_pixAlti( const IntVec2 &pixPPos_ ){

    GameSeed &gameSeedRef = esrc::get_gameSeed();
    glm::dvec2  altiSeed_pposOffBig = gameSeedRef.get_altiSeed_pposOffBig();
    glm::dvec2  altiSeed_pposOffMid = gameSeedRef.get_altiSeed_pposOffMid();
    glm::dvec2  altiSeed_pposOffSml = gameSeedRef.get_altiSeed_pposOffSml();
                            //-- 此处有问题，从 job线程 访问 gameSeed，不够安全...

    double    pixDistance {}; //- pix 距离 世界中心的距离。 暂时假设，(0,0) 为世界中心
    double    seaLvl {};

    double    pnValBig {};
    double    pnValMid {};
    double    pnValSml {};
    double    altiVal  {};  //- target val

    glm::dvec2 pixCFPos {};

            pixCFPos.x = static_cast<double>(pixPPos_.x) / static_cast<double>(PIXES_PER_CHUNK);
            pixCFPos.y = static_cast<double>(pixPPos_.y) / static_cast<double>(PIXES_PER_CHUNK);
            //------------------//
            //     seaLvl
            //------------------//
            pixDistance = glm::distance( pixCFPos, worldCenter );
            pixDistance /= 10.0;
            //--------
            seaLvl = simplex_noise2( pixCFPos * freqSeaLvl ) * 50.0; // [-50.0, 50.0]
            seaLvl += pixDistance;
            if( seaLvl < 0.0 ){ //- land
                seaLvl *= 0.3;  // [-15.0, 50.0]
            }
            //------------------//
            //    alti.val
            //------------------//
            //--- 使用速度最快的 2D-simplex-noise ---
            pnValBig = simplex_noise2( (pixCFPos + altiSeed_pposOffBig) * freqBig ) * 100.0 - seaLvl; // [-100.0, 100.0]
            pnValMid = simplex_noise2( (pixCFPos + altiSeed_pposOffMid) * freqMid ) * 50.0  - seaLvl; // [-50.0, 50.0]
            pnValSml = simplex_noise2( (pixCFPos + altiSeed_pposOffSml) * freqSml ) * 20.0  - seaLvl; // [-20.0, 20.0]
            //---------
            altiVal = floor(pnValBig + pnValMid + pnValSml);

            //------- 抹平头尾 -------//
            if( altiVal > 100.0 ){
                altiVal = 100.0;
            }else if( altiVal < -100.0 ){
                altiVal = -100.0;
            }
            // now, altiVal: [-100.0, 100.0]=

    return altiVal;
}


/* ===========================================================
 *                 calc_chunkData
 * -----------------------------------------------------------
 * -- 生成 chunkData 中数据
 */
void calc_chunkData(const IntVec2 &chunkMPos_, 
                    ChunkData &chunkDataRef_ ){

    IntVec2    tmpFieldMPos {};
    IntVec2    tmpFieldPPos {};

    IntVec2    tmpEntMPos {};
    IntVec2    pposOff {};   //- 通用偏移向量
    IntVec2    mposOff {};

    PixData   pixData {};//- each pix

    size_t    entIdx_in_chunk {};
    size_t    count {};

    glm::dvec2  pixCFPos {};

    MapAltitude  pixAlti {};

    //------------------------//
    //   mapEntAltis, mapEntMPoses    
    //------------------------//
    chunkDataRef_.init_mapEntAltis();

    //------------------------//
    //      fieldKeys
    //------------------------//
    std::vector<fieldKey_t> fieldKeys {}; //- 8*8 fieldKeys，only used inner
    fieldKeys.reserve( FIELDS_PER_CHUNK * FIELDS_PER_CHUNK ); // reserve FIRST !!!
    IntVec2    tmpFieldMpos {};
    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field in 8*8
            tmpFieldMpos.set(   chunkMPos_.x + w*ENTS_PER_FIELD, 
                                chunkMPos_.y + h*ENTS_PER_FIELD );
            fieldKeys.push_back( fieldMPos_2_fieldKey(tmpFieldMpos) );
        }
    }
    //------------------------//
    //       
    //------------------------//

    std::map<occupyWeight_t,FieldData> nearby_4_fieldDatas {}; //- 一个 field 周边4个 field 数据
                                    // 按照 ecoObj.occupyWeight 倒叙排列（值大的在前面）

    for( const auto &fieldKey : fieldKeys ){ //- each field key

        //-- 收集 目标field 周边4个 field 实例指针  --
        tmpFieldMPos = colloect_nearFour_fieldDatas( nearby_4_fieldDatas, fieldKey );
        tmpFieldPPos = mpos_2_ppos( tmpFieldMPos );

        for( int eh=0; eh<ENTS_PER_FIELD; eh++ ){
            for( int ew=0; ew<ENTS_PER_FIELD; ew++ ){ //- each ent in field

                tmpEntMPos = tmpFieldMPos + IntVec2{ ew, eh };
                mposOff = tmpEntMPos - chunkMPos_;
                entIdx_in_chunk = cast_2_size_t( mposOff.y * ENTS_PER_CHUNK + mposOff.x );


                //--------------------------------//
                // mapent 中点 pix
                //--------------------------------//
                pixData.init( mpos_2_ppos(tmpEntMPos) + IntVec2{ HALF_PIXES_PER_MAPENT, HALF_PIXES_PER_MAPENT } );
                pposOff = pixData.ppos - mpos_2_ppos(chunkMPos_);
                pixData.pixIdx_in_chunk = cast_2_size_t( pposOff.y * PIXES_PER_CHUNK + pposOff.x );

                pposOff = pixData.ppos - tmpFieldPPos;
                pixData.pixIdx_in_field = cast_2_size_t( pposOff.y * PIXES_PER_FIELD + pposOff.x );

                //--------------------------------//
                // 确定 pix 属于 周边4个field 中的哪一个
                //--------------------------------//
                count = 0;
                for( auto &fieldPair : nearby_4_fieldDatas ){ //--- 周边4个 field 信息
                    count++;
                    const FieldData &fieldDataRef = fieldPair.second;
                    if( count != nearby_4_fieldDatas.size() ){   //- 前3个 field
                        if( fieldDataRef.quadContainerPtr->at(pixData.pixIdx_in_field) == 1 ){
                            pixData.fieldDataPtr = const_cast<FieldData*>( &fieldDataRef );
                            break;
                        }
                    }else{     //- 第4个 field
                        pixData.fieldDataPtr = const_cast<FieldData*>( &fieldDataRef );
                    }
                } //--- 周边4个 field 信息 end ---

                //--------------------------------//
                //  计算 本pix  alti
                //--------------------------------//
                pixAlti.set( calc_pixAlti( pixData.ppos ) );

                //--------------------------------//
                // 每个 mapent.mapAlti 被设置为其 中点pix 的 alti
                //--------------------------------//
                chunkDataRef_.set_mapEntAlti( entIdx_in_chunk, pixAlti );
                esrc::atom_field_reflesh_min_and_max_altis( fieldKey, pixAlti );

            }
        }//- each ent in field end -- 
    }//-- each field key end --

}


/* ===========================================================
 *              colloect_nearFour_fieldDatas
 * -----------------------------------------------------------
 * -- 收集 目标field 周边4个 field 数据
 * -- 返回 目标 field mpos
 */
const IntVec2 colloect_nearFour_fieldDatas( std::map<occupyWeight_t,FieldData> &container_,
                                        fieldKey_t fieldKey_ ){

    IntVec2     targetFieldMPos = fieldKey_2_mpos( fieldKey_ );
    fieldKey_t  tmpFieldKey {};
    //---
    container_.clear();
    for( const auto &fieldInfo : nearFour_fieldInfos ){

        tmpFieldKey = fieldMPos_2_fieldKey( targetFieldMPos + fieldInfo.mposOff );
        //-- 这个数据 仅临时存在一下
        auto tmpUPtr = esrc::atom_get_mapFieldData_in_chunkCreate( tmpFieldKey );
        container_.insert({ -(tmpUPtr->occupyWeight), 
                            FieldData{  *(tmpUPtr.get()), 
                                        fieldInfo.quad } }); //- copy
    }
    return targetFieldMPos;
}

}//-------------- namespace: bcd_inn end ----------------//


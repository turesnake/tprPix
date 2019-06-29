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
#include "FieldBorderSet.h"
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
    const float freqSeaLvl { 0.05f };
    const float freqBig    { 0.4f };
    const float freqMid    { 1.6f };
    const float freqSml    { 4.0f };

    // 暂未被使用...
    //const float zOffBig  { 0.2f };
    //const float zOffMid  { 7.5f };
    //const float zOffSml  { 17.8f };

    const glm::vec2  worldCenter { 0.0f, 0.0f };

    //----------- 用于 calc_chunkData ------------//

    class FieldData{
    public:
        explicit FieldData( const MapFieldData_In_ChunkBuild &data_,
                            QuadType       quadType_ ){
            this->fieldKey = data_.fieldKey;
            this->landColorsPtr = esrc::atom_get_ecoObj_landColorsPtr( data_.ecoObjKey );
            this->quadContainerPtr = const_cast<FieldBorderSet::quadContainer_t*>( 
                                                    &get_fieldBorderSet( data_.fieldBorderSetId, quadType_) );
            this->densityIdx = data_.densityIdx;
            this->nodeMPos = data_.nodeMPos;
        }
        inline const RGBA &clac_pixColor() const {
            return this->landColorsPtr->at( this->densityIdx );
        }

        inline bool is_equal_2_nodeMPos( const IntVec2 &mpos_ ) const {
            return (this->nodeMPos == mpos_);
        }

        //====== vals ======//
        fieldKey_t               fieldKey {};
        const std::vector<RGBA> *landColorsPtr {};
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
        size_t     pixIdx_in_chunkTex {}; //- chunk-texture 会多出来几排pix
        size_t     pixIdx_in_field    {};
        IntVec2    ppos      {};
        RGBA      *texPixPtr {nullptr};
        FieldData *fieldDataPtr {nullptr}; 
        MapAltitude   alti {};
    };


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
    void calc_pixAltis(     const IntVec2 &chunkMPos_, 
                            std::vector<float> &pixAltis_ );

    void calc_chunkData(    const IntVec2 &chunkMPos_, 
                            const std::vector<float> &pixAltis_,
                            ChunkData *chunkDataPtr_ ); 
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
    ChunkData *chunkDataPtr = esrc::atom_insert_new_chunkData( arg.chunkKey );
    IntVec2 chunkMPos = chunkKey_2_mpos( arg.chunkKey );

    //------------------------------//
    //           [1]
    // 收集 周边 4个 sectionKey
    // 创建它们的 第一阶段数据 ( section / ecoObj )
    //------------------------------//
    // 已经在 主线程 chunkBuild_1_push_job() 中 提前完成
    // 反正再糟糕也不过是，1帧内创建 5 个 ecoObj 实例

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
        }
    } //- each field in 2*2chunks

    //--------------------------//
    //       pixAltis
    //--------------------------//
    std::vector<float> pixAltis {}; //- 仅内部使用
    bcd_inn::calc_pixAltis( chunkMPos, pixAltis );

    //--------------------------//
    //      chunkData
    //--------------------------//
    bcd_inn::calc_chunkData( chunkMPos, pixAltis, chunkDataPtr );

    //--------------------------//
    //-- chunkData 数据计算完成后，向 状态表 添加一个元素
    //   以此来提醒 主线程，这个 chunk 数据准备好了
    //--------------------------//
    esrc::atom_push_back_2_chunkDataFlags( arg.chunkKey );
    
        /*
        cout << "    build_chunkData_main(): "
            << "chunkMPos: " << chunkMPos.x
            << ", " << chunkMPos.y
            << "; DONE;"
            << endl;
        */
}


namespace bcd_inn {//----------- namespace: bcd_inn ----------------//


/* ===========================================================
 *                 calc_pixAltis
 * -----------------------------------------------------------
 */
void calc_pixAltis( const IntVec2 &chunkMPos_, 
                    std::vector<float> &pixAltis_ ){

    IntVec2      chunkPPos = mpos_2_ppos(chunkMPos_);
    glm::vec2    pixCFPos {}; //- 以 chunk 为晶格的 fpos

    GameSeed &gameSeedRef = esrc::get_gameSeed();
    glm::vec2  altiSeed_pposOffBig = gameSeedRef.get_altiSeed_pposOffBig();
    glm::vec2  altiSeed_pposOffMid = gameSeedRef.get_altiSeed_pposOffMid();
    glm::vec2  altiSeed_pposOffSml = gameSeedRef.get_altiSeed_pposOffSml();
                            //-- 此处有问题，从 job线程 访问 gameSeed，不够安全...

    float      pixDistance {}; //- pix 距离 世界中心的距离。 暂时假设，(0,0) 为世界中心
    float      seaLvl {};

    float    pnValBig {};
    float    pnValMid {};
    float    pnValSml {};
    float    altiVal  {};  //- target val

    size_t   pixIdx {};

    pixAltis_.resize( PIXES_PER_CHUNK * PIXES_PER_CHUNK, 0.0f );
    for( int h=0; h<PIXES_PER_CHUNK; h++ ){
        for( int w=0; w<PIXES_PER_CHUNK; w++ ){//- each pix in chunk

            pixCFPos.x = static_cast<float>(chunkPPos.x+w) / PIXES_PER_CHUNK;
            pixCFPos.y = static_cast<float>(chunkPPos.y+h) / PIXES_PER_CHUNK;
            //------------------//
            //     seaLvl
            //------------------//
            pixDistance = glm::distance( pixCFPos, worldCenter );
            pixDistance /= 10.0f;
            //--------
            seaLvl = simplex_noise2( pixCFPos * freqSeaLvl ) * 50.0f; // [-50.0, 50.0]
            seaLvl += pixDistance;
            if( seaLvl < 0.0f ){ //- land
                seaLvl *= 0.3f;  // [-15.0, 50.0]
            }
            //------------------//
            //    alti.val
            //------------------//
            //--- 使用速度最快的 2D-simplex-noise ---
            pnValBig = simplex_noise2( (pixCFPos + altiSeed_pposOffBig) * freqBig ) * 100.0f - seaLvl; // [-100.0, 100.0]
            pnValMid = simplex_noise2( (pixCFPos + altiSeed_pposOffMid) * freqMid ) * 50.0f  - seaLvl; // [-50.0, 50.0]
            pnValSml = simplex_noise2( (pixCFPos + altiSeed_pposOffSml) * freqSml ) * 20.0f  - seaLvl; // [-20.0, 20.0]
            //---------
            altiVal = floor(pnValBig + pnValMid + pnValSml);

            //------- 抹平头尾 -------//
            if( altiVal > 100.0f ){
                altiVal = 100.0f;
            }else if( altiVal < -100.0f ){
                altiVal = -100.0f;
            }
            // now, altiVal: [-100.0, 100.0]
            //------------------//
            //   写入 chunkData
            //------------------//
            pixIdx = to_size_t_cast( h * PIXES_PER_CHUNK + w );
            pixAltis_.at(pixIdx) = altiVal;
        }
    } //- each pix in chunk: end --
}


/* ===========================================================
 *                 calc_chunkData
 * -----------------------------------------------------------
 * -- 生成 chunkData 中数据
 */
void calc_chunkData(const IntVec2 &chunkMPos_, 
                    const std::vector<float> &pixAltis_,
                    ChunkData *chunkDataPtr_ ){

    RGBA      *texBufHeadPtr {}; //- mapTex
    RGBA       color {};

    IntVec2    tmpFieldMPos {};
    IntVec2    tmpFieldPPos {};

    IntVec2    tmpEntMPos {};
    IntVec2    pposOff {};   //- 通用偏移向量
    IntVec2    mposOff {};

    PixData   pixData {};//- each pix

    size_t    entIdx_in_chunk {};
    size_t    count {};

    //------------------------//
    //   mapEntAltis, mapEntMPoses    
    //------------------------//
    chunkDataPtr_->init_mapEntAltis();
    //-------
    std::vector<IntVec2> mapEntMPoses {}; //- 仅内部使用
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){
            mapEntMPoses.push_back( chunkMPos_ + IntVec2{w, h} ); //- copy
        }
    }

    //------------------------//
    //      fieldKeys
    //------------------------//
    std::vector<fieldKey_t> fieldKeys {}; //- 8*8 fieldKeys，仅函数内使用
    IntVec2    tmpFieldMpos {};
    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field in 8*8
            tmpFieldMpos = chunkMPos_ + IntVec2{    w*ENTS_PER_FIELD,
                                                    h*ENTS_PER_FIELD };
            fieldKeys.push_back( fieldMPos_2_fieldKey(tmpFieldMpos) );
        }
    }

    //------------------------//
    //        mapTex
    //------------------------//
    chunkDataPtr_->resize_texBuf();
    texBufHeadPtr = chunkDataPtr_->getnc_texBufHeadPtr();

    std::map<occupyWeight_t,FieldData> nearFour_fieldDatas {}; //- 一个 field 周边4个 field 数据
                                    // 按照 ecoObj.occupyWeight 倒叙排列（值大的在前面）

    for( const auto &fieldKey : fieldKeys ){ //- each field key

        //-- 收集 目标field 周边4个 field 实例指针  --
        tmpFieldMPos = colloect_nearFour_fieldDatas( nearFour_fieldDatas, fieldKey );
        tmpFieldPPos = mpos_2_ppos( tmpFieldMPos );

        for( int eh=0; eh<ENTS_PER_FIELD; eh++ ){
            for( int ew=0; ew<ENTS_PER_FIELD; ew++ ){ //- each ent in field

                tmpEntMPos = tmpFieldMPos + IntVec2{ ew, eh };
                mposOff = tmpEntMPos - chunkMPos_;
                entIdx_in_chunk = to_size_t_cast( mposOff.y * ENTS_PER_CHUNK + mposOff.x );

                for( int ph=0; ph<PIXES_PER_MAPENT; ph++ ){
                    for( int pw=0; pw<PIXES_PER_MAPENT; pw++ ){ //------ each pix in mapent ------

                        pixData.init( mpos_2_ppos(tmpEntMPos) + IntVec2{pw,ph} );
                        pposOff = pixData.ppos - mpos_2_ppos(chunkMPos_);
                        pixData.pixIdx_in_chunk = to_size_t_cast( pposOff.y * PIXES_PER_CHUNK + pposOff.x );

                        pixData.pixIdx_in_chunkTex = to_size_t_cast( pposOff.y * PIXES_PER_CHUNK_IN_TEXTURE + pposOff.x );
                        pixData.texPixPtr = texBufHeadPtr + pixData.pixIdx_in_chunkTex;

                        pposOff = pixData.ppos - tmpFieldPPos;
                        pixData.pixIdx_in_field = to_size_t_cast( pposOff.y * PIXES_PER_FIELD + pposOff.x );

                        //--------------------------------//
                        // 确定 pix 属于 周边4个field 中的哪一个
                        //--------------------------------//
                        count = 0;
                        for( auto &fieldPair : nearFour_fieldDatas ){ //--- 周边4个 field 信息
                            count++;
                            const FieldData &fieldDataRef = fieldPair.second;
                            if( count != nearFour_fieldDatas.size() ){   //- 前3个 field
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
                        pixData.alti.set( pixAltis_.at(pixData.pixIdx_in_chunk) );

                        //--------------------------------//
                        // 每个 mapent.mapAlti 被设置为其 中点pix 的 alti
                        //--------------------------------//
                        if( (ph==HALF_PIXES_PER_MAPENT) && (pw==HALF_PIXES_PER_MAPENT) ){//- ent 中点 pix
                            chunkDataPtr_->set_mapEntAlti( entIdx_in_chunk, pixData.alti );
                            esrc::atom_field_reflesh_min_and_max_altis( fieldKey, pixData.alti );
                        }
                        
                        //--------------------------------//
                        //    正式给 pix 上色
                        //--------------------------------//
                        color = pixData.fieldDataPtr->clac_pixColor();
                        color.a = 255;
                            //-- 当前版本，整个 chunk 都是实心的，water图层 被移动到了 chunk图层上方。
                        *pixData.texPixPtr = color;
                    }
                } //- each pix in mapent end ---
            }
        }//- each ent in field end -- 
    }//-- each field key end --

    //---------------------------//
    //  为了解决游戏中 “chunk间白线” 问题
    //  将 chunk.tex 上方／右方 两条 pix 颜色补齐
    //  补齐方案很简单，直接复制 相邻pix 的颜色
    //---------------------------//
    RGBA    *pixPtr      {};
    RGBA    *nearPixPtr  {};
    //--- chunk 上方那条 额外 pix ---
    for( int pw=0; pw<PIXES_PER_CHUNK_IN_TEXTURE; pw++ ){
        nearPixPtr = texBufHeadPtr + 
                    ((PIXES_PER_CHUNK_IN_TEXTURE-2) * PIXES_PER_CHUNK_IN_TEXTURE + pw);
        pixPtr = texBufHeadPtr + 
                    ((PIXES_PER_CHUNK_IN_TEXTURE-1) * PIXES_PER_CHUNK_IN_TEXTURE + pw);
        //---
        *pixPtr = *nearPixPtr;
    }
    //--- chunk 右侧那条 额外 pix ---
    for( int ph=0; ph<PIXES_PER_CHUNK_IN_TEXTURE; ph++ ){
        nearPixPtr = texBufHeadPtr + 
                    (ph * PIXES_PER_CHUNK_IN_TEXTURE + (PIXES_PER_CHUNK_IN_TEXTURE-2));
        pixPtr = texBufHeadPtr + 
                    (ph * PIXES_PER_CHUNK_IN_TEXTURE + (PIXES_PER_CHUNK_IN_TEXTURE-1));
        //---
        *pixPtr = *nearPixPtr;
    }
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
        std::pair<occupyWeight_t, MapFieldData_In_ChunkBuild> tmpPair = 
                    esrc::atom_get_mapFieldData_in_chunkBuild( tmpFieldKey );

        container_.insert({ -tmpPair.first, 
                            FieldData{  tmpPair.second, 
                                        fieldInfo.quad } }); //- copy
    }
    return targetFieldMPos;
}

}//-------------- namespace: bcd_inn end ----------------//


/*
 * ======================= Job_Chunk.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  以 chunk 为单位的，需要被 job线程 计算生成的 数据集 
 * ----------------------------
 */
#include "Job_Chunk.h"


//-------------------- Engine --------------------//
#include "simplexNoise.h"

#include "esrc_ecoObj.h"
#include "esrc_animFrameSet.h" // tmp

//-------------------- Script --------------------//
#include "Script/resource/ssrc_gameObj.h" // tmp
#include "Script/json/json_multiGoMesh.h"

namespace jobChunk_inn {//----------- namespace: jobChunk_inn ----------------//

    size_t calc_goMesh_windDelayIdx( const glm::dvec2 &dpos_ )noexcept;

}//-------------- namespace: jobChunk_inn end ----------------//


void Job_Chunk::init()noexcept{

    this->mapEntInns.resize( (ENTS_PER_CHUNK+2) * (ENTS_PER_CHUNK+2) );// 34*34 mapents
    this->job_fields.reserve( FIELDS_PER_CHUNK * FIELDS_PER_CHUNK );
    this->fields.reserve( FIELDS_PER_CHUNK * FIELDS_PER_CHUNK );

    IntVec2     tmpFieldMPos {};
    fieldKey_t  tmpFieldKey {};
    for( size_t h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( size_t w=0; w<FIELDS_PER_CHUNK; w++ ){
            tmpFieldMPos = this->chunkMPos + IntVec2{ w*ENTS_PER_FIELD, h*ENTS_PER_FIELD };
            tmpFieldKey = fieldMPos_2_fieldKey(tmpFieldMPos);
            //---
            auto outPair1 = this->job_fields.insert({ tmpFieldKey, std::make_unique<Job_Field>() });
            tprAssert( outPair1.second );
            //---
            auto outPair2 = this->fields.insert({ tmpFieldKey, std::make_unique<MapField>(tmpFieldMPos) });
            tprAssert( outPair2.second );
        }
    }
}


void Job_Chunk::write_2_field_from_jobData(){

    IntVec2     fieldMPos   {};
    fieldKey_t  fieldKey    {};    
    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field in chunk
            fieldMPos = this->chunkMPos + IntVec2{ w*ENTS_PER_FIELD, h*ENTS_PER_FIELD };
            fieldKey = fieldMPos_2_fieldKey( fieldMPos );
            //---
            auto &jobField = *(this->job_fields.at(fieldKey).get());
            auto &field    = *(this->fields.at(fieldKey).get());
            //---
            field.set_isCrossEcoObj( jobField.is_crossEcoObj() );
            field.set_isCrossColorTable( jobField.is_crossColorTable() );
        }
    }
}




// param: mposOff_ [0, 31]
bool Job_Chunk::is_borderMapEnt( IntVec2 mposOff_ )noexcept{

    tprAssert(  (mposOff_.x>=0) && (mposOff_.x<ENTS_PER_CHUNK) &&
                (mposOff_.y>=0) && (mposOff_.y<ENTS_PER_CHUNK) ); //- [0,31]

    IntVec2 offV = mposOff_ + IntVec2{1,1}; //- 现在的 mapEntInns 外凸了一圈 mapent [1,32]
    size_t idx = cast_2_size_t(offV.y * (ENTS_PER_CHUNK+2) + offV.x);
    tprAssert( idx < this->mapEntInns.size() );

    //---
    //colorTableId_t selfColorId = this->mapEntInns.at(idx).colorTableId;
    //colorTableId_t tmpColorId {};

    sectionKey_t selfEcoKey = this->mapEntInns.at(idx).ecoObjKey;
    sectionKey_t tmpEcoKey {};
    
    IntVec2 tmpOffV {};
    size_t  tmpIdx  {};

    //-- nearby_9_mapent --
    for( int j=-1; j<=1; j++ ){
        for( int i=-1; i<=1; i++ ){
            
            //-- skip self --
            if( (j==0) && (i==0) ){
                continue;
            }
            //-- skip outEnt --
            tmpOffV = offV + IntVec2{i,j}; // [0,33]

            tprAssert(  (tmpOffV.x>=0) && (tmpOffV.x<ENTS_PER_CHUNK+2) &&
                        (tmpOffV.y>=0) && (tmpOffV.y<ENTS_PER_CHUNK+2) ); //- [0,34]

            //-- do check --
            tmpIdx = static_cast<size_t>(tmpOffV.y * (ENTS_PER_CHUNK+2) + tmpOffV.x);

            tmpEcoKey = this->mapEntInns.at(tmpIdx).ecoObjKey;
            if( selfEcoKey != tmpEcoKey ){
                return true;
            }

        }
    }
    return false;
}


void Job_Chunk::create_field_goSpecDatas(){

    sectionKey_t    ecoObjKey {};
    IntVec2         mposOff {}; // from chunk left-bottom
    IntVec2         tmpFieldMPos {};
    fieldKey_t      tmpFieldKey {};
    size_t          randUValOff {};
    animSubspeciesId_t subSpecId {};
    size_t             windDelayIdx {};

    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field in chunk (8*8)
            tmpFieldMPos = this->chunkMPos + IntVec2{ w*ENTS_PER_FIELD, h*ENTS_PER_FIELD };
            tmpFieldKey = fieldMPos_2_fieldKey(tmpFieldMPos);
            //---
            auto &fieldRef = *(this->fields.at(tmpFieldKey));
            auto &job_fieldRef = *(this->job_fields.at(tmpFieldKey));

            if( fieldRef.is_land() ){ // skip water-field

                ecoObjKey = fieldRef.get_ecoObjKey();

                const auto &densityPool = esrc::atom_ecoObj_get_densityPool(ecoObjKey, fieldRef.get_density().get_idx() );

                const auto &fieldDistributePlan = densityPool.apply_a_fieldDistributePlan( fieldRef.get_uWeight() );

                for( const auto &pair : fieldDistributePlan.get_points() ){ // each point 

                    mposOff = dpos_2_mpos(fieldRef.get_midDPos() + pair.second) - this->chunkMPos;
                    auto &mapEntInnRef = this->getnc_mapEntInnRef( mposOff );
                    
                    if( !mapEntInnRef.alti.is_land() ){
                        continue;
                    }
                    if( !densityPool.isNeed2Apply(mapEntInnRef.uWeight) ){
                        continue;
                    }

                    const auto *goSpecDataPtr = densityPool.apply_a_goSpecDataPtr( pair.first, mapEntInnRef.uWeight );
                    const auto &goSpecRef = ssrc::get_goSpecRef( goSpecDataPtr->get_rootGoSpecId() );
                        
                    auto &job_goData = job_fieldRef.insert_new_job_goData();
                    job_goData.goDposOff = pair.second;
                    job_goData.goSpecId = goSpecDataPtr->get_rootGoSpecId();
                    job_goData.mapEntUWeight = mapEntInnRef.uWeight;


                    if( !goSpecDataPtr->get_isMultiGoMesh() ){
                        //--- single gomesh ---//
                        subSpecId = esrc::apply_a_random_animSubspeciesId(  goSpecRef.animFrameSetName, // e.g. "mushroom"
                                                                            goSpecDataPtr->get_animLabels(),
                                                                            mapEntInnRef.uWeight );
                        windDelayIdx = jobChunk_inn::calc_goMesh_windDelayIdx( job_goData.goDposOff ); // goMeshDPosOff is 0

                        job_goData.job_goMeshs.push_back( Job_GoMesh{   subSpecId, 
                                                                        glm::dvec2{0.0, 0.0},
                                                                        windDelayIdx } );

                    }else{
                        //--- multi gomeshs ---//
                        tprAssert( goSpecRef.multiGoMeshUPtr );

                        const auto &json_GoMeshSetRef = goSpecRef.multiGoMeshUPtr->apply_a_json_goMeshSet( 
                                            goSpecDataPtr->get_multiGoMeshType(), 
                                            mapEntInnRef.uWeight
                                            );

                        randUValOff = 0;
                        for( const auto &jgomesh : json_GoMeshSetRef.gomeshs ){ // each json_goMesh
                            randUValOff += 17;

                            subSpecId = esrc::apply_a_random_animSubspeciesId(  jgomesh.animFrameSetName,
                                                                                jgomesh.animLabels,
                                                                                mapEntInnRef.uWeight + randUValOff );
                            windDelayIdx = jobChunk_inn::calc_goMesh_windDelayIdx( job_goData.goDposOff + jgomesh.dposOff );
                                                                        
                            job_goData.job_goMeshs.push_back( Job_GoMesh{   subSpecId, 
                                                                            jgomesh.dposOff,
                                                                            windDelayIdx } );
                        }
                    }
                }
            }
        }
    }
}




namespace jobChunk_inn {//----------- namespace: jobChunk_inn ----------------//


//- 在完善的实现中，所有go 类型，都要标准自己是否受到 windClock 影响
//  对于那些不受影响的，直接放弃此值的生成
//  ...
//  目前，统一为所有 gomesh 生成此值
size_t calc_goMesh_windDelayIdx( const glm::dvec2 &dpos_ )noexcept{

    // 延迟帧数半径
    double rad = 60;

    double freq = 1.0 / ( static_cast<double>(PIXES_PER_MAPENT) * 7.0 ); // 麦浪分布圈，2*2 fields
    double x = dpos_.x * freq;
    double y = dpos_.y * freq;

    double perlin = simplex_noise2( x, y ); // [-1.0, 1.0]

    size_t delay = cast_2_size_t(std::abs(perlin+1.0 * rad)); // [0. 120]
    return delay;
    
        // 目前的 麦浪效果并不理想，只能说凑合着用
        //
}



}//-------------- namespace: jobChunk_inn end ----------------//





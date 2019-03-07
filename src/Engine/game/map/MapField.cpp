/*
 * ===================== MapField.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   4*4mapent 构成一个 field   [第二版]
 * ----------------------------
 */
#include "MapField.h"


//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "random.h"
#include "IntVec.h"
#include "srcs_engine.h" //- 所有资源
#include "ChunkFieldSet.h"


namespace{//----------- namespace ---------------//

    bool   is_randEngine_init   {false};    //- tmp
    std::default_random_engine  randEngine; //-随机数引擎实例
    std::uniform_int_distribution<int> uDistribution(0,
                                        PIXES_PER_FIELD-1); 
                                        // [0,19] 

}//-------------- namespace : end ---------------//



/* ===========================================================
 *                    init_firstOrderData
 * -----------------------------------------------------------
 *  仅 初始化 一阶数据
 * param: _mpos      -- 此 field 中的任意 mapent.mpos 
 * param: _chunkMPos -- 此 field 所属的 chunk mpos
 */
void MapField::init_firstOrderData( const IntVec2 &_anyMPos, const IntVec2 &_chunkMPos ){

    //--- 初始化 随机引擎的 一个不好的方法 ---
    if( is_randEngine_init == false ){
        is_randEngine_init = true;
        randEngine.seed( get_new_seed() );//- tmp
    }
    //--- field.mcpos ---
    mcpos.set_by_mpos( anyMPos_2_fieldMPos(_anyMPos) );
    //--- field.nodePPos ---
    IntVec2 lPPos { uDistribution(randEngine), 
                    uDistribution(randEngine) };
    nodePPos = mcpos.get_ppos() + lPPos;
    //--- field.lNodePPosOff ---
    lNodePPosOff = nodePPos - _chunkMPos;
    //--- field.fieldKey ---
    fieldKey = anyMPos_2_fieldKey( mcpos.get_mpos() );
    //----
    this->is_firstOrderData_init = true; //- MUST
}


/* ===========================================================
 *                    init_secondOrderData
 * -----------------------------------------------------------
 *  仅 初始化 二阶数据.
 *  将访问相邻 9个field 的 ChunkFieldSet / MapField 数据。
 *  且要确保，9个 field 都已完成 一阶数据init
 */
void MapField::init_secondOrderData(){

    IntVec2          fieldMPos = this->mcpos.get_mpos();
    IntVec2          nearbyFieldMPos {};
    chunkKey_t       chunkKey     {};
    ChunkFieldSet   *chunkFieldSetPtr;
    MapField        *tmpFieldPtr;

    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 周边 9 个 field
            //-- self --//
            if( (h==0) && (w==0) ){
                    assert( this->is_firstOrderData_init ); //- tmp
                this->nearby_field_nodePPoses.insert({  this->fieldKey, 
                                                        this->nodePPos });
                continue;
            }
            //---------
            nearbyFieldMPos.set(fieldMPos.x + w*ENTS_PER_FIELD,
                                fieldMPos.y + h*ENTS_PER_FIELD );
            
            chunkKey = anyMPos_2_chunkKey( nearbyFieldMPos );
            chunkFieldSetPtr = esrc::get_chunkFieldSetPtr(chunkKey);
            tmpFieldPtr = chunkFieldSetPtr->get_fieldPtr_by_mpos( nearbyFieldMPos );
                assert( tmpFieldPtr->is_firstOrderData_init ); //- tmp
            //----
            this->nearby_field_nodePPoses.insert({  tmpFieldPtr->fieldKey,
                                                    tmpFieldPtr->nodePPos });
        }
    }
    this->is_secondOrderData_init = true; //- MUST
}





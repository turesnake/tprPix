/*
 * ============= ChunkFieldSetInBuild.cpp ===================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    
 * ----------------------------
 */
#include "ChunkFieldSetInBuild.h"

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源
#include "MapField.h"

namespace sectionBuild { //------- namespace: sectionBuild ----------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void ChunkFieldSetInBuild::init( ChunkFieldSet *_chunkFieldSetPtr ){

    chunkFieldSetPtr = _chunkFieldSetPtr;

    //---------------------//
    //   fieldsInBuild
    //---------------------//
    IntVec2          nearbyFieldMPos {};
    IntVec2          fieldMPos_ {}; //- each Field
    u64_t            key        {}; //- chunkKey.key
    ChunkFieldSet  *fieldSetPtr;
    MapField         *fieldPtr;

    for( const auto &fieldRef : chunkFieldSetPtr->fields ){
        MapFieldInBuild  fieldInBuild {}; //- insert obj
        //--------
        //-- write fieldInBuild.nearby_nodePPoses ----//
        for( int h=-1; h<=1; h++ ){ //- 周边 9 个 field
            for( int w=-1; w<=1; w++ ){
                //-- self --//
                if( (h==0) && (w==0) ){
                    fieldInBuild.nearby_nodePPoses.push_back( fieldRef.nodePPos ); //- copy
                    continue;
                }
                //---------
                fieldMPos_ = fieldRef.mcpos.get_mpos();
                nearbyFieldMPos.set(fieldMPos_.x + w*MapField::entSideLen,
                                    fieldMPos_.y + h*MapField::entSideLen );
                key = just_creat_chunkKey_by_mpos( nearbyFieldMPos );
                    assert( esrc::chunkFieldSets.find(key) != esrc::chunkFieldSets.end() ); //- tmp
                fieldSetPtr = (ChunkFieldSet*)&(esrc::chunkFieldSets.at(key));
                fieldPtr = fieldSetPtr->get_mapFieldPtr_by_mpos( nearbyFieldMPos );
                //----
                fieldInBuild.nearby_nodePPoses.push_back( fieldPtr->nodePPos ); //- copy
            }
        }
        //--------
        fieldsInBuild.push_back( fieldInBuild ); //- copy
    }


}


}//----------------- namespace: sectionBuild: end -------------------//

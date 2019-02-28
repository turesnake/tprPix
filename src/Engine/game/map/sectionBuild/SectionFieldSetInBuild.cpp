/*
 * ============= SectionFieldSetInBuild.cpp ===================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section "二级距离场"集 
 * ----------------------------
 */
#include "SectionFieldSetInBuild.h"

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源
#include "MapField.h"

namespace sectionBuild { //------- namespace: sectionBuild ----------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void SectionFieldSetInBuild::init( SectionFieldSet *_sectionFieldSetPtr ){

    sectionFieldSetPtr = _sectionFieldSetPtr;

    //---------------------//
    //   fieldsInBuild
    //---------------------//
    IntVec2          nearbyFieldMPos {};
    IntVec2          fieldMPos_ {}; //- each Field
    u64_t            key       {}; //- sectionKey.key
    SectionFieldSet  *fieldSetPtr;
    MapField         *fieldPtr;

    for( const auto &fieldRef : sectionFieldSetPtr->fields ){
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
                key = just_creat_sectionKey_by_mpos( nearbyFieldMPos );
                    assert( esrc::sectionFieldSets.find(key) != esrc::sectionFieldSets.end() ); //- tmp
                fieldSetPtr = (SectionFieldSet*)&(esrc::sectionFieldSets.at(key));
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

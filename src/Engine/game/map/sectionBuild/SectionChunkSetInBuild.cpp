/*
 * ============= SectionChunkSetInBuild.cpp ===================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section "二级距离场"集 
 * ----------------------------
 */
#include "SectionChunkSetInBuild.h"

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源

namespace sectionBuild { //------- namespace: sectionBuild ----------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void SectionChunkSetInBuild::init( SectionChunkSet *_sectionChunkSetPtr ){

    sectionChunkSetPtr = _sectionChunkSetPtr;

    //---------------------//
    //   chunksInBuild
    //---------------------//
    IntVec2          nearbyChunkMPos {};
    IntVec2          chunkMPos_ {}; //- each chunk 
    u64_t            key       {}; //- sectionKey.key
    SectionChunkSet  *chunkSetPtr;
    MapChunk         *chunkPtr;

    for( const auto &chunkRef : sectionChunkSetPtr->chunks ){
        MapChunkInBuild  chunkInBuild {}; //- insert obj
        //--------
        //-- write chunkInBuild.nearby_nodePPoses ----//
        for( int h=-1; h<=1; h++ ){ //- 周边 9 个 chunk
            for( int w=-1; w<=1; w++ ){
                //-- self --//
                if( (h==0) && (w==0) ){
                    chunkInBuild.nearby_nodePPoses.push_back( chunkRef.nodePPos ); //- copy
                    continue;
                }
                //---------
                chunkMPos_ = chunkRef.mcpos.get_mpos();
                nearbyChunkMPos.set(chunkMPos_.x + w*MapChunk::entSideLen,
                                    chunkMPos_.y + h*MapChunk::entSideLen );
                key = just_creat_sectionKey_by_mpos( nearbyChunkMPos );
                    assert( esrc::sectionChunkSets.find(key) != esrc::sectionChunkSets.end() ); //- tmp
                chunkSetPtr = (SectionChunkSet*)&(esrc::sectionChunkSets.at(key));
                chunkPtr = chunkSetPtr->get_mapChunkPtr_by_mpos( nearbyChunkMPos );
                //----
                chunkInBuild.nearby_nodePPoses.push_back( chunkPtr->nodePPos ); //- copy
            }
        }
        //--------
        chunksInBuild.push_back( chunkInBuild ); //- copy
    }


}


}//----------------- namespace: sectionBuild: end -------------------//

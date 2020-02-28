/*
 * ========================= create_goes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CREATE_GOES_H
#define TPR_CREATE_GOES_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "Density.h"
#include "dyParams.h"
#include "UIAnchor.h"
#include "Chunk.h"


//--- need ---//
class Job_Chunk;
class GameObj;
class DiskGameObj;



void create_gos_in_field(   fieldKey_t fieldKey_, 
                            const Chunk &chunkRef_,
                            const Job_Chunk &job_chunkRef_ );


namespace gameObjs{//------------- namespace gameObjs ----------------


goid_t create_a_Go( goSpeciesId_t goSpeciesId_,
                    const glm::dvec2 &dpos_,
                    const DyParam &dyParams_ );

void rebind_a_disk_Go(  goid_t          diskGoId_,
                        goSpeciesId_t   diskGoSpeciesId_,
                        const glm::dvec2 &dpos_,
                        const DyParam &dyParams_  );

// by hand
goid_t create_go_from_goDataForCreate( const GoDataForCreate *goDPtr_ );


void rebind_diskGo_by_hand(   const DiskGameObj &diskGo_ );


void signUp_newGO_to_chunk_and_mapEnt( GameObj &goRef_ );


}//------------- namespace gameObjs: end ----------------











namespace uiGos{//------------- namespace uiGos ----------------

goid_t create_a_UIGo( goSpeciesId_t goSpeciesId_,
                    const glm::dvec2 &basePointProportion_, 
                    const glm::dvec2 &offDPos_,
                    const DyParam &dyParams_ );


goid_t create_a_UIGo( goSpeciesId_t goSpeciesId_,
                    const UIAnchor &uiAnchor_,
                    const DyParam &dyParams_ );

}//------------- namespace uiGos: end ----------------









#endif 


/*
 * ====================== esrc_job_ecoObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.30
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_JOB_ECO_OBJ_H
#define TPR_ESRC_JOB_ECO_OBJ_H

//-------------------- Engine --------------------//
#include "sectionKey.h"
#include "EcoObj.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


EcoObj &atom_insert_new_job_ecoObj( sectionKey_t ecoObjKey_ );

void atom_insert_2_job_ecoObjFlags( sectionKey_t ecoObjKey_ );
size_t atom_move_all_ecoObjUptrs_from_job_2_esrc();


}//---------------------- namespace: esrc -------------------------//
#endif


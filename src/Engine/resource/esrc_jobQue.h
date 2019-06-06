/*
 * ========================= esrc_jobQue.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_JOB_QUE_H_
#define TPR_ESRC_JOB_QUE_H_


//-------------------- Engine --------------------//
#include "Job.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


void atom_exitJobThreadsFlag_store( bool _val );
bool atom_exitJobThreadsFlag_load();

void atom_push_back_2_jobQue( const Job &_job );
bool atom_is_jobQue_empty();
Job  atom_pop_from_jobQue();



}//---------------------- namespace: esrc -------------------------//
#endif 


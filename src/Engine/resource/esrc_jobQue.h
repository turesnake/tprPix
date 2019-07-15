/*
 * ========================= esrc_jobQue.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_JOB_QUE_H
#define TPR_ESRC_JOB_QUE_H

//-------------------- CPP --------------------//
#include <memory>

//-------------------- Engine --------------------//
#include "Job.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


void atom_exitJobThreadsFlag_store( bool _val );
bool atom_exitJobThreadsFlag_load();

void atom_push_back_2_jobQue( std::shared_ptr<Job> jobSPtr_ );
bool atom_is_jobQue_empty();
std::shared_ptr<Job>  atom_pop_from_jobQue();



}//---------------------- namespace: esrc -------------------------//
#endif 


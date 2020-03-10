/*
 * =================== create_job_ecoObj.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  job: build job_ecoObj
 * ----------------------------
 */
#include "pch.h"

//-------------------- CPP --------------------//
#include <cstring>

//-------------------- Engine --------------------//
#include "jobs_all.h"
#include "Job.h"
#include "simplexNoise.h"
#include "MapTexture.h" 
#include "Quad.h"
#include "random.h"

#include "esrc_job_ecoObj.h"
#include "esrc_gameSeed.h"
#include "esrc_field.h"
#include "esrc_ecoObj.h"

#include "Job_MapEnt.h"
#include "Job_Field.h"



namespace cjeo_inn {//----------- namespace: cjeo_inn ----------------//

    //-- 不要随意建立 static数据，本文件的代码，会被数个 job线程 调用 --





    //===== funcs =====//
    
    
}//-------------- namespace: cjeo_inn end ----------------//



/* ===========================================================
 *                create_job_ecoObj_main
 * -----------------------------------------------------------
 * 在未来，这个函数需要写进 atom 函数中 ...
 */
void create_job_ecoObj_main( const Job &job_ ){

    //-------------------//
    //   job.argBinary
    //-------------------//
    const auto *jobParamPtr = job_.get_param<ArgBinary_Create_Job_EcoObj>();

    IntVec2 ecoObjMPos = sectionKey_2_mpos( jobParamPtr->ecoObjKey );

    //--------------------------//
    //      create job_chunk
    //--------------------------//
    EcoObj &ecoObjRef = esrc::atom_insert_new_job_ecoObj( jobParamPtr->ecoObjKey );


    //--------------------------//
    //    正式 创建 ecoobj 数据
    //--------------------------//
    ecoObjRef.init( jobParamPtr->ecoObjKey );
   
    //--------------------------//
    //-- job_ecoObj 数据计算完成后，向 状态表 添加一个元素
    //   以此来提醒 主线程，这个 ecoObj 数据准备好了
    //--------------------------//
    esrc::atom_insert_2_job_ecoObjFlags( jobParamPtr->ecoObjKey );
}


namespace cjeo_inn {//----------- namespace: cjeo_inn ----------------//










}//-------------- namespace: cjeo_inn end ----------------//


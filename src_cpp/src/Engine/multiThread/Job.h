/*
 * ========================== Job.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  将任务封装成不同的 job，发送给 job线程
 * ----------------------------
 */
#ifndef TPR_JOB_H_
#define TPR_JOB_H_

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Libs --------------------//
#include "tprDataType.h"

//-------------------- Engine --------------------//
#include "JobType.h"

class Job{
public:
    JobType           jobType    {JobType::Null};
    std::vector<u8_t> argBinary  {}; //- 相关参数
};




#endif 


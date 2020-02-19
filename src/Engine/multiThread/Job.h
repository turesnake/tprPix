/*
 * ========================== Job.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  将任务封装成不同的 job，发送给 job线程
 * ----------------------------
 */
#ifndef TPR_JOB_H
#define TPR_JOB_H

//-------------------- CPP --------------------//
#include <vector>
#include <any>

//-------------------- Engine --------------------//
#include "JobType.h"
#include "tprAssert.h"

class Job{
public:

    template<typename T>
    inline T *init_param()noexcept{
        this->param = T {};
        return std::any_cast<T>( &(this->param) );
    }

    template<typename T>
    inline const T *get_param()const noexcept{
        tprAssert( this->param.has_value() );
        tprAssert( this->param.type().hash_code() == typeid(T).hash_code() );
        return std::any_cast<T>( &(this->param) );
    }

    inline void set_jobType( JobType type_ ){ this->jobType = type_; }
    inline JobType get_jobType()const noexcept{ return this->jobType; }

private:
    JobType     jobType    {JobType::Null};
    std::any    param;
};


#endif 


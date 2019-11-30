/*
 * ========================== JobType.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JOB_TYPE_H
#define TPR_JOB_TYPE_H



enum class JobType : int{
    Null = 0,
    JustTimeOut = 1, //- 不是 job 类型，而是告诉 job线程：
                     //  “本次返回仅仅是因为 时间到了，请再次尝试 提取job”
    Create_Job_Chunk,
    Create_Job_EcoObj,
};



#endif 


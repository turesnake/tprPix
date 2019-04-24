/*
 * ========================== JobType.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_JOB_TYPE_H_
#define _TPR_JOB_TYPE_H_



enum class JobType : int{
    Null = 0,
    JustTimeOut = 1, //- 不是 job 类型，而是告诉 job线程：
                     //  “本次返回仅仅是因为 时间到了，请再次尝试 提取job”
    Build_ChunkData
};




#endif 


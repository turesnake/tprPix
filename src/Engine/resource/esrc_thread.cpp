/*
 * ======================= esrc_thread.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.25
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
//-------------------- C --------------------//
//#include <cassert>

//-------------------- CPP --------------------//
#include <thread>

//-------------------- Engine --------------------//
#include "esrc_thread.h"
#include "esrc_jobQue.h"
#include "jobThread.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

    
namespace {//------------ namespace --------------//

    //-- 一组 thread 空实例 [1~4]个 --
    std::thread jobThread_1;
    std::thread jobThread_2;
    std::thread jobThread_3;

}//---------------- namespace end --------------//




/* ===========================================================
 *              start_jobThreads
 * -----------------------------------------------------------
 */
void start_jobThreads(){
    //-- 创建一个 临时thread实例，并隐式 move 其所有权 到 空实例中 
    jobThread_1 = std::thread( jobThread_main );
    jobThread_2 = std::thread( jobThread_main );
    jobThread_3 = std::thread( jobThread_main );

}

/* ===========================================================
 *              join_jobThreads
 * -----------------------------------------------------------
 */
void join_jobThreads(){

    //-- 改写全局标识，允许 jobThreads 自行exit --
    esrc::atom_exitJobThreadsFlag_store( true );

    //-- 等待 jobThreads exit --
    jobThread_1.join();
    jobThread_2.join();
    jobThread_3.join();
}






}//---------------------- namespace: esrc -------------------------//


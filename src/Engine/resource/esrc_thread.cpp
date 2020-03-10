/*
 * ======================= esrc_thread.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.25
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
//-------------------- CPP --------------------//
#include <thread>

//-------------------- Engine --------------------//
#include "esrc_thread.h"
#include "esrc_jobQue.h"
#include "jobThread.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

    
namespace thread_inn {//------------ namespace: thread_inn --------------//

    //-- 一组 thread 空实例 [1~7]个 --
    std::thread jobThread_1;
    
    std::thread jobThread_2;
    
    std::thread jobThread_3;
    std::thread jobThread_4;
    std::thread jobThread_5;
    std::thread jobThread_6;
    std::thread jobThread_7;
    
    
    

}//---------------- namespace: thread_inn end --------------//


/* ===========================================================
 *              start_jobThreads
 * -----------------------------------------------------------
 */
void start_jobThreads(){
    //-- 创建一个 临时thread实例，并隐式 move 其所有权 到 空实例中 
    thread_inn::jobThread_1 = std::thread( jobThread_main );
    
    thread_inn::jobThread_2 = std::thread( jobThread_main );
    
    thread_inn::jobThread_3 = std::thread( jobThread_main );
    thread_inn::jobThread_4 = std::thread( jobThread_main );
    thread_inn::jobThread_5 = std::thread( jobThread_main );
    thread_inn::jobThread_6 = std::thread( jobThread_main );
    thread_inn::jobThread_7 = std::thread( jobThread_main );
    

}

/* ===========================================================
 *              join_jobThreads
 * -----------------------------------------------------------
 */
void join_jobThreads(){

    //-- 改写全局标识，允许 jobThreads 自行exit --
    esrc::atom_exitJobThreadsFlag_store( true );

    //-- 等待 jobThreads exit --
    thread_inn::jobThread_1.join();
    
    thread_inn::jobThread_2.join();
    
    thread_inn::jobThread_3.join();
    thread_inn::jobThread_4.join();
    thread_inn::jobThread_5.join();
    thread_inn::jobThread_6.join();
    thread_inn::jobThread_7.join();
    
}


}//---------------------- namespace: esrc -------------------------//


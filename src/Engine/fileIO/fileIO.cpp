/*
 * ======================= fileIO.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.07.08
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#include "fileIO.h"

//--------------- Libs ------------------//
#include "tprGeneral.h"

#include "SysConfig.h" // MUST BEFORE TPR_OS_WIN32_ !!!
#if defined TPR_OS_WIN32_ 
    #include "tprFileSys_win.h"
#elif defined TPR_OS_UNIX_ 
    #include "tprFileSys_unix.h"
#endif

//-------------------- Engine --------------------//
#include "tprAssert.h"

/* ===========================================================
 *                    read_a_file
 * -----------------------------------------------------------
 * cross platform
 */
std::unique_ptr<std::string> read_a_file( const std::string &filePath_ ){

    auto bufUPtr = std::make_unique<std::string>();
    //-- read files --
#if defined TPR_OS_WIN32_
    tprWin::file_load( filePath_, *(bufUPtr.get()) );
#elif defined TPR_OS_UNIX_
    tprUnix::file_load( filePath_, *(bufUPtr.get()) );
#endif
    return bufUPtr;
}


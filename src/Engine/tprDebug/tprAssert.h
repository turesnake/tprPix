/*
 * ======================= tprAssert.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * a poor assert macro, support win/unix
 * ---------------------
 */
#ifndef TPR_ASSERT_H
#define TPR_ASSERT_H

#include "SysConfig.h"

/* ===========================================================
 *                    tprAssert_inn
 * -----------------------------------------------------------
 */
#if defined TPR_OS_WIN32_

    #include <windows.h>
    #include "fmt/format.h"
    inline int tprMessageBox( const std::string &e_, const std::string &file_, int line_ ){
        std::string str = fmt::format(
            "ERROR: {0}\n" \
            "file: {1}: {2}",
            e_, file_, line_
        );
        MessageBox( nullptr, str.c_str(), "tprAssert", MB_OK);
        exit(-99);  
        return 0; //- never reach                                        
    }
    #define tprAssert_inn(e,File,Line)  (e) ? 0 : tprMessageBox(#e, File,Line)

#elif defined TPR_OS_UNIX_
    // allow assert in release mode 
    // 如果不取消 NDEBUG，将在 release mode 引发关于 OpenGL_glad 的奇怪的 Segmentation fault 错误...
    #ifdef NDEBUG 
        #undef NDEBUG 
    #endif
    #include <cassert>
    #define tprAssert_inn(e,File,Line) assert(e);
#endif


/* ===========================================================
 *                    tprAssert
 * -----------------------------------------------------------
 */
#define tprAssert(e) tprAssert_inn(e,__FILE__, __LINE__)


#endif 


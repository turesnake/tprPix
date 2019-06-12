/*
 * ======================= tprAssert.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * a poor assert macro, support win/unix
 * ---------------------
 */
#ifndef TPR_ASSERT_H_
#define TPR_ASSERT_H_


#include "SysConfig.h"


/* ===========================================================
 *                    tprAssert_inn
 * -----------------------------------------------------------
 */
#ifdef TPR_OS_WIN32_

    #include <windows.h>
    #include <sstream>   //-- stringstream
    inline int tprMessageBox(  const std::string &_e,
                                const std::string &_file,
                                int _line ){
        std::stringstream ss;                               
        ss << _e << "\n";                                 
        ss << _file << ": " << _line;                  
        MessageBox( nullptr, ss.str().c_str(), "tprAssert", MB_OK);
        exit(-99);  
        return 0; //- never reach                                        
    }
    #define tprAssert_inn(e,File,Line)  (e) ? 0 : tprMessageBox(#e, File,Line)

#else
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


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


#ifdef TPR_OS_WIN32_

    #include <windows.h>
    #include <sstream>   //-- stringstream

    inline void tprMessageBox(  const std::string &_e,
                                const std::string &_file,
                                int _line ){
        std::stringstream ss;                               
        ss << _e << "\n";                                 
        ss << _file << ": " << _line;                  
        MessageBox(NULL, ss.str().c_str(), "tprAssert", MB_OK);
        exit(-99);                                          
    }
    #define tprAssert(e)  (e) ? (void)0 : tprMessageBox(#e, __FILE__, __LINE__);

#else
    #include <cassert>
    #define tprAssert(e) assert(e);
#endif



#endif 


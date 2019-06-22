/*
 * ==================== cpp_with_csharp.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CPP_WITH_CSHARP_H_
#define TPR_CPP_WITH_CSHARP_H_


#include "SysConfig.h" //- must before TPR_OS_WIN32_
#if defined    TPR_OS_WIN32_
    #define PPPP__1 __declspec(dllexport)
    #define PPPP__2 __cdecl
#elif defined  TPR_OS_UNIX_
    #define PPPP__1
    #define PPPP__2
#endif

#ifdef __cplusplus
extern "C" {
#endif//----------------------- "C" ---------------------------//


 PPPP__1 int PPPP__2 cppMain_tmp(  char *exeDirPath_  );







#ifdef __cplusplus //-------- - "C" ---------------------------//
}
#endif

#undef PPPP__1
#undef PPPP__2

#endif 




/*
 * ========================= tprDataType.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.15
 *                                        修改 -- 2018.10.15
 * ----------------------------------------------------------
 *   可以被 几乎所谓文件／库 重复调用的 通用h文件, 
 *   数据类型 
 * ----------------------------
 */

//-------- integer --------//
#ifndef _TPR_U8_
    #define _TPR_U8_
    //typedef unsigned char u8;
    using u8_t = unsigned char;
#endif

#ifndef _TPR_I16_
    #define _TPR_I16_
    //typedef short          i16;
    using i16_t = short;
#endif

#ifndef _TPR_U16_
    #define _TPR_U16_
    //typedef unsigned short u16;
    using u16_t = unsigned short;
#endif

#ifndef _TPR_I32_
    #define _TPR_I32_
    //typedef int           i32;
    using i32_t = int;
#endif

#ifndef _TPR_U32_
    #define _TPR_U32_
    //typedef unsigned int  u32;
    using u32_t = unsigned int;
#endif

#ifndef _TPR_I64_
    #define _TPR_I64_
    //typedef  long long    i64;
    using i64_t =  long long;
#endif

#ifndef _TPR_U64_
    #define _TPR_U64_
    //typedef unsigned long long u64;
    using u64_t =  unsigned long long;
#endif

//--------- bytes --------//
#ifndef _TPR_KB_
    #define _TPR_KB_ 1024
#endif

#ifndef _TPR_MB_
    #define _TPR_MB_ (1024 * _TPR_KB_)
#endif

#ifndef _TPR_GB_
    #define _TPR_GB_ (1024 * _TPR_MB_)
#endif






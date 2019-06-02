/*
 * ========================= tprDataType.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.10.15
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   support all files/libs
 *   data type
 * ----------------------------
 */
#ifndef _TPR_DATA_TYPE_H_
#define _TPR_DATA_TYPE_H_


//-------- integer --------//
#ifndef _TPR_U8_
    #define _TPR_U8_
    using u8_t = unsigned char;
#endif

#ifndef _TPR_I16_
    #define _TPR_I16_
    using i16_t = short;
#endif

#ifndef _TPR_U16_
    #define _TPR_U16_
    using u16_t = unsigned short;
#endif

#ifndef _TPR_I32_
    #define _TPR_I32_
    using i32_t = int;
#endif

#ifndef _TPR_U32_
    #define _TPR_U32_
    using u32_t = unsigned int;
#endif

#ifndef _TPR_I64_
    #define _TPR_I64_
    using i64_t =  long long;
#endif

#ifndef _TPR_U64_
    #define _TPR_U64_
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



#endif


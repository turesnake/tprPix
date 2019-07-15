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
#ifndef TPR_DATA_TYPE_H_
#define TPR_DATA_TYPE_H_


//-------- integer --------//
#ifndef TPR_U8_
    #define TPR_U8_
    using u8_t = unsigned char;
#endif

#ifndef TPR_I16_
    #define TPR_I16_
    using i16_t = short;
#endif

#ifndef TPR_U16_
    #define TPR_U16_
    using u16_t = unsigned short;
#endif

#ifndef TPR_I32_
    #define TPR_I32_
    using i32_t = int;
#endif

#ifndef TPR_U32_
    #define TPR_U32_
    using u32_t = unsigned int;
#endif

#ifndef TPR_I64_
    #define TPR_I64_
    using i64_t =  long long;
#endif

#ifndef TPR_U64_
    #define TPR_U64_
    using u64_t =  unsigned long long;
#endif


//--------- bytes --------//
#ifndef TPR_KB_
    #define TPR_KB_ 1024
#endif

#ifndef TPR_MB_
    #define TPR_MB_ (1024 * _TPR_KB_)
#endif

#ifndef TPR_GB_
    #define TPR_GB_ (1024 * _TPR_MB_)
#endif



#endif


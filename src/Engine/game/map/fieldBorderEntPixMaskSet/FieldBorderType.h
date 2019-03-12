/*
 * ==================== FieldBorderType.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   表述 mapent 在其 field 内的 边缘程度
 * ----------------------------
 */
#ifndef _TPR_FIELD_BORDER_TYPE_H_
#define _TPR_FIELD_BORDER_TYPE_H_


enum class FieldBorderType{
    Nearest  = 0, //- 最边缘 mapent
    Sec   = 1, //- 次边缘
    Thd   = 2, //- 次次边缘
    Inner = 3  //- 内部
};


#endif 


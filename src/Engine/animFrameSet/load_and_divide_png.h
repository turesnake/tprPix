/*
 * =================== load_and_divide_png.h =================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_LOAD_AND_DIVIDE_PNG_H
#define TPR_LOAD_AND_DIVIDE_PNG_H

#include "pch.h"


//------------------- Engine --------------------//
#include "RGBA.h"


IntVec2 load_and_divide_png(const std::string &path_,
                            IntVec2     frameNum_,
                            size_t         totalFrameNum_,
        std::vector< std::vector<RGBA>> &frame_data_ary_ );


#endif 

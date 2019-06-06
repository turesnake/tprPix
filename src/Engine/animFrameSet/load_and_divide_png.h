/*
 * =================== load_and_divide_png.h =================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_LOAD_AND_DIVIDE_PNG_H_
#define TPR_LOAD_AND_DIVIDE_PNG_H_

//-------------------- CPP --------------------//
#include <vector>
#include <string>

//------------------- Engine --------------------//
#include "RGBA.h"
#include "IntVec.h"


IntVec2 load_and_divide_png(const std::string &_path,
                            const IntVec2 &_frameNum,
                            int            _totalFrameNum,
        std::vector< std::vector<RGBA>> &_frame_data_ary );


#endif 

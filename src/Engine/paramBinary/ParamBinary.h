/*
 * ======================= ParamBinary.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_PARAM_BINARY_H
#define TPR_PARAM_BINARY_H

//-------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 


enum class ParamBinaryType{
    Nil,
    MapSurface,
    Tree, //- 临时简陋版
};


//-- 参数二进制包，目前仅用于 go create 中的 动态参数传递
//   一个很简陋的方案
class ParamBinary{
public:
    ParamBinary() = default;

    inline void resize_binary( size_t size_ ){
        this->binary.resize(size_);
    }
    inline const u8_t *get_binaryPtr() const {
        return &(this->binary.at(0));
    }

    ParamBinaryType    type {ParamBinaryType::Nil};
private:
    std::vector<u8_t>  binary {}; //- 数据本体
};


//- 空参数包 -
inline ParamBinary emptyParamBinary {};


#endif 


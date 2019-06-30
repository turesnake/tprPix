/*
 * ========================= ColliEntSetLoader.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    ColliEntSet png数据 加载器
 * ----------------------------
 */
#ifndef TPR_COLLI_ENT_SET_LOADER_H
#define TPR_COLLI_ENT_SET_LOADER_H

//-------------------- CPP --------------------//
#include <vector>
#include <string>

//-------------------- Engine --------------------//
#include "ColliEntSet.h"


//-- 需要load 的 png文件 可能只有 一张 --
// 本实例起到一个 数据中转站的 功能：
// -- 在一个函数体内CREATE 本实例
// -- 从 png图中读取 并解析信息，整理成 数据集
// -- 将 数据存入 全局容器
// -- 离开函数，顺带销毁 本实例
class ColliEntSetLoader{
public:
    ColliEntSetLoader(  const std::string &lpath_,
                        IntVec2  frameNum_,
                        size_t   totalFrameNum_
                        ):
        lpath(lpath_),
        frameNum(frameNum_),
        totalFrameNum(totalFrameNum_)
        {}

    void init();

    //---- get ----//
    const ColliEntSet &get_collientSet( size_t idx_ ) const {
        return this->collientSets.at(idx_); //-- auto throw error
    }

private:
    std::string  lpath {};

    IntVec2  pixNum_per_frame {};   //- 单帧画面 的 长宽 像素值
    IntVec2  frameNum {};         //- 画面中，横排可分为几帧，纵向可分为几帧
    size_t   totalFrameNum {};    //- 总 图元帧 个数

    std::vector<ColliEntSet> collientSets {}; //- 临时存储区
};


#endif 


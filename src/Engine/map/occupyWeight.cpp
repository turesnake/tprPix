/*
 * ========================== occupyWeight.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "occupyWeight.h"


namespace ow_inn {//-------- namespace: ow_inn --------------//

    //- 为了避免相邻的 4顶点 不出现相同的 occupyWeight
    //  首先根据 顶点在 地图上的 奇偶性， 将每个 顶点 划分为 4种类型。
    //  然后在 对应的类型中，取随机值。
    //  通过 "之"字 来排列 取值分布
    const std::vector<std::vector<occupyWeight_t>> occupyWeights{
        std::vector<occupyWeight_t>{ 0, 6, 10, 15 }, //- {0,0}
        std::vector<occupyWeight_t>{ 1, 7,  9, 12 }, //- {0,1}
        std::vector<occupyWeight_t>{ 2, 4, 11, 14 }, //- {1,0}
        std::vector<occupyWeight_t>{ 3, 5,  8, 13 }  //- {1,1}
    };

}//------------- namespace: ow_inn end --------------//


/* ===========================================================
 *                  calc_occupyWeight
 * -----------------------------------------------------------
 * param: oddEven_ -- 目标点在map 中的 坐标奇偶性，必须是 {0,0}; {1,0}; {0,1}; {1,1} 中的一种
 * param: randIdx_ -- 从 occupyWeights 子容器中 选择具体值。
 */
occupyWeight_t calc_occupyWeight( IntVec2 oddEven_, size_t randIdx_ ){

    tprAssert( (oddEven_.x>=0) && (oddEven_.x<=1) && 
            (oddEven_.y>=0) && (oddEven_.y<=1) );

    size_t containerIdx = cast_2_size_t( oddEven_.y * 2 + oddEven_.x );
        //tprAssert( containerIdx < occupyWeights.size() ); //- tmp
    const std::vector<occupyWeight_t> &container = ow_inn::occupyWeights.at(containerIdx);

    size_t idx = randIdx_ % container.size(); //- mod
    return container.at(idx);
}


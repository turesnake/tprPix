/*
 * ========================= colliEntSets_mem.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.14
 *                                        修改 -- 2019.01.14
 * ----------------------------------------------------------
 *  资源管理:  colliEntSets
 * ----------------------------
 */
//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "ColliEntSet.h"
#include "ColliEntSetLoader.h"
#include "srcs_engine.h" //- 所有资源

using std::string;

//#include "debug.h" //- tmp

namespace esrc{ //------------------ namespace: esrc -------------------------//

/* ===========================================================
 *                   colliEntSets_load
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一加载并初始化 所有 colliEntSets 资源
 */
void colliEntSets_load(){

    //-- 创建 ColliEntSetLoader 实例 --
    ColliEntSetLoader  loader { "colliEntSet_1.png",
                                PixVec2{ 21, 21 },
                                PixVec2{ 4, 4 },
                                16 };
    
    //-- 解析 图片数据，将数据存储自身容器中 --
    loader.init();


    //-- 将 loader 中的数据，手动 搬运到 全局容器中  --
    esrc::colliEntSets.insert({ "ces_1_1", loader.get_collientSet(0) }); //- copy
    esrc::colliEntSets.insert({ "ces_1_2", loader.get_collientSet(1) });

    esrc::colliEntSets.insert({ "ces_2_1", loader.get_collientSet(2) });
    esrc::colliEntSets.insert({ "ces_2_2", loader.get_collientSet(3) });

    esrc::colliEntSets.insert({ "ces_3_3", loader.get_collientSet(4) });
    esrc::colliEntSets.insert({ "ces_3_2", loader.get_collientSet(5) });
    esrc::colliEntSets.insert({ "ces_2_3", loader.get_collientSet(6) });

    esrc::colliEntSets.insert({ "ces_4_3", loader.get_collientSet(7) });
    esrc::colliEntSets.insert({ "ces_5_3", loader.get_collientSet(8) });

    esrc::colliEntSets.insert({ "ces_4_4", loader.get_collientSet(9) });
    esrc::colliEntSets.insert({ "ces_5_4", loader.get_collientSet(10) });
    esrc::colliEntSets.insert({ "ces_5_5", loader.get_collientSet(11) });

    esrc::colliEntSets.insert({ "ces_6_5", loader.get_collientSet(12) });
    esrc::colliEntSets.insert({ "ces_6_6", loader.get_collientSet(13) });
    esrc::colliEntSets.insert({ "ces_7_6", loader.get_collientSet(14) });
    esrc::colliEntSets.insert({ "ces_7_7", loader.get_collientSet(15) });

    //-- 自动销毁 ColliEntSetLoader 实例 --
}


}//---------------------- namespace: esrc -------------------------//


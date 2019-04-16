/*
 * =================== EcoSysType.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    ecosystem type
 * ----------------------------
 */
#include "EcoSysType.h"

//------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "random.h"
#include "srcs_engine.h"


namespace{//-------- namespace: --------------//

    std::uniform_int_distribution<int> uDistribution(   EcoSysType_MinIdx,
                                                        EcoSysType_MaxIdx); 

}//------------- namespace: end --------------//


/* ===========================================================
 *                  get_rand_EcoSysType    tmp
 * -----------------------------------------------------------
 * -- 极其临时性的实现。 随机分配一个 EcoSysType
 */
EcoSysType get_rand_EcoSysType(){

    //-- 一个临时性的方法 ------
    int val = uDistribution( esrc::gameSeed.randEngine );
    switch (val){
        case 1: return EcoSysType::Forest;
        case 2: return EcoSysType::DarkForest;
        case 3: return EcoSysType::Plain;
        case 4: return EcoSysType::Swamp;
        case 5: return EcoSysType::Desert;
        default:
            assert(0);
            return EcoSysType::Forest; //- never touch
    }
}




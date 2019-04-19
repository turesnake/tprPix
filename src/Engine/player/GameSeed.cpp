/*
 * ====================== GameSeed.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   game seed for random
 * ----------------------------
 */
#include "GameSeed.h"

#include "debug.h"


namespace{//----------- namespace ----------------//

    inline std::uniform_int_distribution<int>   uDistribution_altiSeed(-1000, 1000);
    inline std::uniform_int_distribution<float> uDistribution_float(-1000.0, 1000.0);


}//-------------- namespace : end ----------------//


/* ===========================================================
 *                        init      [tmp]
 * -----------------------------------------------------------
 * 在完成版中，gameSeed 依赖游戏存档
 */
void GameSeed::init(){
    //-- 只可被初始化一次... --
    assert( this->is_all_seed_init == false );

    this->randEngine.seed( get_new_seed() );

    //------------------------//
    //   初级版本，所有的 seed 都随机分配
    //   最终，所有 子seed 应该关联于 baseSeed
    //------------------------//
    this->baseSeed = get_new_seed();
   

    //...

    //-------- altiSeed --------//
    this->init_glm_vec2s();

    //...

    //------------------------//
    //       randEngine
    //------------------------//
    this->randEngine.seed( get_new_seed() );


    this->is_all_seed_init = true;
}



/* ===========================================================
 *                  init_glm_vec2s
 * -----------------------------------------------------------
 */
void GameSeed::init_glm_vec2s(){

    //------ altiSeeds --------//
    this->altiSeed_pposOffSeaLvl = glm::vec2(static_cast<float>(uDistribution_altiSeed(this->randEngine)),
                                            static_cast<float>(uDistribution_altiSeed(this->randEngine)) );
    this->altiSeed_pposOffBig = glm::vec2(  static_cast<float>(uDistribution_altiSeed(this->randEngine)),
                                            static_cast<float>(uDistribution_altiSeed(this->randEngine)) );
    this->altiSeed_pposOffMid = glm::vec2(  static_cast<float>(uDistribution_altiSeed(this->randEngine)),
                                            static_cast<float>(uDistribution_altiSeed(this->randEngine)) );
    this->altiSeed_pposOffSml = glm::vec2(  static_cast<float>(uDistribution_altiSeed(this->randEngine)),
                                            static_cast<float>(uDistribution_altiSeed(this->randEngine)) );

    //------ densitySeed --------//
    this->densitySeed_pposOff = glm::vec2(  static_cast<float>(uDistribution_altiSeed(this->randEngine)),
                                            static_cast<float>(uDistribution_altiSeed(this->randEngine)) );
    //------ field --------//
    this->field_pposOff =   glm::vec2(  static_cast<float>(uDistribution_float(this->randEngine)),
                                        static_cast<float>(uDistribution_float(this->randEngine)) );
    //------ ecoSysInMap --------//
    this->ecoSysInMapWeight_pposOff =   glm::vec2(  static_cast<float>(uDistribution_float(this->randEngine)),
                                                    static_cast<float>(uDistribution_float(this->randEngine)) );
}





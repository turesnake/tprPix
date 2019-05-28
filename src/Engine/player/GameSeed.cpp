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

#include "tprDebug.h"


namespace{//----------- namespace ----------------//

    inline std::uniform_int_distribution<int>   uDistribution_altiSeed(-1000, 1000);
    inline std::uniform_real_distribution<float> uDistribution_float(-1000.0, 1000.0);


}//-------------- namespace : end ----------------//


/* ===========================================================
 *                        init      [tmp]
 * -----------------------------------------------------------
 * 在完成版中，gameSeed 依赖游戏存档
 */
void GameSeed::init(  u32_t _baseSeed ){
    //-- 只可被初始化一次... --
    //   不一定... 在未来会被 取消...
    assert( this->is_all_seed_init == false );

    this->baseSeed = _baseSeed;

    this->randEngine.seed( this->baseSeed + 431 );


    //-------- altiSeed --------//
    this->init_glm_vec2s();

    //...

    //------------------------//
    //       realRandEngine
    //------------------------//
    this->realRandEngine.seed( get_new_seed() );


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
    //------ ecoObj --------//
    this->ecoObjWeight_pposOff =   glm::vec2(  static_cast<float>(uDistribution_float(this->randEngine)),
                                                static_cast<float>(uDistribution_float(this->randEngine)) );
}





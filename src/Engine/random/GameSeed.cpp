/*
 * ====================== GameSeed.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   game seed for random
 * ----------------------------
 */
#include "pch.h"
#include "GameSeed.h"
#include "tprAssert.h"


namespace gameSeed_inn {//----------- namespace: gameSeed_inn ----------------//

    std::uniform_int_distribution<int>    uDistribution_altiSeed(-1000, 1000);
    std::uniform_real_distribution<double> uDistribution_double(-1000.0, 1000.0);

}//-------------- namespace: gameSeed_inn end ----------------//


/* ===========================================================
 *                        init      [tmp]
 * -----------------------------------------------------------
 * 在完成版中，gameSeed 依赖游戏存档
 */
void GameSeed::init(  uint32_t baseSeed_ ){
    //-- 只可被初始化一次... --
    //   不一定... 在未来会被 取消...
    tprAssert( this->is_all_seed_init == false );

    this->baseSeed = baseSeed_;

    this->randEngine.seed( static_cast<uint_fast32_t>(this->baseSeed + 431) );


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
    this->altiSeed_pposOffSeaLvl = glm::dvec2(static_cast<double>(gameSeed_inn::uDistribution_altiSeed(this->randEngine)),
                                            static_cast<double>(gameSeed_inn::uDistribution_altiSeed(this->randEngine)) );
    this->altiSeed_pposOffBig = glm::dvec2(  static_cast<double>(gameSeed_inn::uDistribution_altiSeed(this->randEngine)),
                                            static_cast<double>(gameSeed_inn::uDistribution_altiSeed(this->randEngine)) );
    this->altiSeed_pposOffMid = glm::dvec2(  static_cast<double>(gameSeed_inn::uDistribution_altiSeed(this->randEngine)),
                                            static_cast<double>(gameSeed_inn::uDistribution_altiSeed(this->randEngine)) );
    this->altiSeed_pposOffSml = glm::dvec2(  static_cast<double>(gameSeed_inn::uDistribution_altiSeed(this->randEngine)),
                                            static_cast<double>(gameSeed_inn::uDistribution_altiSeed(this->randEngine)) );

                    //-- 未来需要添加一道限制，
                    // 确保 玩家一定出生在陆地上，如果没有，则重新生成。
                    // ...



    //------ densitySeed --------//
    this->densitySeed_pposOff = glm::dvec2(  static_cast<double>(gameSeed_inn::uDistribution_altiSeed(this->randEngine)),
                                            static_cast<double>(gameSeed_inn::uDistribution_altiSeed(this->randEngine)) );
    //------ field --------//
    this->field_dposOff =   glm::dvec2(  gameSeed_inn::uDistribution_double(this->randEngine),
                                        gameSeed_inn::uDistribution_double(this->randEngine) );
    //------ chunk --------//
    this->chunk_dposOff =   glm::dvec2(  gameSeed_inn::uDistribution_double(this->randEngine),
                                        gameSeed_inn::uDistribution_double(this->randEngine) );
    //------ ecoObj --------//
    this->ecoObjWeight_dposOff =   glm::dvec2(  gameSeed_inn::uDistribution_double(this->randEngine),
                                                gameSeed_inn::uDistribution_double(this->randEngine) );
}


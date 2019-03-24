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


/* ===========================================================
 *                        init 
 * -----------------------------------------------------------
 */
void GameSeed::init(){
    //-- 只可被初始化一次... --
    assert( this->is_all_seed_init == false );

    //------------------------//
    //   初级版本，所有的 seed 都随机分配
    //   最终，所有 子seed 应该关联于 baseSeed
    //------------------------//
    this->baseSeed = get_new_seed();
   
    this->perlinSeed_ecoSysInMap = get_new_seed();
    this->perlinSeed_fields_in_ecoSysInMap = get_new_seed();
    this->perlinSeed_pixes_in_field = get_new_seed();
    this->perlinSeed_field = get_new_seed();
    this->perlinSeed_alti = get_new_seed();
    this->perlinSeed_density = get_new_seed();

    //...


    //------------------------//
    //       perlins
    //------------------------//
    this->pn_field_in_ecoSysInMap.init( this->perlinSeed_fields_in_ecoSysInMap );
    this->pn_pixes_in_field.init( this->perlinSeed_pixes_in_field );
    this->pn_ecoSysInMap.init( this->perlinSeed_ecoSysInMap );
    this->pn_field.init( this->perlinSeed_field );
    this->pn_alti.init( this->perlinSeed_alti );
    this->pn_density.init( this->perlinSeed_density );


    //------------------------//
    //       randEngine
    //------------------------//
    //this->randEngine.seed( get_new_seed() );


    this->is_all_seed_init = true;
}



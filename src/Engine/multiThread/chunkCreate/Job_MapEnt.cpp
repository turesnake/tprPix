/*
 * ========================== Job_MapEnt.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Job_MapEnt.h"

//-------------------- Engine --------------------//
#include "MapEnt.h"
#include "random.h"



extern double calc_mapEntAlti( IntVec2 mpos_ )noexcept;


void Job_MapEnt::init( IntVec2 mpos_ )noexcept{
    this->mpos = { mpos_ };
    this->alti = { MapAltitude{ calc_mapEntAlti(mpos_) } };
    this->bioSoupState = { gameObjs::bioSoup::calc_bioSoupState_by_mapAlti(this->get_alti()) };
    this->chunkKey = { anyMPos_2_chunkKey( this->get_mpos() ) };
    this->uWeight = { calc_simple_mapent_uWeight( this->get_mpos() ) };
}


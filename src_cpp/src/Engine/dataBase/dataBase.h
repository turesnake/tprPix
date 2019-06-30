/*
 * ========================= dataBase.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_DATA_BASE_H
#define TPR_DATA_BASE_H

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "DiskGameObj.h"
#include "gameArchiveId.h"


class GameArchive;

namespace db{//---------------- namespace: db ----------------------//


void atom_init_dataBase();
void atom_close_dataBase();

//-- table_gameArchive --
void atom_select_all_from_table_gameArchive( std::unordered_map<gameArchiveId_t, GameArchive> &container_ );
void atom_insert_or_replace_to_table_gameArchive( const GameArchive &archive_ );

void atom_writeBack_to_table_gameArchive();

//-- table_goes --
void atom_select_one_from_table_goes( goid_t goid_, DiskGameObj &diskGo_ );
void atom_insert_or_replace_to_table_goes( const DiskGameObj &diskGo_ );


}//----------------------- namespace: db end ----------------------//
#endif 


/*
 * ========================= dataBase.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_DATA_BASE_H_
#define _TPR_DATA_BASE_H_

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
void atom_select_all_from_table_gameArchive( std::unordered_map<gameArchiveId_t, GameArchive> &_container );
void atom_insert_or_replace_to_table_gameArchive( const GameArchive &_archive );

void atom_writeBack_to_table_gameArchive();

//-- table_goes --
void atom_select_one_from_table_goes( goid_t _goid, DiskGameObj &_diskGo );
void atom_insert_or_replace_to_table_goes( const DiskGameObj &_diskGo );


}//----------------------- namespace: db end ----------------------//
#endif 


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
#include "gameArchiveId.h"


class GameArchive;

namespace db{//---------------- namespace: db ----------------------//


void init_dataBase();
void close_dataBase();

void insert_or_replace_to_table_gameArchive( const GameArchive &_archive );
void select_all_from_table_gameArchive( std::unordered_map<gameArchiveId_t, GameArchive> &_container );


}//----------------------- namespace: db end ----------------------//
#endif 


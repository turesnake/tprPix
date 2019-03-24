/*
 * ==================== FieldBorderSet.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_FIELD_BORDER_SET_H_
#define _TPR_FIELD_BORDER_SET_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "Quad.h"
#include "chunkKey.h"
#include "ID_Manager.h" 
#include "fieldBorderSetId_t.h"



class FieldBorderSet{
public:
    using quadContainer_t = std::vector<u8_t>; //- 扇区容器type

    fieldBorderSetId_t  id {0};

    //-- 将预制件数据 一分为四 [left-top] --
    quadContainer_t leftBottoms {};
    quadContainer_t rightBottoms {};
    quadContainer_t leftTops {};
    quadContainer_t rightTops {};

    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 id 

};
//======== static ========//
inline ID_Manager  FieldBorderSet::id_manager { ID_TYPE::U32, 1};


void clear_for_fieldBorderSet();
void copy_originData_for_fieldBorderSet( const FieldBorderSet::quadContainer_t &_datas  );
void build_all_mutant_datas_for_fieldBorderSet();


fieldBorderSetId_t apply_a_fieldBorderSetId( size_t _randIdx );
const FieldBorderSet::quadContainer_t &get_fieldBorderSet( fieldBorderSetId_t _id, QuadType _quad );



#endif 


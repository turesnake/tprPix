/*
 * =================== SectionFieldSet.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section Field集合 
 * ----------------------------
 */
#ifndef _TPR_SECTION_FIELD_SET_H
#define _TPR_SECTION_FIELD_SET_H


//-------------------- CPP --------------------//
#include <vector>


//-------------------- Engine --------------------//
#include "config.h"
#include "SectionKey.h"
#include "MapCoord.h"
#include "MapField.h"


//-- 一个 section 拥有的 64*64 个 Field 的集合 --
class SectionFieldSet{
public:

    void init( const IntVec2 &_mpos );

    MapField *get_mapFieldPtr_by_mpos( const IntVec2 &_mpos );

    //====== vals =======//
    std::vector<MapField>  fields {}; //- 64*64 个

    //-- 下面两个数据 和 Section 彻底重复了... --
    //  未来考虑 整合之 ...
    MapCoord      mcpos      {}; //- section.mcpos
    SectionKey    sectionKey {}; //- 存储了相邻8个section 的key数据
                        
    
    //====== static =======//
    static  int   fieldSideLen; //- 64*64 个 field (只记录边长)
};
//====== static =======//
inline int SectionFieldSet::fieldSideLen { SECTION_SIDE_ENTS/MapField::entSideLen }; //- 64


#endif 


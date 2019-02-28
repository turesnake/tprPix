/*
 * ============== SectionFieldSetInBuild.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section Field集合 
 * ----------------------------
 */
#ifndef _TPR_SECTION_FIELD_SET_IN_BUILD_H
#define _TPR_SECTION_FIELD_SET_IN_BUILD_H


//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "MapFieldInBuild.h"
#include "SectionFieldSet.h"

namespace sectionBuild { //------- namespace: sectionBuild ----------//


//-- 一个 section 拥有的 64*64 个 Field 的集合 --
//  本 class 仅有一个实例，存储在 sectionBuild.h 中
//  每一个正在被创建的 section，都临时使用这个 实例
class SectionFieldSetInBuild{
public:

    void init( SectionFieldSet *_sectionFieldSetPtr );

    //====== vals =======//
    std::vector<MapFieldInBuild>  fieldsInBuild {}; //- 64*64 个
                //- 建造 section 需要的 临时性 Field数据 
                //  本clss 存在的根本意义。

    SectionFieldSet  *sectionFieldSetPtr {nullptr};
                //- 指向 当前正在build 的 section 的 数据
    
};


}//----------------- namespace: sectionBuild: end -------------------//
#endif 


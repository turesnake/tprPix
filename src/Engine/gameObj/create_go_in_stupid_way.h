/*
 * ================= create_go_in_stupid_way.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 *  临时版，使用类似 蓝图的流程，在主线程中，手动创建一个 go
 * 
 * -----
 */
#ifndef TPR_CREATE_GO_IN_STUPID_WAY_H
#define TPR_CREATE_GO_IN_STUPID_WAY_H

//------------------- Engine --------------------//
#include "GoDataForCreate.h"
#include "GoSpecFromJson.h"
#include "DyParam.h"
#include "create_goes.h"


goid_t create_go_in_stupid_way( goSpeciesId_t       goSpeciesId_,
                                goLabelId_t         goLabelId_,
                                const glm::dvec2    &dpos_,
                                NineDirection       direction_,
                                BrokenLvl           brokenLvl_,
                                size_t              mapEntUWeight_ );


#endif



/*
 * ================= create_go_by_hand.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  临时版，使用类似 蓝图的流程，在主线程中，手动创建一个 go
 */
#ifndef TPR_CREATE_GO_BY_HAND_H
#define TPR_CREATE_GO_BY_HAND_H


//------------------- Engine --------------------//
#include "GoDataForCreate.h"
#include "GoSpecFromJson.h"
#include "DyParam.h"
#include "create_goes.h"
#include "DiskGameObj.h"

goid_t create_go_by_hand(       goSpeciesId_t       goSpeciesId_,
                                goLabelId_t         goLabelId_,
                                IntVec2             mpos_,
                                const glm::dvec2    &dpos_,
                                NineDirection       direction_,
                                BrokenLvl           brokenLvl_
                                );


goid_t create_go_from_goDataForCreate( const GoDataForCreate *goDPtr_ );


void rebind_diskGo_by_hand(   const DiskGameObj &diskGo_ );


#endif



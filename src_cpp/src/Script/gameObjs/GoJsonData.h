/*
 * ======================= GoJsonData.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.07.02
 *                                        修改 -- 
 * ----------------------------------------------------------
 * 从 json文件中读取的 有关 go 的固定数据
 */
#ifndef TPR_GO_JSON_DATA_H
#define TPR_GO_JSON_DATA_H


//-------------------- Engine --------------------//
#include "GameObj.h"

class GoJsonData{
public:

    std::string       gameObjType {};
    goSpecId_t        specID {};
    goid_t            parentID {NULLID};

    //----- enum -----//
    GameObjFamily     family {};
    GameObjState      state  {};
    GameObjMoveState  moveState {};
    MoveType          moveType {};

    //----- bool -----//
    bool    isTopGo  {};
    bool    isActive {};
    bool    isDoPass {};
    bool    isBePass {};
    //bool    isDirty  {};
    //bool    isControlByPlayer {};

    //----- numbers -----//
    SpeedLevel  speedLvl {};
    float       alti   {};
    float       weight {};
    //...
};


void parse_from_goJsonFile();
void assemble_goJsonData_2_newGo( goSpecId_t specID_, 
                                GameObj &goRef_ );


#endif 



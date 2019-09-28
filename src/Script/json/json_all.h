/*
 * ======================== json_all.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.09.29
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JSON_ALL_H
#define TPR_JSON_ALL_H

//-------------------- Engine --------------------//
#include "GameObj.h"

//------------------------------//
void parse_from_animFrameSetJsonFile();
void parse_from_colorTableJsonFile();
void parse_from_ecoSysPlansJsonFile();

//------------------------------//
//    WindowConfigJsonData
//------------------------------//
class WindowConfigJsonData{
public:
    int windowPixW {};
    int windowPixH {};
    bool isFullScreen {};
    //...
};

WindowConfigJsonData parse_from_windowConfigJsonFile();


//------------------------------//
//        GoJsonData
//------------------------------//
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
    bool    isMoveCollide {};
    bool    isDoPass {};
    bool    isBePass {};
    //bool    isActive {}; //- 是否进入激活圈
    //bool    isDirty  {};
    //bool    isControlByPlayer {};

    //----- numbers -----//
    SpeedLevel  speedLvl {};
    double       alti   {};
    double       weight {};
    //...

    //std::unordered_map<PubBinaryValType,int> pubVals {};
    PubBinary2   pubBinary {};
};

void parse_from_goJsonFile();
void assemble_goJsonData_2_newGo( goSpecId_t specID_, 
                                GameObj &goRef_ );

//------------------------------//
//        UIGoJsonData
//------------------------------//
class UIGoJsonData{
public:

    std::string       gameObjType {};
    goSpecId_t        specID {};
    goid_t            parentID {NULLID};

    //----- enum -----//
    GameObjFamily     family {};

    //----- bool -----//
    //----- numbers -----//
    MoveType    moveType {};
    SpeedLevel  speedLvl {};
    //...


};

void parse_from_uiGoJsonFile();
void assemble_uiGoJsonData_2_newUIGo( goSpecId_t specID_, 
                                    GameObj &goRef_ );


#endif 


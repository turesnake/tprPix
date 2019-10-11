/*
 * ====================== GameObjSpec.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GAME_OBJ_SPEC_H
#define TPR_GAME_OBJ_SPEC_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <memory>


//------------------- Libs --------------------//
#include "tprDataType.h" 


//-------------------- Engine --------------------//
#include "GameObjType.h" 
#include "Move.h"
#include "PubBinary2.h"


//--------------- Script ------------------//
#include "Script/json/json_multiGoMesh.h"


class GameObjSpec{
public:

    std::string       goSpecName {};
    goSpecId_t        specID {};
    goid_t            parentID {};


    //----- enum -----//
    GameObjFamily       family {};
    GameObjState        state  {};
    GameObjMoveState    moveState {};
    MoveType            moveType {};

    //----- bool -----//
    bool    isTopGo  {};
    bool    isMoveCollide {};
    bool    isDoPass {};
    bool    isBePass {};

    //----- numbers -----//
    SpeedLevel  speedLvl {};
    double       alti   {};
    double       weight {};
    //...

    PubBinary2   pubBinary {};

    //----- afs / gomeshs -----//
    std::string       animFrameSetName {}; // only used for single-gomesh-go
    std::unique_ptr<json::Json_MultiGoMesh> multiGoMeshUPtr {nullptr};

};





#endif 


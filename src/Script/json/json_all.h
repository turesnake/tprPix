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


namespace json{//------------- namespace json ----------------

//------------------------------//
void parse_from_animFrameSetJsonFile();
void parse_from_colorTableJsonFile();
void parse_from_ecoSysPlansJsonFile();
void parse_from_mapSurfaceRandMeshJsonFile();
void parse_from_fieldDistributePlanJsonFile();
void parse_from_multiGoMeshJsonFile();

//------------------------------//
//    json_windowConfig
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
//        json_gameObj
//------------------------------//
void parse_from_goJsonFile();
void assemble_goSpec_2_newGo( goSpecId_t specID_, 
                                GameObj &goRef_ );

//------------------------------//
//        json_UIGo
//------------------------------//
class UIGoJsonData{
public:

    std::string       goSpecName {};
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
void assemble_jsonData_2_newUIGo( goSpecId_t specID_, 
                                    GameObj &goRef_ );



//------------------------------//
//        json_GoMesh
//------------------------------//
/*
class GoMeshJsonData{
public:

};
*/










}//------------- namespace json: end ----------------
#endif 


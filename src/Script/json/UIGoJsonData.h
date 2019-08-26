/*
 * ======================= UIGoJsonData.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.08.25
 *                                        修改 -- 
 * ----------------------------------------------------------
 * 从 json文件中读取的 有关 uiGo 的固定数据
 */
#ifndef TPR_UI_GO_JSON_DATA_H
#define TPR_UI_GO_JSON_DATA_H


//-------------------- Engine --------------------//
#include "GameObj.h"

class UIGoJsonData{
public:

    std::string       gameObjType {};
    goSpecId_t        specID {};
    goid_t            parentID {NULLID};

    //----- enum -----//
    GameObjFamily     family {};

    //----- bool -----//
    //----- numbers -----//
    SpeedLevel  speedLvl {};
    //...


};


void parse_from_uiGoJsonFile();
void assemble_uiGoJsonData_2_newUIGo( goSpecId_t specID_, 
                                    GameObj &goRef_ );


#endif 



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
void parse_colorTableJsonFile();
void parse_ecoSysPlansJsonFile();

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

WindowConfigJsonData parse_windowConfigJsonFile();


}//------------- namespace json: end ----------------
#endif 


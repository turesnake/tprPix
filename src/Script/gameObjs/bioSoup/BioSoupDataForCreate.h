/*
 * ================== BioSoupDataForCreate.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_BIO_SOUP_DATA_FOR_CREATE_H
#define TPR_BIO_SOUP_DATA_FOR_CREATE_H


//-------------------- Engine --------------------//
#include "MapAltitude.h"

//-------------------- Script --------------------//
#include "Script/gameObjs/bioSoup/BioSoupState.h"


namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------


// used in goDataForCreate
class DataForCreate{
public:
    DataForCreate()=default;
    //----- vals -----//
    State           bioSoupState {};
    MapAltitude     mapEntAlti  {};
};


}//------------- namespace gameObjs::bioSoup: end ----------------
#endif 


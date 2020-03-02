/*
 * ======================== BioSoup.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.18
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_BIO_SOUP_H
#define TPR_GO_BIO_SOUP_H

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "DyParam.h"


namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------

class BioSoup{
public:
    static void init(GameObj &goRef_, const DyParam &dyParams_ );
private:                            
    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace gameObjs::bioSoup: end ----------------
#endif 



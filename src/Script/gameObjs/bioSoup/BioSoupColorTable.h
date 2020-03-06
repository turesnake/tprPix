/*
 * ==================== BioSoupColorTable.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_BIO_SOUP_COLOR_TABLE_H
#define TPR_GO_BIO_SOUP_COLOR_TABLE_H


//-------------------- Engine --------------------//
#include "FloatVec.h"

namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------



struct BioSoupColorTable{
    //-- base
    FloatVec4 base_light;
    FloatVec4 base_mid;
    FloatVec4 base_dark;
    //-- particle
    FloatVec4 particle_light;
    FloatVec4 particle_mid;
    FloatVec4 particle_dark;
};


void write_ubo_BioSoupColorTable();

}//------------- namespace gameObjs::bioSoup: end ----------------
#endif 


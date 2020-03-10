/*
 * ======================= UBO_Time.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "ubo_all.h"

//-------------------- Engine --------------------//
#include "esrc_uniformBlockObj.h"
#include "esrc_time.h"

namespace ubo{//------------- namespace ubo ----------------

// called every render frmae
void write_ubo_Time(){

    UBO_Time data {};
    data.currentTime = static_cast<float>( esrc::get_timer().get_currentTime() );
    //---
    auto &uboRef = esrc::get_uniformBlockObjRef( UBOType::Time );
    uboRef.write(  0, 
                sizeof(UBO_Time),
                static_cast<const GLvoid*>( &data ) );
}

}//------------- namespace ubo: end ----------------

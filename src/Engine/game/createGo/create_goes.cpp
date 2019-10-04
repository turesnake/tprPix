/*
 * ========================= create_goes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "create_goes.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_gameObj.h" 


//--------------- Script ------------------//
#include "Script/gameObjs/allGoes.h"
#include "Script/resource/ssrc.h"

#include "Script/json/json_all.h"

#include "tprDebug.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


/* ===========================================================
 *                  create_a_Go       [tmp]
 * -----------------------------------------------------------
 * -- 仅适用于 部分类型的 go
 * -- 对于很多类似 ui的，不依赖map，且更为轻量的go，应为其专设 create 函数 
 */
goid_t create_a_Go( goSpecId_t goSpecId_,
                    const glm::dvec2 &dpos_,
                    const ParamBinary &dyParams_ ){

    goid_t goid = esrc::insert_new_regularGo( dpos_ );
    GameObj &goRef = esrc::get_goRef( goid );

    //-- set some static datas from JSON --
        tprAssert( ssrc::find_from_goInit_funcs(goSpecId_) );
    json::assemble_goJsonData_2_newGo( goSpecId_, goRef );

    //-- check GameObjFamily --
        tprAssert( goRef.family != GameObjFamily::UI );

    ssrc::call_goInit_func( goSpecId_, goRef, dyParams_ );

    goRef.init_check();
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goRef );
    esrc::insert_2_goids_inactive( goid );
            //- 放入 未激活队列会造成 5帧的 显示空缺
            //- 更为完善的做法是，当场检测应该放入 激活队列还是 未激活队列...
            //  未来被 GoMemState 系统取代
            //  ...
    
    return  goid;
}


/* ===========================================================
 *                  rebind_a_disk_Go       [tmp]
 * -----------------------------------------------------------
 * 从 db读取一个 go 的数据，并用此数据，重建一个 mem态 go实例
 */
void rebind_a_disk_Go(  const DiskGameObj &diskGo_,
                        const glm::dvec2 &dpos_,
                        const ParamBinary &dyParams_  ){

    esrc::insert_a_diskGo( diskGo_.goid, dpos_ );
    GameObj &goRef = esrc::get_goRef( diskGo_.goid );

        tprAssert( ssrc::find_from_goInit_funcs(diskGo_.goSpecId) );

    //-- set some static datas from JSON --
    json::assemble_goJsonData_2_newGo( diskGo_.goSpecId, goRef ); //- tmp
                    //-- 临时措施
                    //   在未来，已经组装 从 数据库取出的数据，而不是从 json 中
                    //   至少有一部分吧
                    //   ...

    ssrc::call_goInit_func( diskGo_.goSpecId, goRef, dyParams_ );
            //-- 临时方案，最好使用 具象go类 rebind 系列函数 

    goRef.init_check();
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goRef );
    esrc::insert_2_goids_inactive( diskGo_.goid );
            //- 放入 未激活队列会造成 5帧的 显示空缺
            //- 更为完善的做法是，当场检测应该放入 激活队列还是 未激活队列...
            //  未来被 GoMemState 系统取代
            //  ...
}


}//------------- namespace gameObjs: end ----------------



namespace uiGos{//------------- namespace uiGos ----------------



/* ===========================================================
 *                  create_a_UIGo
 * -----------------------------------------------------------
 */
goid_t create_a_UIGo( goSpecId_t goSpecId_,
                    const glm::dvec2 &basePointProportion_, 
                    const glm::dvec2 &offDPos_,
                    const ParamBinary &dyParams_ ){

    goid_t goid = esrc::insert_new_uiGo( basePointProportion_, offDPos_ );
    GameObj &goRef = esrc::get_goRef( goid );
    
    //-- set some static datas from JSON --
        tprAssert( ssrc::find_from_uiGoInit_funcs(goSpecId_) );
    json::assemble_uiGoJsonData_2_newUIGo( goSpecId_, goRef );

    //-- check GameObjFamily --
    tprAssert( goRef.family == GameObjFamily::UI );

    ssrc::call_uiGoInit_func( goSpecId_, goRef, dyParams_ );

    goRef.init_check();
    //------------------------------//
    //  uiGo 不用登记到 map 中，目前来看，是被一个 生命周期稳定的 scene 手动管理
    //  ui 也不存在什么 active 状态
    return  goid;
}


goid_t create_a_UIGo( goSpecId_t goSpecId_,
                    const UIAnchor &uiAnchor_,
                    const ParamBinary &dyParams_ ){

    goid_t goid = esrc::insert_new_uiGo(uiAnchor_.get_basePointProportion(), 
                                        uiAnchor_.get_offDPos() );
    GameObj &goRef = esrc::get_goRef( goid );
    
    //-- set some static datas from JSON --
        tprAssert( ssrc::find_from_uiGoInit_funcs(goSpecId_) );
    json::assemble_uiGoJsonData_2_newUIGo( goSpecId_, goRef );

    //-- check GameObjFamily --
    tprAssert( goRef.family == GameObjFamily::UI );

    ssrc::call_uiGoInit_func( goSpecId_, goRef, dyParams_ );
    
    goRef.init_check();
    //------------------------------//
    //  uiGo 不用登记到 map 中，目前来看，是被一个 生命周期稳定的 scene 手动管理
    //  ui 也不存在什么 active 状态
    return  goid;
}


}//------------- namespace uiGos: end ----------------




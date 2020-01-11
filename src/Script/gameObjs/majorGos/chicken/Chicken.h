/*
 * ========================= Chicken.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#ifndef TPR_GO_CHICKEN_H
#define TPR_GO_CHICKEN_H

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "DyParam.h"

namespace gameObjs{//------------- namespace gameObjs ----------------


class Chicken{
public:
    //--- 延迟init ---//
    static void init(GameObj &goRef_, const DyParam &dyParams_ );
private:                            
    static void bind( GameObj &goRef_ );
    static void rebind( GameObj &goRef_ );

    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnLogicUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );

    //--- oth ---//
    static void moveState_manage( GameObj &goRef_,
                                GameObjMesh &goMeshRef_
                                )noexcept;



    static void move_AnimAction_switch( GameObj &goRef_ );



};


}//------------- namespace gameObjs: end ----------------
#endif 


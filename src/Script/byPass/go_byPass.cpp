

#include "Script/gameObjs/Dog_A.h" 

//-------------------- Engine --------------------//
#include "srcs_engine.h" 
#include "GameObj.h" 


/* ===========================================================
 *                       go_byPass
 * -----------------------------------------------------------
 * -- 跳过 section，硬生成一个／组 go实例（Dog_A）
 */
void go_byPass(){

    GameObj go;

        //go

        gameObjs::dog_a.init( &go );





    esrc::insert_new_gameObj( go );
    




}


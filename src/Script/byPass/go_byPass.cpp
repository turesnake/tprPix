

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

    // ***| INSERT FIRST, INIT LATER  |***
    GameObj tmp_go;
    goid_t goid = esrc::insert_new_gameObj( tmp_go );
    GameObj *gop = &(esrc::memGameObjs.at(goid)); //- 获得目标go指针
    gameObjs::dog_a.init( gop );
    gop->currentPos = glm::vec2{ 10.0f, 0.0f };
    
    //-- 再创建一个 --
    goid = esrc::insert_new_gameObj( tmp_go );
    gop = &(esrc::memGameObjs.at(goid)); //- 获得目标go指针
    gameObjs::dog_a.init( gop );
    gop->currentPos = glm::vec2{ 0.0f, 10.0f };
    

    

}


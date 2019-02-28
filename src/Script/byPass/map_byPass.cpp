
//-------------------- Engine --------------------//
#include "Chunk.h"
#include "srcs_engine.h"


/* ===========================================================
 *                       map_byPass
 * -----------------------------------------------------------
 * -- 临时性的 CREATE一个 section 实例。
 */
void map_byPass(){

    //--- 最简模式，仅仅生成 玩家所在的 section
    IntVec2 entMPos { 0,0 };
    build_first_section( entMPos );
    build_nearby_sections( entMPos );
}








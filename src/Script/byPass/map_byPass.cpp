
//-------------------- Engine --------------------//
#include "Chunk.h"
#include "sectionBuild.h"


/* ===========================================================
 *                       map_byPass
 * -----------------------------------------------------------
 * -- 临时性的 CREATE一个 section 实例。
 */
void map_byPass(){

    //--- 最简模式，仅仅生成 玩家所在的 section
    sectionBuild::build_9_chunks_3( IntVec2{ 0,0 } );
}








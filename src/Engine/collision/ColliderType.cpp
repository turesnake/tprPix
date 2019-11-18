/*
 * ====================== ColliderType.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.15
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "ColliderType.h"

//-------------------- Engine --------------------//
#include "config.h"


// 这里的 半径，是 square 在 artifactCoord 坐标系内的 半径值
// 所有 afs / gos 都要和这个值统一
// 暂时只有一个值
// 

//double Square::unifiedRadius { static_cast<double>(HALF_PIXES_PER_MAPENT) }; // 32.0


double Square::unifiedRadius { static_cast<double>(HALF_PIXES_PER_MAPENT) * 2.0 }; // 32.0


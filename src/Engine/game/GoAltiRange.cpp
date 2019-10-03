/*
 * ====================== GoAltiRange.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GoAltiRange.h"

//===== static =====//
char GoAltiRange::jumpLimit = 45;
u8_t GoAltiRange::diskAlti_item    = 254;
u8_t GoAltiRange::diskAlti_surface = 253;
            //- 原本是 255/254; 但这样就会与 AnimFrameSet 中的 rootAnchor 信息冲突
            //- 故暂时下调1，改为 254/253
            //  在未来。随着 碰撞系统的不断完善。 surface_go/item_go 这组设计很可能会被取代...


//-- item / surface --//
const GoAltiRange goAltiRange_item    { static_cast<char>(GoAltiRange::diskAlti_item), 
                                        static_cast<char>(GoAltiRange::diskAlti_item) };
const GoAltiRange goAltiRange_surface { static_cast<char>(GoAltiRange::diskAlti_surface), 
                                        static_cast<char>(GoAltiRange::diskAlti_surface) };



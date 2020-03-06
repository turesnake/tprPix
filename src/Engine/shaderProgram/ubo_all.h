/*
 * ======================= ubo_all.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_UBO_ALL_H
#define TPR_UBO_ALL_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- Engine --------------------//
#include "FloatVec.h"
#include "colorTableId.h"
//...

namespace ubo{//------------- namespace ubo ----------------

//-------------------------//
//      UBO_Camera
//-------------------------//
// struct:
// glm::mat4  mat4_view        (64)
// glm::mat4  mat4_projection  (64)
// FloatVec2  canvasCFPos      (8)    
// ...
inline constexpr size_t UBO_Camera_size { 64 + 64 + 8 };

// called when:
// -1- app.init
// -2- camera state changed
void write_ubo_Camera();


//-------------------------//
//       UBO_Seeds
//-------------------------//
// [once]
struct UBO_Seeds{
    FloatVec2 altiSeed_pposOffSeaLvl {};
    FloatVec2 altiSeed_pposOffBig {};
    FloatVec2 altiSeed_pposOffMid {};
    FloatVec2 altiSeed_pposOffSml {};
    //....
};

void write_ubo_Seeds();

//-------------------------//
//       UBO_Time
//-------------------------//
// [tmp once]
struct UBO_Time{
    float currentTime {};
    //....
};

// called every render frmae
void write_ubo_Time();


//-------------------------//
//        UBO_Window
//-------------------------//
// [tmp once]
struct UBO_Window{
    FloatVec2 gameSZ {}; // equal to ViewingBox::gameSZ
    //....
};

// only called in:
// -1- app.init
// -2- windowSZ changed (never yet...)
void write_ubo_Window();


//-------------------------//
//    UBO_WorldCoord
//-------------------------//
// [once]
struct UBO_WorldCoord{
    FloatVec2   xVec {};
    FloatVec2   yVec {};
    float       denominator {};
    //....
};

// only called in init
void write_ubo_WorldCoord();




//-------------------------//
//  UBO_ColorTable
//-------------------------//
void write_ubo_OriginColorTable();
void update_and_write_ubo_UnifiedColorTable();
void write_ubo_GroundColorTable();
void write_ubo_colorTableId( colorTableId_t id_ );








}//------------- namespace ubo: end ----------------
#endif 


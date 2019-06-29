/*
 * ========================= ViewingBox.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   camera.viewingBox
 * ----------------------------
 */
#ifndef TPR_VIEWING_BOX_H_
#define TPR_VIEWING_BOX_H_

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- C --------------------//
//#include <cassert>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "windowConfig.h"
#include "config.h"
#include "RenderLayerType.h"
#include "IntVec.h"


class ViewingBox{
public: 

    static float get_renderLayerZOff( RenderLayerType type_ ){
        switch(type_){
            case RenderLayerType::Ground:      return ground_zOff;
            case RenderLayerType::Chunks:      return chunks_zOff;
            case RenderLayerType::WaterAnim:   return waterAnim_zOff;
            case RenderLayerType::MapSurfaces: return mapSurfaces_zOff;
            case RenderLayerType::GoShadows:   return goShadows_zOff;
            case RenderLayerType::Debug:       return debug_zOff;
            case RenderLayerType::UIs:         return UIs_zOff;
            case RenderLayerType::MajorGoes: 
                tprAssert(0);
                return 0.0f; //- never reach
            default:
                tprAssert(0);
                return 0.0f; //- never reach
        }
    }

    static void reset( const int windowSZ_x_,
                       const int windowSZ_y_ ){
        windowSZ.x = windowSZ_x_;
        windowSZ.y = windowSZ_y_;
        gameSZ.x = static_cast<float>(windowSZ_x_) / static_cast<float>(PIXES_PER_GAMEPIX);
        gameSZ.y = static_cast<float>(windowSZ_y_) / static_cast<float>(PIXES_PER_GAMEPIX);
    }


    //======= statix =======//
    static IntVec2    windowSZ; //- 屏幕尺寸（像素） （在高分屏上似乎有问题）
    static glm::vec2  gameSZ;   //- 游戏像素尺寸

    static float z;

    //-- distance from zFar(to zNear) --
    //   camera.zFar 是个动态值，此处只能保存一个 相对偏移
    static float  ground_zOff;
        // 整个游戏最 “深” 的图层，a canvas，往往涂上一个单一的底色

    static float  chunks_zOff;
        // 游戏地图 图层。 由无数块 chunk 拼接而成。
        // 其中 water区域是半透明的，可以看到下层的 waterAnimCanvas 效果

    static float  waterAnim_zOff;
        // 水域动画 图层。a canvas，其颜色逐帧生成，并直接渲染

    static float  mapSurfaces_zOff;
        // 地表 图层。 放置 地衣，苔藓 等没有碰撞的 纯装饰性 go
        // 在未来，也许会放到 waterAnim 下方...
        
    static float  goShadows_zOff;
        // go阴影 图层。
        
    static float  debug_zOff;
        // tprDebug 专用 图层
    
    //...


    static float  UIs_zOff;
        // UIs 专用 图层
};


inline IntVec2  ViewingBox::windowSZ {  SCR_WIDTH_, SCR_HEIGHT_ };

inline glm::vec2 ViewingBox::gameSZ {   static_cast<float>(SCR_WIDTH_)  / static_cast<float>(PIXES_PER_GAMEPIX),
                                        static_cast<float>(SCR_HEIGHT_) / static_cast<float>(PIXES_PER_GAMEPIX) };

inline float ViewingBox::z { static_cast<float>(VIEWING_BOX_Z_DEEP) };

inline float  ViewingBox::ground_zOff        { 10.0f };
inline float  ViewingBox::chunks_zOff        { 20.0f };
inline float  ViewingBox::waterAnim_zOff     { 30.0f };
inline float  ViewingBox::mapSurfaces_zOff   { 40.0f };
inline float  ViewingBox::goShadows_zOff     { 50.0f };
inline float  ViewingBox::debug_zOff         { 60.0f };

//...

inline float  ViewingBox::UIs_zOff         { 1900.0f };


#endif


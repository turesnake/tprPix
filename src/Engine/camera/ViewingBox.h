/*
 * ========================= ViewingBox.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   camera.viewingBox
 * ----------------------------
 */
#ifndef TPR_VIEWING_BOX_H
#define TPR_VIEWING_BOX_H

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

    static double get_renderLayerZOff( RenderLayerType type_ ){
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
                return 0.0; //- never reach
            default:
                tprAssert(0);
                return 0.0; //- never reach
        }
    }

    static void reset( const int windowSZ_x_,
                       const int windowSZ_y_ ){
        windowSZ.x = windowSZ_x_;
        windowSZ.y = windowSZ_y_;
        gameSZ.x = static_cast<double>(windowSZ_x_) / static_cast<double>(PIXES_PER_GAMEPIX);
        gameSZ.y = static_cast<double>(windowSZ_y_) / static_cast<double>(PIXES_PER_GAMEPIX);
    }


    //======= statix =======//
    static IntVec2    windowSZ; //- 屏幕尺寸（像素） （在高分屏上似乎有问题）
    static glm::dvec2  gameSZ;   //- 游戏像素尺寸

    static double z;

    //-- distance from zFar(to zNear) --
    //   camera.zFar 是个动态值，此处只能保存一个 相对偏移
    static double  ground_zOff;
        // 整个游戏最 “深” 的图层，a canvas，往往涂上一个单一的底色

    static double  chunks_zOff;
        // 游戏地图 图层。 由无数块 chunk 拼接而成。
        // 其中 water区域是半透明的，可以看到下层的 waterAnimCanvas 效果

    static double  waterAnim_zOff;
        // 水域动画 图层。a canvas，其颜色逐帧生成，并直接渲染

    static double  mapSurfaces_zOff;
        // 地表 图层。 放置 地衣，苔藓 等没有碰撞的 纯装饰性 go
        // 在未来，也许会放到 waterAnim 下方...
        
    static double  goShadows_zOff;
        // go阴影 图层。
        
    static double  debug_zOff;
        // tprDebug 专用 图层
    
    //...


    static double  UIs_zOff;
        // UIs 专用 图层
};


inline IntVec2  ViewingBox::windowSZ {  SCR_WIDTH_, SCR_HEIGHT_ };

inline glm::dvec2 ViewingBox::gameSZ {   static_cast<double>(SCR_WIDTH_)  / static_cast<double>(PIXES_PER_GAMEPIX),
                                        static_cast<double>(SCR_HEIGHT_) / static_cast<double>(PIXES_PER_GAMEPIX) };

inline double ViewingBox::z { static_cast<double>(VIEWING_BOX_Z_DEEP) };

inline double  ViewingBox::ground_zOff        { 10.0 };
inline double  ViewingBox::chunks_zOff        { 20.0 };
inline double  ViewingBox::waterAnim_zOff     { 30.0 };
inline double  ViewingBox::mapSurfaces_zOff   { 40.0 };
inline double  ViewingBox::goShadows_zOff     { 50.0 };
inline double  ViewingBox::debug_zOff         { 60.0 };

//...

inline double  ViewingBox::UIs_zOff         { 1900.0 };


#endif


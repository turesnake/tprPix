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

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "RenderLayerType.h"
#include "IntVec.h"


class ViewingBox{
public: 

    //--- MUST before OpenGL init ---
    static void init();

    static double get_renderLayerZOff( RenderLayerType type_ )noexcept{
        switch(type_){
            case RenderLayerType::Ground:           return ground_zOff;
            case RenderLayerType::GroundGo:         return groundGo_zOff;
            case RenderLayerType::MapSurfaceLower:  return mapSurfaceLower_zOff;
            case RenderLayerType::WaterAnim:        return waterAnim_zOff;
            case RenderLayerType::MapSurfaceUpper:  return mapSurfaceUpper_zOff;
            case RenderLayerType::UnderGoShadows:   return underGoShadows_zOff;
            case RenderLayerType::GoShadows:        return goShadows_zOff;
            case RenderLayerType::Debug:            return debug_zOff;
            case RenderLayerType::AboveMajorGoes:   return aboveMajorGoes_zOff;
            //-----
            case RenderLayerType::UIs:         return UIs_zOff;
            //-----
            case RenderLayerType::MajorGoes: 
                tprAssert(0);
                return 0.0; //- never reach
            default:
                tprAssert(0);
                return 0.0; //- never reach
        }
    }

    //======= statix =======//
    static IntVec2  windowSZ; //- 屏幕尺寸（像素）（在mac屏中，实际窗口尺寸可能为此值的2倍）
    static IntVec2  gameSZ;      //- 游戏像素尺寸

    static bool  isFullScreen;  //- 是否开启全屏模式
                                //  尚未完工，此值必须确保为 false

    static double z;

    //-- distance from zFar(to zNear) --
    //   camera.zFar 是个动态值，此处只能保存一个 相对偏移
    static double  ground_zOff;
        // 整个游戏最 “深” 的图层，a canvas，往往涂上一个单一的底色

    static double  groundGo_zOff;
        

    static double  mapSurfaceLower_zOff; // under water
    static double  mapSurfaceUpper_zOff; // above water
        // 地表 图层。 放置 地衣，苔藓 等没有碰撞的 纯装饰性 go
        // 在未来，也许会放到 waterAnim 下方...

    static double  waterAnim_zOff;
        // 水域动画 图层。a canvas，其颜色逐帧生成，并直接渲染
        
    static double  underGoShadows_zOff;
        // 阴影层 下方的一层，
        // 目前放置 playerGoCircle 的一部分

    static double  goShadows_zOff;
        // go阴影 图层。
        
    static double  debug_zOff;
        // tprDebug 专用 图层
    
    //...

    static double  aboveMajorGoes_zOff;
        // 在 MajorGoes 上方的一层
        // 目前放置 playerGoCircle 的一部分

    static double  UIs_zOff;
        // UIs 专用 图层
};


#endif


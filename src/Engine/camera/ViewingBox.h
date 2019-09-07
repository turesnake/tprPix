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
            case RenderLayerType::Chunks:           return chunks_zOff;
            case RenderLayerType::WaterAnim:        return waterAnim_zOff;
            case RenderLayerType::MapSurfaceLower:  return mapSurfaceLower_zOff;
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

    //static double windowSZ_vs_gameSZ; //- 取代原本的 PIXES_PER_GAMEPIX 一值

    static bool  isFullScreen;  //- 是否开启全屏模式
                                //  尚未完工，此值必须确保为 false

    static double z;

    //-- distance from zFar(to zNear) --
    //   camera.zFar 是个动态值，此处只能保存一个 相对偏移
    static double  ground_zOff;
        // 整个游戏最 “深” 的图层，a canvas，往往涂上一个单一的底色

    static double  chunks_zOff;
        // 游戏地图 图层。 由无数块 chunk 拼接而成。
        // 其中 water区域是半透明的，可以看到下层的 waterAnimCanvas 效果
        // ...
        // 在当前版本中，此层为空

    static double  waterAnim_zOff;
        // 水域动画 图层。a canvas，其颜色逐帧生成，并直接渲染


    static double  mapSurfaceLower_zOff;
    static double  mapSurfaceUpper_zOff;
        // 地表 图层。 放置 地衣，苔藓 等没有碰撞的 纯装饰性 go
        // 在未来，也许会放到 waterAnim 下方...
        
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


//-- init is delay to ViewingBox::init() --
inline IntVec2  ViewingBox::windowSZ { 0, 0 };
inline IntVec2  ViewingBox::gameSZ   { 0, 0 };
//inline double ViewingBox::windowSZ_vs_gameSZ {0.0};
inline bool   ViewingBox::isFullScreen {false};

inline double ViewingBox::z { static_cast<double>(VIEWING_BOX_Z_DEEP) };

inline double  ViewingBox::ground_zOff           { 10.0 };
inline double  ViewingBox::chunks_zOff           { 20.0 };
inline double  ViewingBox::waterAnim_zOff        { 30.0 };
inline double  ViewingBox::mapSurfaceLower_zOff  { 40.0 };
inline double  ViewingBox::mapSurfaceUpper_zOff  { 50.0 };
inline double  ViewingBox::underGoShadows_zOff   { 60.0 };
inline double  ViewingBox::goShadows_zOff        { 70.0 };

//...
inline double  ViewingBox::debug_zOff         { 100.0 };

//...
inline double  ViewingBox::aboveMajorGoes_zOff  { 1890.0 };
inline double  ViewingBox::UIs_zOff             { 1900.0 };


#endif


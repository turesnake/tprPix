/*
 * ========================= ViewingBox.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   camera.viewingBox
 * ----------------------------
 */
#ifndef _TPR_VIEWING_BOX_H_
#define _TPR_VIEWING_BOX_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "windowConfig.h"
#include "config.h"
#include "RenderLayerType.h"


class ViewingBox{
public: 

    static float get_renderLayerZOff( RenderLayerType _type ){
        switch(_type){
            case RenderLayerType::Ground:      return ground_zOff;
            case RenderLayerType::Chunks:      return chunks_zOff;
            case RenderLayerType::WaterAnim:   return waterAnim_zOff;
            case RenderLayerType::MapSurfaces: return mapSurfaces_zOff;
            case RenderLayerType::GoShadows:   return goShadows_zOff;
            case RenderLayerType::Debug:       return debug_zOff;
            case RenderLayerType::MajorGoes: 
                assert(0);
                return 0.0; //- never reach
            default:
                assert(0);
                return 0.0; //- never reach
        }
    }


    //======= statix =======//
    static float x;
    static float y;
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
        // debug 专用 图层
};

inline float ViewingBox::x { (float)WORK_WIDTH };
inline float ViewingBox::y { (float)WORK_HEIGHT };
inline float ViewingBox::z { (float)VIEWING_BOX_Z_DEEP };

inline float  ViewingBox::ground_zOff        { 10.0f };
inline float  ViewingBox::chunks_zOff        { 20.0f };
inline float  ViewingBox::waterAnim_zOff     { 30.0f };
inline float  ViewingBox::mapSurfaces_zOff   { 40.0f };
inline float  ViewingBox::goShadows_zOff     { 50.0f };
inline float  ViewingBox::debug_zOff         { 60.0f };



#endif


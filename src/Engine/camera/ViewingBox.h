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

#include "pch.h"

//-------------------- Engine --------------------//
#include "RenderLayerType.h"


class ViewingBox{
public: 

    //--- MUST before OpenGL init ---
    static void init();

    static double get_renderLayerZOff( RenderLayerType type_ )noexcept{
        switch(type_){
            case RenderLayerType::Ground:           return ground_zOff;
            case RenderLayerType::GroundGo:         return groundGo_zOff;
            case RenderLayerType::Floor:            return floor_zOff;

            case RenderLayerType::BioSoup:          return bioSoup_zOff;

            case RenderLayerType::GoShadows:        return goShadows_zOff;
            case RenderLayerType::Debug:            return debug_zOff;

            case RenderLayerType::MajorGoes:        return majorGoes_zOff;

            case RenderLayerType::AboveMajorGoes:   return aboveMajorGoes_zOff;
            //-----
            case RenderLayerType::UIs:              return UIs_zOff;
            //-----
            
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

            
    //----- 取景盒 深度size -----
    static constexpr double z                       { VIEWING_BOX_Z_DEEP<double> };
    static constexpr double halfZ                   { 0.5 * ViewingBox::z };

    //-- distance from zFar(to zNear) --
    //   camera.zFar 是个动态值，此处只能保存一个 相对偏移
    static constexpr double  ground_zOff            { 10.0 };
        // 整个游戏最 “深” 的图层，a canvas，往往涂上一个单一的底色

    static constexpr double  groundGo_zOff          { 20.0 };
        

    static constexpr double  floor_zOff             { 30.0 }; // under biosoup
        // 地表 图层。 放置 地衣，苔藓 等没有碰撞的 纯装饰性 go


    static constexpr double  bioSoup_zOff           { 3000.0 };
    static constexpr double  boxCenter_2_bioSoup    { -2000.0 };
        // 异世界生物汤，独占一整段 深度区间，目前深度为 (4000)
        // bioSoup_zOff 是这段深度区间的中点

        

    static constexpr double  goShadows_zOff         { 5000.0 };
        // go阴影 图层。
        
    static constexpr double  debug_zOff             { 5010.0 };
        // tprDebug 专用 图层


    static constexpr double  majorGoes_zOff         { 7000.0 };
    static constexpr double  boxCenter_2_majorGoes  { 2000.0 };
        // 主体世界 表层go， 独占一整段 深度区间 目前深度为 (4000)
        // majorGoes_zOff 是这段深度区间的中点

    
    //...

    static constexpr double  aboveMajorGoes_zOff    { 9890.0 };
        // 在 MajorGoes 上方的一层
        // 目前放置 playerGoCircle 的一部分

    static constexpr double  UIs_zOff               { 9900.0 };
        // UIs 专用 图层
};


#endif


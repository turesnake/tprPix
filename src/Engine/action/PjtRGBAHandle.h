/*
 * ========================= PjtRGBAHandle.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.09
 *                                        修改 -- 2019.01.09
 * ----------------------------------------------------------
 *    在 "dog.PJ.png" 中，记载了每个图元帧的 pjt信息
 *    本模块就是用来 解析读取这些信息，将它们还原为 外部所需的状态
 * -------
 *  随着 colliEntSet 预制件 的推广，这个 模块 被大幅度简化
 * -------
 *  目前本模块 暂时只支持 一个 图元帧，包好 一个 colliEntSet预制件。
 *  需要拓展 ...
 * ----------------------------
 */
#ifndef _TPR_PJT_RGBA_HANDLE_H_
#define _TPR_PJT_RGBA_HANDLE_H_
//------------------- C --------------------//
#include <math.h>
#include <cassert>

//------------------- Libs --------------------//
#include "tprDataType.h"

//------------------- Engine --------------------//
#include "RGBA.h"
#include "AltiRange.h" 
#include "ColliEntHead.h"


namespace{//---------- namespace ---------//
    //--- A --- 
    u8    A_SOLID         = 255; 
    //--- R --- 
    u8    R_rootColliEntHead = 100;  //- ces with rootAnchor 
    u8    R_colliEntHead     = 200;  //- regular ces
    
    //--- B --- 
    u8    B_rootAnchor    = 255;
    u8    B_childAnchor   = 100; //- 子锚点应当 额外记录 z深度偏移。
    //...more...                 //- 从而获得 3DPos 的 子锚点信息。

    RGBA  uselessColor_1  { 200, 200, 200, 255 };

}//------------ namespace: end ---------//

//-- 只在 action 资源被加载时 才被调用，对性能无要求 --
//  use:
//  - PjtRGBAHandle jh { 5 };
//  - jh.set_rgba( _pixColor );
//  - XXX = jh.is_colliEntHead();
//
class PjtRGBAHandle{
public:
    explicit PjtRGBAHandle( u8 _off=5 ):
        off(_off)
        {}    

    inline void set_rgba( const RGBA &_rgba, const PixVec2 &_pixPos ){
        //-- reset --
        rgba = _rgba;
        colliEntHead.clear_all(); 

        is_emply_        = true;
        is_rootColliEntHead_ = false;
        is_colliEntHead_     = false;
        is_rootAnchor_       = false;
        is_childAnchor_      = false;

        //--- A ---
        if( (is_near_inner(RGBA_ChannelType::A, A_SOLID)==false) ||
            (is_near(rgba, uselessColor_1, 5)==true) ){
            return;
        }
        is_emply_ = false;

        //--- R --- 
        //------- rootColliEntHead --------
        int idx = rgba.r - R_rootColliEntHead;
        if( (idx>=0) && (idx<16) ){  //- 目前只有 16种 ces预制件
            is_rootColliEntHead_ = true;
            colliEntHead.colliEntSetIdx = idx;
            colliEntHead.colliEntHeadPPosOff = _pixPos;
            set_altiRange();
        }
        //------- colliEntHead --------
        idx = rgba.r - R_colliEntHead;
        if( (idx>=0) && (idx<16) ){  //- 目前只有 16种 ces预制件
            is_colliEntHead_ = true;
            colliEntHead.colliEntSetIdx = idx;
            colliEntHead.colliEntHeadPPosOff = _pixPos;
            set_altiRange();
        }
        
        //--- B --- 
        is_rootAnchor_   = is_near_inner( RGBA_ChannelType::B, B_rootAnchor );
        is_childAnchor_  = is_near_inner( RGBA_ChannelType::B, B_childAnchor );
        //...more...
    }

    inline bool is_emply() const {
        return  is_emply_;
    }
    inline bool is_rootColliEntHead() const {
        return  is_rootColliEntHead_;
    }
    inline bool is_colliEntHead() const {
        return  is_colliEntHead_;
    }
    inline bool is_rootAnchor() const {
        return  is_rootAnchor_;
    }
    inline bool is_childAnchor() const {
        return  is_childAnchor_;
    }

    inline const ColliEntHead &get_colliEntHead() const {
        return colliEntHead;
    }

private:
    RGBA          rgba         {}; //- 本模块处理的数据
    //---
    ColliEntHead  colliEntHead {}; //- 获得的 ceh信息组. 
    //---
    u8            off          {}; //- 颜色误差

    bool is_emply_            {false}; //- when chanel_A==0;
    bool is_rootColliEntHead_ {false}; //- 与 is_colliEntHead_ 不会同时亮起
    bool is_colliEntHead_     {false};
    bool is_rootAnchor_       {false};
    bool is_childAnchor_      {false}; 

    inline bool is_near_inner( RGBA_ChannelType _ct, u8 _target ){
        switch( _ct ){
            case RGBA_ChannelType::R:  return (abs(rgba.r-_target) <= off);
            case RGBA_ChannelType::G:  return (abs(rgba.g-_target) <= off);
            case RGBA_ChannelType::B:  return (abs(rgba.b-_target) <= off);
            case RGBA_ChannelType::A:  return (abs(rgba.a-_target) <= off);
            default:
                assert(0);
                return  false; //- never touch -
        }
    } 

    //-- 将 rgba 态 高度信息，转换为 mem态 altiRange值 --
    inline void set_altiRange(){
        u8 low  = rgba.g;
        u8 high = rgba.b;
        //-- item / surface --//
        if( low == high ){
            if( low == AltiRange::diskAlti_item ){
                colliEntHead.lAltiRange = altiRange_item;
            }else if( low == AltiRange::diskAlti_surface ){
                colliEntHead.lAltiRange = altiRange_surface;
            }else{
                assert(0);
            }
        }
        //--- major ---//
        colliEntHead.lAltiRange.set( (char)rgba.g, (char)rgba.b );
    }

};

#endif 


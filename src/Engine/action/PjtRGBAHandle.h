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


namespace{//---------- namespace ---------//
    //--- A --- 
    u8    A_SOLID         = 255; 
    //--- R --- 
    u8    R_rootColliEnt  = 100;

    //--- B --- 
    u8    B_rootAnchor    = 255;
    u8    B_childAnchor   = 100;
    //...more...

    RGBA  uselessColor_1  { 200, 200, 200, 255 };

}//------------ namespace: end ---------//

//-- 只在 action 资源被加载时 才被调用，对性能无要求 --
//  use:
//  - PjtRGBAHandle jh { 5 };
//  - jh.set_rgba( _pixColor );
//  - XXX = jh.is_rootColliEnt();
//
class PjtRGBAHandle{
public:
    explicit PjtRGBAHandle( u8 _off=5 ):
        off(_off)
        {}    

    inline void set_rgba( const RGBA &_rgba ){
        //-- reset --
        rgba = _rgba;
        colliEntSetIdx = 0;
        altiRange.low = 0;
        altiRange.high = 0; 

        is_emply_        = true;
        is_rootColliEnt_ = false;
        is_rootAnchor_   = false;
        is_childAnchor_  = false;

        //--- A ---
        if( (is_near_inner(RGBA_ChannelType::A, A_SOLID)==false) ||
            (is_near(rgba, uselessColor_1, 5)==true) ){
            return;
        }
        is_emply_ = false;

        //--- R --- 
        //------- rootColliEnt --------
        int idx = rgba.r - R_rootColliEnt;
        if( (idx>=0) && (idx<16) ){  //- 目前只有 16种 ces预制件
            is_rootColliEnt_ = true;
            colliEntSetIdx = idx;
            assert( (rgba.g<=AltiRange::jumpLimit) &&
                    (rgba.g < rgba.b) );
            altiRange.low  = rgba.g;
            altiRange.high = rgba.b;
        }

        //--- B --- 
        is_rootAnchor_   = is_near_inner( RGBA_ChannelType::B, B_rootAnchor );
        is_childAnchor_  = is_near_inner( RGBA_ChannelType::B, B_childAnchor );
        //...more...
    }

    inline bool is_emply() const {
        return  is_emply_;
    }
    inline bool is_rootColliEnt() const {
        return  is_rootColliEnt_;
    }
    inline bool is_rootAnchor() const {
        return  is_rootAnchor_;
    }
    inline bool is_childAnchor() const {
        return  is_childAnchor_;
    }

    inline const int get_colliEntSetIdx() const {
        return colliEntSetIdx;
    };

    inline const AltiRange &get_altiRange() const {
        return altiRange;
    }

private:
    RGBA       rgba           {}; //- 本模块处理的数据
    int        colliEntSetIdx {}; //- ces预制件 idx号 
    AltiRange  altiRange      {}; //- 高度区间 

    u8     off  {}; //- 颜色误差

    bool is_emply_        {false}; //- when chanel_A==0;
    bool is_rootColliEnt_ {false};
    bool is_rootAnchor_   {false};
    bool is_childAnchor_  {false}; 

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
};

#endif 


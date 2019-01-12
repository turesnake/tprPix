/*
 * ========================= PjtRGBAHandle.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.09
 *                                        修改 -- 2019.01.09
 * ----------------------------------------------------------
 *    在 "dog.PJ.png" 中，记载了每个图元帧的 pjt信息
 *    本模块就是用来 解析读取这些信息，将它们还原为 外部所需的状态
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
    u8    R_colliEnt      = 200;
    u8    R_bothColliEnt  = 250;

    //--- B --- 
    u8    B_rootAnchor    = 255;
    u8    B_childAnchor   = 100;
    //...more...

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
        rgba = _rgba;
        //--- A ---
        if( is_near(RGBA_ChannelType::A, A_SOLID) == false ){
            is_emply_ = true;
            is_rootColliEnt_ = false;
            is_colliEnt_ = false;
            is_rootAnchor_ = false;
            is_childAnchor_ = false;
            return;
        }
        is_emply_ = false;

        //--- R --- 
        // 三者只会出现一种 
        if( is_near(RGBA_ChannelType::R, R_rootColliEnt) == true ){
            is_rootColliEnt_ = true;
            is_colliEnt_     = false;
        }else if( is_near(RGBA_ChannelType::R, R_colliEnt) == true ){
            is_rootColliEnt_ = false;
            is_colliEnt_     = true;
        }else if( is_near(RGBA_ChannelType::R, R_bothColliEnt) == true ){
            is_rootColliEnt_ = true;
            is_colliEnt_     = true;
        }else{
            is_rootColliEnt_ = false;
            is_colliEnt_     = false;
        }

        if( is_colliEnt_ == true ){
            assert( (rgba.g<=AltiRange::jumpLimit) &&
                    (rgba.g < rgba.b) );
            altiRange.low  = rgba.g;
            altiRange.high = rgba.b;
        }

        //--- B --- 
        is_rootAnchor_   = is_near( RGBA_ChannelType::B, B_rootAnchor );
        is_childAnchor_  = is_near( RGBA_ChannelType::B, B_childAnchor );
        //...more...
    }

    inline bool is_emply() const {
        return  is_emply_;
    }
    inline bool is_rootColliEnt() const {
        return  is_rootColliEnt_;
    }
    inline bool is_colliEnt() const {
        return  is_colliEnt_;
    }
    inline bool is_rootAnchor() const {
        return  is_rootAnchor_;
    }
    inline bool is_childAnchor() const {
        return  is_childAnchor_;
    }

    inline const AltiRange &get_altiRange() const {
        return altiRange;
    }

private:
    RGBA   rgba {};     //- 本模块处理的数据
    u8     off  {}; //- 颜色误差
    
    AltiRange  altiRange {};

    bool is_emply_        {false}; //- when chanel_A==0;
    bool is_rootColliEnt_ {false};
    bool is_colliEnt_     {false};
    bool is_rootAnchor_   {false};
    bool is_childAnchor_  {false}; 

    inline bool is_near( RGBA_ChannelType _ct, u8 _target ){
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


/*
 * ========================= ColliEntSet_RGBAHandle.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY --
 * ----------------------------------------------------------
 *    ColliEntSet 每个 像素点RGBA值 解析
 * ----------------------------
 */
#ifndef _TPR_COLLI_ENT_SET_RGBA_HANDLE_H_
#define _TPR_COLLI_ENT_SET_RGBA_HANDLE_H_
//------------------- C --------------------//
#include <math.h>
#include <cassert>

//------------------- Libs --------------------//
#include "tprDataType.h"

//------------------- Engine --------------------//
#include "RGBA.h"


namespace{//---------- namespace ---------//
    //--- A --- 
    u8    A_SOLID         = 255; 
    //--- R --- 
    u8    R_colliEnt      = 100;

    //--- B --- 
    u8    B_center        = 255;
    //...more...

    RGBA  uselessColor_1  { 200, 200, 200, 255 };
    RGBA  uselessColor_2  { 170, 170, 170, 255 };
    RGBA  uselessColor_3  { 140, 150, 140, 255 };

}//------------ namespace: end ---------//


//-- 只在 colliEntSets 资源被加载时 才被调用，对性能无要求 --
//  use:
//   - ColliEntSet_RGBAHandle ch {5};
//   - ch.set_rba( _pixColor );
//   - XXX = ch.is_center();
//   - ...
class ColliEntSet_RGBAHandle{
public:
    explicit ColliEntSet_RGBAHandle( u8 _off=5 ):
        off(_off)
        {} 

    inline void set_rgba( const RGBA &_rgba ){
        //-- reset --
        rgba = _rgba;
        radius_10 = 0;

        isEmpty = true;
        isCenter = false;
        isColliEnt = false;

        //-- empty --
        if( (is_near_inner(RGBA_ChannelType::A, A_SOLID)==false) || 
            (rgba.is_near( uselessColor_1, 5)==true) ||
            (rgba.is_near( uselessColor_2, 5)==true) ||
            (rgba.is_near( uselessColor_3, 5)==true) ){
            return;
        }
        isEmpty = false;

        //--- R --- 
        if( is_near_inner(RGBA_ChannelType::R, R_colliEnt) == true ){
            isColliEnt     = true;
        }

        //--- B --- 
        if( is_near_inner(RGBA_ChannelType::B, B_center) == true ){
            isCenter       = true;
            assert( rgba.g > 0 );
            radius_10 = (int)rgba.g;
        }
    }

    inline bool is_emply() const {
        return  isEmpty;
    }
    inline bool is_center() const {
        return  isCenter;
    }
    inline bool is_colliEnt() const {
        return  isColliEnt;
    }

    inline int get_radius_10() const {
        return radius_10;
    }

private:
    RGBA   rgba      {};  //- 本模块处理的数据
    int    radius_10 {};  //- (半径 * 10) -- 从而维持 整型 

    u8     off  {}; //- 颜色误差
    
    bool isEmpty        {false}; //- when chanel_A==0, or {200,200,200,255}
    bool isCenter       {false};
    bool isColliEnt     {false};
 
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


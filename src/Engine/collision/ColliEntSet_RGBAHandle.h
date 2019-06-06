/*
 * ========================= ColliEntSet_RGBAHandle.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY --
 * ----------------------------------------------------------
 *    ColliEntSet 每个 像素点RGBA值 解析
 * ----------------------------
 */
#ifndef TPR_COLLI_ENT_SET_RGBA_HANDLE_H_
#define TPR_COLLI_ENT_SET_RGBA_HANDLE_H_

//------------------- C --------------------//
#include <cmath>
//#include <cassert>

//------------------- Libs --------------------//
#include "tprDataType.h"

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "RGBA.h"


namespace{//---------- namespace ---------//
    //--- A --- 
    u8_t    A_SOLID         = 255; 
    //--- R --- 
    u8_t    R_colliEnt      = 100;

    //--- B --- 
    u8_t    B_center        = 255;
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
    explicit ColliEntSet_RGBAHandle( int _off=5 ):
        off(_off)
        {} 

    inline void set_rgba( const RGBA &_rgba ){
        //-- reset --
        this->rgba = _rgba;

        this->isEmpty = true;
        this->isCenter = false;
        this->isColliEnt = false;

        //-- empty --
        if( (is_near_inner(RGBA_ChannelType::A, A_SOLID)==false) || 
            (this->rgba.is_near( uselessColor_1, 5)==true) ||
            (this->rgba.is_near( uselessColor_2, 5)==true) ||
            (this->rgba.is_near( uselessColor_3, 5)==true) ){
            return;
        }
        this->isEmpty = false;

        //--- R --- 
        if( is_near_inner(RGBA_ChannelType::R, R_colliEnt) == true ){
            this->isColliEnt = true;
        }

        //--- B --- 
        if( is_near_inner(RGBA_ChannelType::B, B_center) == true ){
            this->isCenter       = true;
            tprAssert( this->rgba.g > 0 );
        }
    }

    inline bool is_emply() const {
        return  this->isEmpty;
    }
    inline bool is_center() const {
        return  this->isCenter;
    }
    inline bool is_colliEnt() const {
        return  this->isColliEnt;
    }

private:
    
    inline bool is_near_inner( RGBA_ChannelType _ct, u8_t _target ){
        switch( _ct ){
            case RGBA_ChannelType::R:  return (std::abs(static_cast<int>(this->rgba.r-_target)) <= this->off);
            case RGBA_ChannelType::G:  return (std::abs(static_cast<int>(this->rgba.g-_target)) <= this->off);
            case RGBA_ChannelType::B:  return (std::abs(static_cast<int>(this->rgba.b-_target)) <= this->off);
            case RGBA_ChannelType::A:  return (std::abs(static_cast<int>(this->rgba.a-_target)) <= this->off);
            default:
                tprAssert(0);
                return  false; //- never touch -
        }
    } 

    //======== vals ========//
    RGBA   rgba      {};  //- 本模块处理的数据

    int     off  {}; //- 颜色误差. 为了照顾 std::abs(), 改用 int 类型
    
    bool isEmpty        {false}; //- when chanel_A==0, or {200,200,200,255}
    bool isCenter       {false};
    bool isColliEnt     {false};
};

#endif 


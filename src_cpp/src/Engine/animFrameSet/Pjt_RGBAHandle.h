/*
 * ========================= Pjt_RGBAHandle.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.09
 *                                        MODIFY -- 
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
#ifndef TPR_PJT_RGBA_HANDLE_H
#define TPR_PJT_RGBA_HANDLE_H
//------------------- C --------------------//
#include <cmath>

//------------------- Libs --------------------//
#include "tprDataType.h"

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "RGBA.h"
#include "AltiRange.h" 
#include "ColliEntHead.h"


namespace pjt_RGBAHandle_inn{//---------- namespace ---------//
    //--- A --- 
    u8_t    A_SOLID         { 255 }; 
    //--- R --- 
    u8_t    R_rootColliEntHead_CarryAffect { 50 };   //- ces with rootAnchor (carry affect)
    u8_t    R_rootColliEntHead_NoAffect    { 100 };  //- ces with rootAnchor (no affect)
                //- 新版本中，一个mesh 只有一个 ces/ceh
                //  也就是这里的 rootCEH
    
    //--- B --- 
    u8_t    B_rootAnchor    { 255 };
    u8_t    B_childAnchor   { 100 }; //- 子锚点应当 额外记录 z深度偏移。
    //...more...                 //- 从而获得 3DPos 的 子锚点信息。

    RGBA  uselessColor_1  { 200, 200, 200, 255 };

}//------------ namespace: end ---------//

//-- 只在 animFrameSet 资源被加载时 才被调用，对性能无要求 --
//  use:
//  - Pjt_RGBAHandle jh { 5 };
//  - jh.set_rgba( _pixColor );
//  - XXX = jh.is_colliEntHead();
class Pjt_RGBAHandle{
public:
    explicit Pjt_RGBAHandle( int off_=5 ):
        off(off_)
        {}    

    //inline void set_rgba( const RGBA &rgba_, const IntVec2 &_pixPos ){
    inline void set_rgba( const RGBA &rgba_ ){
        //-- reset --
        this->rgba = rgba_;
        this->colliEntHead.clear_all(); 

        this->isEmpty            = true;
        this->isRootColliEntHead = false;
        this->isRootAnchor       = false;
        this->isChildAnchor      = false;

        //--- A ---
        if( (is_near_inner(RGBA_ChannelType::A, pjt_RGBAHandle_inn::A_SOLID)==false) ||
            (this->rgba.is_near( pjt_RGBAHandle_inn::uselessColor_1, 5)==true) ){
            return;
        }
        this->isEmpty = false;

        //--- R --- 
        //------- rootColliEntHead_NoAffect --------
        u8_t cesNUm = 16; //- 目前只有 16种 ces预制件
        if(       is_in_range(this->rgba.r, pjt_RGBAHandle_inn::R_rootColliEntHead_CarryAffect, cesNUm) ){ //---
            this->isRootColliEntHead = true;
            this->colliEntHead.colliEntSetIdx = this->rgba.r - pjt_RGBAHandle_inn::R_rootColliEntHead_CarryAffect;
            this->colliEntHead.isCarryAffect = true;
            this->set_altiRange();
        }
        else if( is_in_range(this->rgba.r, pjt_RGBAHandle_inn::R_rootColliEntHead_NoAffect, cesNUm) ){    //---
            this->isRootColliEntHead = true;
            this->colliEntHead.colliEntSetIdx = this->rgba.r - pjt_RGBAHandle_inn::R_rootColliEntHead_NoAffect;
            this->colliEntHead.isCarryAffect = false;
            this->set_altiRange();
        }
        
        //--- B --- 
        this->isRootAnchor   = is_near_inner( RGBA_ChannelType::B, pjt_RGBAHandle_inn::B_rootAnchor );
        this->isChildAnchor  = is_near_inner( RGBA_ChannelType::B, pjt_RGBAHandle_inn::B_childAnchor );
        //...more...
    }

    inline bool is_emply() const {
        return  this->isEmpty;
    }
    inline bool is_rootColliEntHead() const {
        return  this->isRootColliEntHead;
    }
    inline bool is_rootAnchor() const {
        return  this->isRootAnchor;
    }
    inline bool is_childAnchor() const {
        return  this->isChildAnchor;
    }
    inline const ColliEntHead &get_colliEntHead() const {
        return this->colliEntHead;
    }

private:

    inline bool is_near_inner( RGBA_ChannelType ct_, u8_t target_ ){
        switch( ct_ ){
            case RGBA_ChannelType::R:  return (std::abs(static_cast<int>(this->rgba.r-target_)) <= this->off);
            case RGBA_ChannelType::G:  return (std::abs(static_cast<int>(this->rgba.g-target_)) <= this->off);
            case RGBA_ChannelType::B:  return (std::abs(static_cast<int>(this->rgba.b-target_)) <= this->off);
            case RGBA_ChannelType::A:  return (std::abs(static_cast<int>(this->rgba.a-target_)) <= this->off);
            default:
                tprAssert(0);
                return  false; //- never touch -
        }
    } 

    //-- 将 rgba 态 高度信息，转换为 mem态 altiRange值 --
    inline void set_altiRange(){
        u8_t low  = this->rgba.g;
        u8_t high = this->rgba.b;


            //-- item / surface --//
            // 在未来，item/surface 设计 可能会被取消...
            /*
            if( low == high ){
                if( low == AltiRange::diskAlti_item ){
                    colliEntHead.lAltiRange = altiRange_item;
                    return;
                }else if( low == AltiRange::diskAlti_surface ){
                    colliEntHead.lAltiRange = altiRange_surface;
                    return;
                }else{
                    tprAssert(0);
                }
            }
            */

        //-- 暂时无视 item / surface 这组概念
        //   但，高度区间是一定要有的，不然怎么实现碰撞检测 ？？？
        tprAssert( low < high );
            
        //-- isBody --//
        if( low > 100 ){
            this->colliEntHead.isBody = false;
            low  -= 100;
            high -= 100;
        }
        this->colliEntHead.isBody = true;

        //--- major ---//
        this->colliEntHead.lAltiRange.set( static_cast<char>(low), static_cast<char>(high) );
    }

    //-- 检测 参数 _beCheck，是否在 [_low,_low+_off) 区间内
    inline bool is_in_range( u8_t beCheck_, u8_t low_, u8_t off_ ){
        return ((beCheck_>=low_) && (beCheck_<(low_+off_)));
    }

    //======== vals ========//
    RGBA          rgba         {}; //- 本模块处理的数据
    ColliEntHead  colliEntHead {}; //- 获得的 ceh信息组. 
    int           off          {}; //- 颜色误差. 为了照顾 std::abs(), 改用 int 类型

    bool isEmpty            {false}; //- when chanel_A==0;
    bool isRootColliEntHead {false}; //- 与 isColliEntHead 不会同时亮起
    bool isRootAnchor       {false};
    bool isChildAnchor      {false}; 
};

#endif 


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
#ifndef _TPR_PJT_RGBA_HANDLE_H_
#define _TPR_PJT_RGBA_HANDLE_H_
//------------------- C --------------------//
#include <cmath>
#include <cassert>

//------------------- Libs --------------------//
#include "tprDataType.h"

//------------------- Engine --------------------//
#include "RGBA.h"
#include "AltiRange.h" 
#include "ColliEntHead.h"


namespace{//---------- namespace ---------//
    //--- A --- 
    u8_t    A_SOLID         = 255; 
    //--- R --- 
    u8_t    R_rootColliEntHead_CarryAffect = 50;   //- ces with rootAnchor (carry affect)
    u8_t    R_colliEntHead_CarryAffect     = 150;  //- regular ces         (carry affect)
    u8_t    R_rootColliEntHead_NoAffect    = 100;  //- ces with rootAnchor (no affect)
    u8_t    R_colliEntHead_NoAffect        = 200;  //- regular ces         (no affect)
    
    //--- B --- 
    u8_t    B_rootAnchor    = 255;
    u8_t    B_childAnchor   = 100; //- 子锚点应当 额外记录 z深度偏移。
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
    explicit Pjt_RGBAHandle( u8_t _off=5 ):
        off(_off)
        {}    

    inline void set_rgba( const RGBA &_rgba, const IntVec2 &_pixPos ){
        //-- reset --
        rgba = _rgba;
        colliEntHead.clear_all(); 

        isEmpty        = true;
        isRootColliEntHead = false;
        isColliEntHead     = false;
        isRootAnchor       = false;
        isChildAnchor      = false;

        //--- A ---
        if( (is_near_inner(RGBA_ChannelType::A, A_SOLID)==false) ||
            (rgba.is_near( uselessColor_1, 5)==true) ){
            return;
        }
        isEmpty = false;

        //--- R --- 
        //------- rootColliEntHead_NoAffect --------
        int cesNUm = 16; //- 目前只有 16种 ces预制件
        if(       is_in_range(rgba.r, R_rootColliEntHead_CarryAffect, cesNUm) ){ //---
            isRootColliEntHead = true;
            colliEntHead.colliEntSetIdx = rgba.r - R_rootColliEntHead_CarryAffect;
            colliEntHead.pposOff_fromRootAnchor = _pixPos; //-未完，后面还需要 "- rootAnchorOff"
            colliEntHead.isCarryAffect = true;
            set_altiRange();

        }else if( is_in_range(rgba.r, R_rootColliEntHead_NoAffect, cesNUm) ){    //---
            isRootColliEntHead = true;
            colliEntHead.colliEntSetIdx = rgba.r - R_rootColliEntHead_NoAffect;
            colliEntHead.pposOff_fromRootAnchor = _pixPos; //-未完，后面还需要 "- rootAnchorOff"
            colliEntHead.isCarryAffect = false;
            set_altiRange();

        }else if( is_in_range(rgba.r, R_colliEntHead_CarryAffect, cesNUm) ){     //---
            isColliEntHead = true;
            colliEntHead.colliEntSetIdx = rgba.r - R_colliEntHead_CarryAffect;
            colliEntHead.pposOff_fromRootAnchor = _pixPos; //-未完，后面还需要 "- rootAnchorOff"
            colliEntHead.isCarryAffect = true;
            set_altiRange();

        }else if( is_in_range(rgba.r, R_colliEntHead_NoAffect, cesNUm) ){        //---
            isColliEntHead = true;
            colliEntHead.colliEntSetIdx = rgba.r - R_colliEntHead_NoAffect;
            colliEntHead.pposOff_fromRootAnchor = _pixPos; //-未完，后面还需要 "- rootAnchorOff"
            colliEntHead.isCarryAffect = false;
            set_altiRange();
        }
        
        //--- B --- 
        isRootAnchor   = is_near_inner( RGBA_ChannelType::B, B_rootAnchor );
        isChildAnchor  = is_near_inner( RGBA_ChannelType::B, B_childAnchor );
        //...more...
    }

    inline bool is_emply() const {
        return  isEmpty;
    }
    inline bool is_rootColliEntHead() const {
        return  isRootColliEntHead;
    }
    inline bool is_colliEntHead() const {
        return  isColliEntHead;
    }
    inline bool is_rootAnchor() const {
        return  isRootAnchor;
    }
    inline bool is_childAnchor() const {
        return  isChildAnchor;
    }

    inline const ColliEntHead &get_colliEntHead() const {
        return colliEntHead;
    }

private:

    inline bool is_near_inner( RGBA_ChannelType _ct, u8_t _target ){
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
        u8_t low  = rgba.g;
        u8_t high = rgba.b;


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
                    assert(0);
                }
            }
            */

        //-- 暂时无视 item / surface 这组概念
        //   但，高度区间是一定要有的，不然怎么实现碰撞检测 ？？？
        assert( low < high );
            
        //-- isBody --//
        if( low > 100 ){
            colliEntHead.isBody = false;
            low  -= 100;
            high -= 100;
        }
        colliEntHead.isBody = true;

        //--- major ---//
        colliEntHead.lAltiRange.set( (char)low, (char)high );
    }

    //-- 检测 参数 _beCheck，是否在 [_low,_low+_off) 区间内
    inline bool is_in_range( u8_t _beCheck, u8_t _low, u8_t _off ){
        return ((_beCheck>=_low) && (_beCheck<(_low+_off)));
    }

    //======== vals ========//
    RGBA          rgba         {}; //- 本模块处理的数据
    //---
    ColliEntHead  colliEntHead {}; //- 获得的 ceh信息组. 
    //---
    u8_t            off          {}; //- 颜色误差

    bool isEmpty            {false}; //- when chanel_A==0;
    bool isRootColliEntHead {false}; //- 与 isColliEntHead 不会同时亮起
    bool isColliEntHead     {false};
    bool isRootAnchor       {false};
    bool isChildAnchor      {false}; 
};

#endif 


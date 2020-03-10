/*
 * ====================== BioSoupColor.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.10
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_BIO_SOUP_COLOR_H
#define TPR_BIO_SOUP_COLOR_H

#include "pch.h"


#include <utility>

//-------------------- Engine --------------------//
#include "FloatVec.h"


namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------


class Color {
public:
    constexpr Color()=default;
    constexpr Color( const FloatVec3 &light_,
                            const FloatVec3 &mid_,
                            const FloatVec3 &dark_ ):
        light(light_),
        mid(mid_),
        dark(dark_)
        {}

    // param: pct_ (0, 1.0)
    // return:
    //      true  - do close
    //      false - already close enough, do nothing
    inline bool close_to( const Color &target_, float pct_ )noexcept{
            
        FloatVec3 lOff = target_.light - this->light;
        FloatVec3 mOff = target_.mid   - this->mid;
        FloatVec3 dOff = target_.dark  - this->dark;
        constexpr FloatVec3 zero { 0.0f, 0.0f, 0.0f };
        //--
        if( is_closeEnough( lOff, zero, 0.01f ) &&
            is_closeEnough( mOff, zero, 0.01f ) &&
            is_closeEnough( dOff, zero, 0.01f ) ){
            return false; // close enough
        }
        // do close 
        this->light += lOff * pct_;
        this->mid   += mOff * pct_;
        this->dark  += dOff * pct_;
        return true;
    }
    //===//
    FloatVec3 light;
    FloatVec3 mid;
    FloatVec3 dark;
};



class ColorNode {
public:
    const Color *colorPtr   {};
    float       closePct {}; // 从任何颜色，接近本 node color 的接近率 (0.0f, 1.0f)
};





// 一组循环流动的 颜色信息，
// 当 diosoup 进入一种 chain 实例，就会停留在里面，循环播放这些颜色
class ColorNodeChain{
public:
    

    static void init_for_state()noexcept;

    // 切换不同的 chain
    inline static void bind_a_chain( const std::string &chainName_ )noexcept{
        tprAssert( ColorNodeChain::chains.find(chainName_) != ColorNodeChain::chains.end() );
        ColorNodeChain::currentChainPtr = ColorNodeChain::chains.at(chainName_).get();
        //--
        ColorNodeChain::nodeIdx = 0;
    }

    // 自动计算 本帧的 颜色
    static const Color &calc_next_baseColor()noexcept{
        const ColorNode &node = ColorNodeChain::currentChainPtr->get_node( ColorNodeChain::nodeIdx );
        bool ret = ColorNodeChain::currentColor.close_to( *node.colorPtr , node.closePct );
        if( !ret ){
            ColorNodeChain::nodeIdx++;
            if( ColorNodeChain::nodeIdx >= ColorNodeChain::currentChainPtr->nodes.size() ){
                ColorNodeChain::nodeIdx = 0;
            }
        }
        //===
        return ColorNodeChain::currentColor;
    }

private:

    // 在构造语句中，直接创建 nodes
    explicit ColorNodeChain( std::vector<ColorNode> &&nodes_ ):
        nodes( std::move(nodes_) )
        {}

    const ColorNode &get_node( size_t idx_ )const noexcept{
        tprAssert( this->nodes.size() > idx_ );
        return this->nodes.at(idx_);
    }


    const std::vector<ColorNode>  nodes; // 在 init 就被创建的 固定不变的数据

    //------------ all chains --------------//
    static std::unordered_map<std::string, std::unique_ptr<ColorNodeChain>> chains;

    //------------- user data -------------//  
    static const ColorNodeChain *currentChainPtr;
    static size_t                nodeIdx; // 指向 currentChain 中的 targetNode
    static Color                 currentColor; // 调和色，返回给用户
};



}//------------- namespace gameObjs::bioSoup: end ----------------
#endif 


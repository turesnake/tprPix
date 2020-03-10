/*
 * ====================== SignInMapEnts_Square.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_SIGN_IN_MAP_ENTS_SQUARE_H
#define TPR_SIGN_IN_MAP_ENTS_SQUARE_H

#include "pch.h"

//--------------- Engine ------------------//
#include "SignInMapEnts_Square_Type.h"


class SignInMapEnts_Square{
public:
    inline const std::vector<IntVec2> &get_all_mapEntOffs()const noexcept{ return this->mapEntOffs; }
    inline const glm::dvec2 &get_rootMapEntMid_2_rootAnchor_dposOff()const noexcept{ return this->rootMapEntMid_2_rootAnchor_dposOff; }
    

    static inline const SignInMapEnts_Square &get_signInMapEnts_square_ref( SignInMapEnts_Square_Type type_ )noexcept{
        tprAssert( SignInMapEnts_Square::dataUPtrs.find(type_) != SignInMapEnts_Square::dataUPtrs.end() );
        return *SignInMapEnts_Square::dataUPtrs.at(type_);
    }


    static void init_for_static()noexcept; // MUST CALL IN MAIN !!! 

private:
    SignInMapEnts_Square()=default;

    static inline SignInMapEnts_Square &insert_new_signInMapEnts_square( SignInMapEnts_Square_Type type_ )noexcept{

        std::unique_ptr<SignInMapEnts_Square> uptr ( new SignInMapEnts_Square() ); // can't use std::make_unique
        auto [insertIt, insertBool] = SignInMapEnts_Square::dataUPtrs.emplace( type_, std::move(uptr) );
        tprAssert( insertBool );
        return *(insertIt->second);
    }

    //-------- vals --------//
    std::vector<IntVec2> mapEntOffs {}; 
                    // 每个 squGo,恒定拥有一个 rootMP (off={0,0}
                    // 以及 0～n 个 weakMP

    glm::dvec2          rootMapEntMid_2_rootAnchor_dposOff {};
                    // 加上 rootMPMidDPos, 可以计算出 go rootAnchor 所在点。（也就是 goDPos 位置）
                    // ---
                    // rootMP 往往不能成为 整组 multiMP 的正中心位置（偏向右上角）


    //======= static =======//
    static std::unordered_map<SignInMapEnts_Square_Type, std::unique_ptr<SignInMapEnts_Square>> dataUPtrs;
};


#endif 


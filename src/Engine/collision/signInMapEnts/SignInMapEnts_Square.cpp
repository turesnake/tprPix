/*
 * ===================== SignInMapEnts_Square.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "SignInMapEnts_Square.h"



namespace wmes_inn {//------------- namespace wmes_inn ----------------

    void batch( std::vector<IntVec2> &v_, IntVec2 iRange_, IntVec2 jRange_ );

}//------------- namespace wmes_inn: end ----------------


std::unordered_map<SignInMapEnts_Square_Type, std::unique_ptr<SignInMapEnts_Square>>    SignInMapEnts_Square::dataUPtrs;

// [*main-thread*]
// 直接手动初始化数据
void SignInMapEnts_Square::init_for_static()noexcept{

    {//----- T_1m1 -----//
        SignInMapEnts_Square &ref = SignInMapEnts_Square::insert_new_signInMapEnts_square( SignInMapEnts_Square_Type::T_1m1 );
        //ref.mapEntOffs.push_back( IntVec2 { 0,  0 } );
        ref.mapEntOffs.emplace_back(  0,  0 );
        ref.rootMapEntMid_2_rootAnchor_dposOff = glm::dvec2{ 0.0, 0.0 };
    }
    
    {//----- T_1m2 -----//
        SignInMapEnts_Square &ref = SignInMapEnts_Square::insert_new_signInMapEnts_square( SignInMapEnts_Square_Type::T_1m2 );
        //ref.mapEntOffs.push_back( IntVec2 { 0,  0 } );
        //ref.mapEntOffs.push_back( IntVec2 { 0, -1 } );
        ref.mapEntOffs.emplace_back(  0,  0  );
        ref.mapEntOffs.emplace_back(  0, -1  );
        ref.rootMapEntMid_2_rootAnchor_dposOff = glm::dvec2{ 0.0, -HALF_PIXES_PER_MAPENT_D };
    }

    {//----- T_2m1 -----//
        SignInMapEnts_Square &ref = SignInMapEnts_Square::insert_new_signInMapEnts_square( SignInMapEnts_Square_Type::T_2m1 );
        //ref.mapEntOffs.push_back( IntVec2 { 0,  0 } );
        //ref.mapEntOffs.push_back( IntVec2 { -1, 0 } );
        ref.mapEntOffs.emplace_back(  0,  0  );
        ref.mapEntOffs.emplace_back(  -1, 0  );
        ref.rootMapEntMid_2_rootAnchor_dposOff = glm::dvec2{ -HALF_PIXES_PER_MAPENT_D, 0.0 };
    }

    {//----- T_2m2 -----//
        SignInMapEnts_Square &ref = SignInMapEnts_Square::insert_new_signInMapEnts_square( SignInMapEnts_Square_Type::T_2m2 );
        wmes_inn::batch( ref.mapEntOffs, IntVec2{ -1, 0 }, IntVec2{ -1, 0 } );
        ref.rootMapEntMid_2_rootAnchor_dposOff = glm::dvec2{ -HALF_PIXES_PER_MAPENT_D,  -HALF_PIXES_PER_MAPENT_D };
    }

    {//----- T_3m3 -----//
        SignInMapEnts_Square &ref = SignInMapEnts_Square::insert_new_signInMapEnts_square( SignInMapEnts_Square_Type::T_3m3 );
        wmes_inn::batch( ref.mapEntOffs, IntVec2{ -1, 1 }, IntVec2{ -1, 1 } );
        ref.rootMapEntMid_2_rootAnchor_dposOff = glm::dvec2{ 0.0,  0.0 };
    }

    {//----- T_4m4 -----//
        SignInMapEnts_Square &ref = SignInMapEnts_Square::insert_new_signInMapEnts_square( SignInMapEnts_Square_Type::T_4m4 );
        wmes_inn::batch( ref.mapEntOffs, IntVec2{ -2, 1 }, IntVec2{ -2, 1 } );
        ref.rootMapEntMid_2_rootAnchor_dposOff = glm::dvec2{ -HALF_PIXES_PER_MAPENT_D,  -HALF_PIXES_PER_MAPENT_D };
    }

    //... move ...
}



namespace wmes_inn {//------------- namespace wmes_inn ----------------


void batch( std::vector<IntVec2> &v_, IntVec2 iRange_, IntVec2 jRange_ ){

    tprAssert(  (iRange_.x <= 0) && 
                (iRange_.y >= 0) &&
                (jRange_.x <= 0) && 
                (jRange_.y >= 0) );
    tprAssert(  (iRange_.x <= iRange_.y) && 
                (jRange_.x <= jRange_.y) );
    
    for( int j=jRange_.x; j<=jRange_.y; j++ ){
        for( int i=iRange_.x; i<=iRange_.y; i++ ){

            // 包含 rootMapEnt
            //if( (j==0) && (i==0) ){ 
            //    continue; // {0,0} 是默认的 rootMapEnt 的位置，要被跳过
            //}

            //v_.push_back( IntVec2{ i, j } );
            v_.emplace_back(  i, j );
        }
    }
}


}//------------- namespace wmes_inn: end ----------------


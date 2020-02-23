/*
 * ===================== WeakMapEntSet.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "WeakMapEntSet.h"




namespace wmes_inn {//------------- namespace wmes_inn ----------------

    void batch( std::vector<IntVec2> &v_, IntVec2 iRange_, IntVec2 jRange_ );

}//------------- namespace wmes_inn: end ----------------



std::unordered_map<WeakMapEntsType, std::vector<IntVec2>>   WeakMapEntSet::datas {};
bool                                                        WeakMapEntSet::isStaticInit {false};


// 直接手动初始化数据
void WeakMapEntSet::init_for_static()noexcept{


    //----- Empty -----//
    WeakMapEntSet::datas.insert({ WeakMapEntsType::Empty, 
        std::vector<IntVec2>{}
    });

    //----- T_1m2 -----//
    WeakMapEntSet::datas.insert({ WeakMapEntsType::T_1m2, 
        std::vector<IntVec2>{
            IntVec2 { 0, -1 }
        }
    });

    //----- T_2m1 -----//
    WeakMapEntSet::datas.insert({ WeakMapEntsType::T_2m1, 
        std::vector<IntVec2>{
            IntVec2 { -1, 0 }
        }
    });

    
    {//----- T_2m2 -----//
        auto [insertIt, insertBool] = WeakMapEntSet::datas.insert({ WeakMapEntsType::T_2m2, std::vector<IntVec2>{} });
        tprAssert( insertBool );
        wmes_inn::batch( insertIt->second, IntVec2{ -1, 0 }, IntVec2{ -1, 0 } );
    }

    {//----- T_3m3 -----//
        auto [insertIt, insertBool] = WeakMapEntSet::datas.insert({ WeakMapEntsType::T_3m3, std::vector<IntVec2>{} });
        tprAssert( insertBool );
        wmes_inn::batch( insertIt->second, IntVec2{ -1, 1 }, IntVec2{ -1, 1 } );
    }

    {//----- T_4m4 -----//
        auto [insertIt, insertBool] = WeakMapEntSet::datas.insert({ WeakMapEntsType::T_4m4, std::vector<IntVec2>{} });
        tprAssert( insertBool );
        wmes_inn::batch( insertIt->second, IntVec2{ -2, 1 }, IntVec2{ -2, 1 } );
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
            if( (j==0) && (i==0) ){ 
                continue; // {0,0} 是默认的 rootMapEnt 的位置，要被跳过
            }
            v_.push_back( IntVec2{ i, j } );
        }
    }
}


}//------------- namespace wmes_inn: end ----------------


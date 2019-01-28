/*
 * ======================= Cycle.cpp ========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.31
 *                                        修改 -- 2018.12.31
 * ----------------------------------------------------------
 *    动画动作 处理器：简单死循环动作 处理器
 * ----------------------------
 */
#include "Script/actionHandle/Cycle.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <string>

using namespace std::placeholders;


using std::string;


//#include "debug.h" //- tmp


namespace actionHdle{//------------- namespace ActionHdle ----------------


//--- static member ----
u32  Cycle::typeId {0};


/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 并不是单纯的 bind，还附带了 目标ah实例 的初始化工作。
 */
void Cycle::bind(  ActionHandle *_ahPtr,
                int _begIdx,
                int _endIdx,
                int _enterIdx,
                const std::vector<int> &_steps,
                bool _isStepEqual ){

    //-- 绑定 基础ah实例 --
    //  在此之前，外部代码需要自行生成 基础ah类实例
    ahPtr = _ahPtr;

    //-- type --
    ahPtr->typeId = Cycle::typeId;

    //-- binary --
    ahPtr->binary.resize( sizeof(Cycle_Binary) );
    bp = (Cycle_Binary*)&(ahPtr->binary.at(0)); 
    //===================================//

    assert( (_enterIdx>=_begIdx) && (_enterIdx<=_endIdx) );
    bp->begIdx = _begIdx;
    bp->endIdx = _endIdx;
    bp->enterIdx = _enterIdx;
    ahPtr->currentIdx = _enterIdx;
    bp->updates = 0;

    //-------- steps --------//
    bp->isStepEqual = _isStepEqual;
    bp->steps.insert( bp->steps.end(), _steps.begin(), _steps.end() );
    if( _isStepEqual == true ){
        assert( _steps.size() == 1 );
        bp->currentStep = bp->steps.at(0);
    }else{
        assert( _steps.size() == (_endIdx - _begIdx + 1) );
        bp->currentStep = bp->steps.at( _enterIdx - _begIdx );
    }

    //----- sign up callback funcs -----
    //-- 故意将 首参数this 绑定到 保留类实例 cycle 身上
    if( _isStepEqual == true ){
        ahPtr->funcs.insert({ "update", 
                std::bind( &Cycle::update_equal, &cycle_obj, _1 )
                });
    }else{
        ahPtr->funcs.insert({ "update", 
                std::bind( &Cycle::update_NotEqual, &cycle_obj, _1 )
                });
    }
}


/* ===========================================================
 *                        update_equal
 * -----------------------------------------------------------
 * -- int update( ActionHandle *_ahPtr )
 * -- 需要在 每一视觉帧 调用
 * -- 若 某段时间 不主动调用本函数，动画将陷入停滞（并不会严格对应到全局总时间帧）
 * -- 顺带返回  currentIdx 的值
 */
int Cycle::update_equal( ActionHandle *_ahPtr ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    assert( _ahPtr->typeId == Cycle::typeId );
    //-- rebind ptr -----
    ahPtr = _ahPtr;
    bp = (Cycle_Binary*)&(ahPtr->binary.at(0));

    //=====================================//
    bp->updates++;

    //----- node frame -----//
    if( bp->updates >= bp->currentStep ){
        bp->updates = 0;
        //--- currentIdx ---
        (ahPtr->currentIdx==bp->endIdx) ? ahPtr->currentIdx=bp->begIdx :
                                          ahPtr->currentIdx++;
        //--- currentStep ---
        // 永远不用重设
    }
    //=====================================//
    //                ret
    //-------------------------------------//  
    scriptBuf.push_int( ahPtr->currentIdx );
    return sizeof(int);
}

/* ===========================================================
 *                        update_NotEqual
 * -----------------------------------------------------------
 * -- int update( ActionHandle *_ahPtr )
 * -- 需要在 每一视觉帧 调用
 * -- 若 某段时间 不主动调用本函数，动画将陷入停滞（并不会严格对应到全局总时间帧）
 * -- 顺带返回  currentIdx 的值
 */
int Cycle::update_NotEqual( ActionHandle *_ahPtr ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    assert( _ahPtr->typeId == Cycle::typeId );
    //-- rebind ptr -----
    ahPtr = _ahPtr;
    bp = (Cycle_Binary*)&(ahPtr->binary.at(0));

    //=====================================//
    bp->updates++;

    //----- node frame -----//
    if( bp->updates >= bp->currentStep ){
        bp->updates = 0;
        //--- currentIdx ---
        (ahPtr->currentIdx==bp->endIdx) ? ahPtr->currentIdx=bp->begIdx :
                                          ahPtr->currentIdx++;
        //--- currentStep ---
        bp->currentStep=bp->steps.at(ahPtr->currentIdx-bp->begIdx);
    }
    //=====================================//
    //                ret
    //-------------------------------------//  
    scriptBuf.push_int( ahPtr->currentIdx );
    return sizeof(int);
}


}//----------------- namespace ActionHdle: end -------------------


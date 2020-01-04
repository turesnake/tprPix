/*
 * ====================== CircuitBoard.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CIRCUIT_BOARD_H
#define TPR_CIRCUIT_BOARD_H


//-------------------- CPP --------------------//
#include <vector>
#include <functional>
#include <unordered_map>
#include <map>

#include <memory>


//------------------- Engine --------------------//
#include "GameObjType.h"

#include "IntVec.h"
#include "mapEntKey.h"
#include "chunkKey.h"

#include "tprAssert.h"


namespace circuitBoard {//------------------ namespace: circuitBoard start ---------------------//


// 调用优先级
enum class MessageWeight{
    V_0 = 0, // last call
    V_1,
    V_2,
    V_3,
    V_4,
    V_5, // first call
};


// 
class Message{
public:
    Message( F_AFFECT functor_, goid_t dogoid_ ):
        functor(functor_),
        dogoid(dogoid_)
        {}

    F_AFFECT  functor {nullptr};
    goid_t    dogoid {};
};



// 所有数据 按照 chunk 分成块存储。以便在释放阶段，一次性释放掉 整个 chunk 的数据
class ChunkData{
public:
    ChunkData()=default;
    std::unordered_map<mapEntKey_t, std::multimap<MessageWeight, std::unique_ptr<Message>>> messages {};
};



class CircuitBoard{
public:

    static void signUp( goid_t dogoid_, IntVec2 mpos_, MessageWeight weight_, F_AFFECT functor_ )noexcept;

    static void check_and_call_messages( IntVec2 mpos_, GameObj &begoRef_ )noexcept;

    inline static void erase_chunkData( chunkKey_t key_ )noexcept{
        CircuitBoard::datas.erase( key_ ); // maybe. 若不存在此 key，则什么也不做
    }

private:

    static std::unordered_map<chunkKey_t, std::unique_ptr<ChunkData>> datas;
};


}//--------------------- namespace: circuitBoard end ------------------------//
#endif 


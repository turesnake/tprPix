/*
 * ===================== CircuitBoard.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "circuitBoard/CircuitBoard.h"



//------------------- Engine --------------------//
#include "GameObj.h"

#include "esrc_gameObj.h"



namespace circuitBoard {//------------------ namespace: circuitBoard start ---------------------//


//===== static =====//
std::unordered_map<chunkKey_t, std::unique_ptr<ChunkData>> CircuitBoard::datas {};



// dogo 主动登记 msg 函数 
void CircuitBoard::signUp( goid_t dogoid_, IntVec2 mpos_, MessageWeight weight_, F_AFFECT functor_ )noexcept{

    chunkKey_t chunkKey = anyMPos_2_chunkKey(mpos_);
    
    // 兼顾 查找or添加 的 无脑 insert
    // 若已存在，什么操作也不做，返回 目标原始 迭代器
    // 若未存在，执行insert
    auto outPair1 = CircuitBoard::datas.insert({ chunkKey, std::make_unique<ChunkData>() }); // maybe
    ChunkData &chunkDataRef = *(outPair1.first->second);

    mapEntKey_t mpKey = mpos_2_key( mpos_ );

    // 兼顾 查找or添加 的 无脑 insert
    auto outPair2 = chunkDataRef.messages.insert({ mpKey, std::multimap<MessageWeight, std::unique_ptr<Message>>{} });
    auto &messages = outPair2.first->second;

    // 一定成功的 insert
    messages.insert({ weight_, std::make_unique<Message>(functor_, dogoid_) });
}



// 每个 mpgo，在自己被创建出来后，都要主动调用本函数，检查目标 
void CircuitBoard::check_and_call_messages( IntVec2 mpos_, GameObj &begoRef_ )noexcept{

    chunkKey_t chunkKey = anyMPos_2_chunkKey(mpos_);

    if( auto target=CircuitBoard::datas.find(chunkKey); target==CircuitBoard::datas.end() ){

        ChunkData &chunkDataRef = *(target->second);
        mapEntKey_t mpKey = mpos_2_key( mpos_ );

        // 本 mapEnt 没有被登记任何 msg，直接退出 
        if( chunkDataRef.messages.find(mpKey) == chunkDataRef.messages.end() ){
            return;
        }   

        auto &messages = chunkDataRef.messages.at(mpKey);
        for( auto it=messages.rbegin(); it!=messages.rend(); it++ ){ // 倒序遍历，优先执行权重值高的

            Message &msgRef = *(it->second);

            // 如果 dogo 已经不存在，主动销毁此 msg
            if( !esrc::is_go_active(msgRef.dogoid) ){
                messages.erase( it.base() );
                continue; // NEXT
            }

            // call message functor
            tprAssert( msgRef.functor != nullptr );
            GameObj &dogoRef = esrc::get_goRef( msgRef.dogoid );
            msgRef.functor( dogoRef, begoRef_ );
        }
    }
}




}//--------------------- namespace: circuitBoard end ------------------------//


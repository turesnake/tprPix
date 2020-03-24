/*
 * ==================== ecoObjCreate.cpp ======================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"

//-------------------- Engine --------------------//
#include "chunkCreate.h"
#include "sectionKey.h"
//#include "tprAssert.h"
#include "GameObj.h"

#include "jobs_all.h"
#include "Job.h"

#include "esrc_player.h"
#include "esrc_ecoObj.h"
#include "esrc_jobQue.h"



namespace chunkCreate {//------- namespace: chunkCreate ----------//
namespace eoc_inn {//----------- namespace: eoc_inn ----------------//

    bool        isFirstCall {true};
    size_t      count {0};
    IntVec2     lastPlayerSectionMPos {};

}//-------------- namespace: eoc_inn end ----------------//


 
/* 调用完本函数后，还应该处理 ecoObjMemState  
 */
void push_ecoObj_2_jobQue( sectionKey_t ecoObjKey_ ){

    auto jobSPtr = std::make_shared<Job>();
    jobSPtr->set_jobType( JobType::Create_Job_EcoObj );
    auto *paramPtr = jobSPtr->init_param<ArgBinary_Create_Job_EcoObj>();
    paramPtr->ecoObjKey = ecoObjKey_;
    //----------
    esrc::atom_push_back_2_jobQue( jobSPtr );
}


/* 简陋的方法，来预加载 ecoobjs，以便减少 chunk 进入 WaitForCreate 状态 的几率
 */
void  collect_ecoObjs_need_to_be_create(){

    //  并不是每次调用都执行
    //  而是每隔 n 秒执行一次
    if( eoc_inn::count != 0 ){
        eoc_inn::count--;
        return;
    }
    eoc_inn::count = 12 * 10; // 每隔 10 秒一次

    //---
    IntVec2 playerMPos = dpos_2_mpos(esrc::get_player().get_goRef().get_dpos());
    IntVec2 currentPlayerSectionMPos = anyMPos_2_sectionMPos( playerMPos );

    //---
    if( eoc_inn::isFirstCall ){
        eoc_inn::isFirstCall = false;
        eoc_inn::lastPlayerSectionMPos = currentPlayerSectionMPos;
    }

    //---
    if( eoc_inn::lastPlayerSectionMPos != currentPlayerSectionMPos ){
        eoc_inn::lastPlayerSectionMPos = currentPlayerSectionMPos;
        //--

        IntVec2         tmpSectionMPos {};
        sectionKey_t    ecoObjKey {};
        for( int h=-2; h<=2; h++ ){
            for( int w=-2; w<=2; w++ ){            
                tmpSectionMPos = currentPlayerSectionMPos + IntVec2{ w*ENTS_PER_SECTION<>, h*ENTS_PER_SECTION<> };
                ecoObjKey = sectionMPos_2_sectionKey( tmpSectionMPos );

                auto state = esrc::get_ecoObjMemState( ecoObjKey );
                switch (state){
                    case EcoObjMemState::OnCreating:
                    case EcoObjMemState::Active:
                        // do nothing
                        break;

                    case EcoObjMemState::NotExist:
                        //-- 正式将 此ecoObj 压入 job 线程
                        push_ecoObj_2_jobQue( ecoObjKey );
                        esrc::insert_ecoObjKey_2_onCreating( ecoObjKey );
                        break;
                
                    case EcoObjMemState::OnReleasing: 
                        // 周边 25个 ecoObj，居然有正在被释放的，
                        // 太不正常了，直接报错
                        tprAssert(0);
                        break;
                    
                    default:
                        tprAssert(0);
                        break;
                }                
            }
        }
    }
}





}//----------------- namespace: chunkCreate: end -------------------//

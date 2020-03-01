/*
 * ======================== WindAnim.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_COMONENT_WIND_ANIM_H
#define TPR_COMONENT_WIND_ANIM_H

//-------------------- CPP --------------------//
#include <vector>
#include <string>
#include <memory>


//-------------------- Engine --------------------//
#include "AnimActionEName.h"
#include "GameObjMesh.h"


namespace component{//------------- namespace component ----------------


// 辅助 go 实现 风吹动画  
// manual:
// -1- 在 具象go类 pvt class 中，包含本实例
// -2- 先调用 insert_goMesh(), 收集所有 参与风吹动画的 gomesh
// -3- 最后调用 init(); 彻底完成 初始化
// -4- each render frame, call updat()
//
class WindAnim{
public:

    class GoMeshData{
    public:        
        GoMeshData( GameObjMesh* goMeshPtr_,
                    size_t       windDelayStepCount_,
                    size_t       windDelayIdx_,
                    bool         isWindClockWorking_):
            goMeshPtr(goMeshPtr_),
            windDelayStepCount(windDelayStepCount_),
            windDelayIdx(windDelayIdx_),
            isWindClockWorking(isWindClockWorking_)
            {}

        //----- vals -----//
        GameObjMesh*    goMeshPtr;
        size_t          windDelayStepCount; // dynamicVal, count down
        size_t          windDelayIdx;       // for WindClock::get_playSpeedScale()
        bool            isWindClockWorking; 
    };

    //======= Self =======//
    WindAnim()=default;

    // first call
    inline void insert_goMesh( GameObjMesh* goMeshPtr_, size_t windDelayIdx_ )noexcept{
        std::unique_ptr<GoMeshData> uptr = std::make_unique<GoMeshData>( goMeshPtr_, 5, windDelayIdx_, false ); // 初始值 5，最简便的实现
        this->goMeshs.push_back( std::move(uptr));
    }

    // final call
    void init( const std::vector<AnimActionEName> *ActionEName_poolPtr_ )noexcept;

    void update()noexcept;


    // 可能要被改为 private ...
    inline AnimActionEName apply_a_windAnimActionEName()noexcept{
        this->windAnimActionEName_poolIdx++;
        if( this->windAnimActionEName_poolIdx >= this->ActionEName_poolPtr->size() ){
            this->windAnimActionEName_poolIdx = 0;
        }
        return this->ActionEName_poolPtr->at( this->windAnimActionEName_poolIdx );
    }


    //======= static =======//
    static inline size_t apply_a_goMesh_windDelayStep()noexcept{
        WindAnim::goMesh_windDelaySteps_poolIdx++;
        if( WindAnim::goMesh_windDelaySteps_poolIdx >= WindAnim::goMesh_windDelaySteps_pool.size() ){
            WindAnim::goMesh_windDelaySteps_poolIdx = 0;
        }
        return WindAnim::goMesh_windDelaySteps_pool.at(WindAnim::goMesh_windDelaySteps_poolIdx);
    }

    static inline size_t apply_a_go_windDelayStep()noexcept{
        WindAnim::go_windDelaySteps_poolIdx++;
        if( WindAnim::go_windDelaySteps_poolIdx >= WindAnim::go_windDelaySteps_pool.size() ){
            WindAnim::go_windDelaySteps_poolIdx = 0;
        }
        return WindAnim::go_windDelaySteps_pool.at(WindAnim::go_windDelaySteps_poolIdx);
    }

    static void init_for_static()noexcept; // MUST CALL IN MAIN !!! 

    
private:  

    std::unique_ptr<double> testUPtr {nullptr};

    size_t  windClockCheckStepCount {}; // count down
    size_t  oldWindClockCount {0};


    size_t  windAnimActionEName_poolIdx  {0};

    const std::vector<AnimActionEName> *ActionEName_poolPtr; 
                                        // 实际容器，由各 具象go类 实现
                                        // 仅提供 只读指针 到此

    std::vector<std::unique_ptr<GoMeshData>> goMeshs {}; // 参与 风吹动画的 gomeshPtr 集

    //======= static =======//
    static size_t goMesh_windDelaySteps_poolIdx;
    static size_t go_windDelaySteps_poolIdx;

    static std::vector<size_t> goMesh_windDelaySteps_pool;  // 1000 ents
    static std::vector<size_t> go_windDelaySteps_pool;      // 100 ents
   
};


}//------------- namespace component: end ----------------
#endif 


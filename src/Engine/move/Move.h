/*
 * ========================= Move.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_MOVE_H
#define TPR_MOVE_H

//-------------------- CPP --------------------//
#include <functional>
#include <memory>
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "functorTypes.h"
#include "SpeedLevel.h"
#include "MapCoord.h"
#include "DirAxes.h"

#include <iostream>
using std::cout;
using std::endl;

//-- need --
class GameObj;
class GameObjPos;
class Collision;


enum class MoveType : int {
    Crawl,   // often used for regular Go, with uniform speed [have speed upper limit]
    Drag,    // often used for regular Go, with uniform speed [have speed upper limit]
    Adsorb,  // often used for UIGo, with smooth speed change [no speed upper limit]
};

MoveType str_2_MoveType( const std::string name_ )noexcept;


class Move{
public:
    explicit Move( GameObj &goRef_ ):
        goRef(goRef_)
        {}

    inline void RenderUpdate()noexcept{
        this->renderUpdateFunc();
    }

    //------- flags -------//
    inline bool is_crawl()  const noexcept{ return (this->moveType==MoveType::Crawl); }
    inline bool is_drag()   const noexcept{ return (this->moveType==MoveType::Drag); }
    inline bool is_adsorb() const noexcept{ return (this->moveType==MoveType::Adsorb); }

    //------- set -------//
    inline void set_MoveType( MoveType type_ )noexcept{
        this->moveType = type_;
        switch ( type_ ){
            case MoveType::Crawl:      
                this->renderUpdateFunc = std::bind( &Move::renderUpdate_crawl, this ); 
                return;
            case MoveType::Drag: 
                this->renderUpdateFunc = std::bind( &Move::renderUpdate_drag, this ); 
                return;
            case MoveType::Adsorb: 
                this->renderUpdateFunc = std::bind( &Move::renderUpdate_adsorb, this ); 
                return;
            default:
                tprAssert(0);
                return;
        }
    }

    void set_newCrawlDirAxes( const DirAxes &newDirAxes_ );

    inline void set_drag_targetDPos( const glm::dvec2 &DPos_ )noexcept{
        tprAssert( this->is_drag() );
        if( DPos_ == this->targetDPos ){
            return;
        }
        this->targetDPos = DPos_;
        this->isMoving = true;
    }
    inline void set_adsorb_targetDPos( const glm::dvec2 &DPos_ )noexcept{
        tprAssert( this->is_adsorb() );
        if( DPos_ == this->targetDPos ){
            return;
        }
        this->targetDPos = DPos_;
        this->isMoving = true;
    }

    //------- get -------//
    inline bool get_isMoving()const noexcept{ return this->isMoving; }


private:
    void renderUpdate_crawl();
    void renderUpdate_drag();
    void renderUpdate_adsorb();

    //===== vals =====//
    GameObj      &goRef;

    MoveType    moveType  { MoveType::Crawl };

    
    DirAxes  newDirAxes {};  //- this renderFrame, new input val (move dir)
    DirAxes  oldDirAxes {};  //- last renderFrame, old input val (move dir)

                    // 也将被替换为 History 结构
                    // ...


    glm::dvec2 targetDPos  {-987654321.98765,-987654321.98765};
                                //- 设置一个不可能指向的 初始值。防止 第一次使用 set_drag_targetDPos() 时
                                //  就引发 targetDPos 初始值 等于 目标值。从而 drag 失效
                                //  一种很简陋，但有效的办法

    //------- functor -------//
    F_void renderUpdateFunc {nullptr}; //- 只在初始化阶段绑定，也许未来是可以切换的，但目前未实现

    //===== flags =====//
    bool    isMoving {false};

    //bool    isCrawlAnimActionStateDirty {false}; 
                        // only Crawl mode:
                        // 在本帧内，go.dir / move.moveSpeedLvl 任一值发生变化，此值为 true
                        // 即意味着，move aaction 可能要被切换了

                        // 最好放入 crawl 专用数据结构中 ....



};


#endif 


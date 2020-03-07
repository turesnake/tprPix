/*
 * ===================== GameObjMeshSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GAME_OBJ_MESH_SET_H
#define TPR_GAME_OBJ_MESH_SET_H


//-------------------- Engine --------------------//
#include "GameObjMesh.h"
#include "ShaderType.h"



class GameObjMeshSet{
public:
    using F_callback = std::function<void(GameObjMesh&)>;

    class Data{
    public:
        Data( std::unique_ptr<GameObjMesh> uptr_ ):
            goMesh(std::move(uptr_)),
            callback_inLastFrame(nullptr),
            callback_everyFrame(nullptr),
            goMeshPvtDataPtr( this->goMesh->get_animActionPvtDataPtr() )
            {}
        //------------//
        std::unique_ptr<GameObjMesh>    goMesh;
        F_callback                      callback_inLastFrame;
        F_callback                      callback_everyFrame;
        const AnimAction::PvtData       *goMeshPvtDataPtr;
    };


    GameObjMeshSet( GameObj &goRef_ ):
        goRef(goRef_)
        {}

    GameObjMesh &creat_new_goMesh(  
                            const std::string       &name_,
                            animSubspeciesId_t      subspeciesId_,
                            AnimActionEName         actionEName_,
                            RenderLayerType         layerType_,
                            ShaderType              shaderType_,
                            const glm::vec2         pposOff_ = glm::vec2{0.0,0.0},
                            double                  zOff_ = 0.0,
                            size_t                  uWeight = 1051, // 素数
                            bool                    isVisible_ = true
                        );


    // 暂时仅支持 特定位置的 callback
    inline void bind_goMesh_callback_inLastFrame( const std::string &name_, F_callback callback_ )noexcept{
        tprAssert( this->goMeshs.find(name_) != this->goMeshs.end() );
        this->goMeshs.at(name_)->callback_inLastFrame = callback_;
    }

    inline void bind_goMesh_callback_everyFrame( const std::string &name_, F_callback callback_ )noexcept{
        tprAssert( this->goMeshs.find(name_) != this->goMeshs.end() );
        this->goMeshs.at(name_)->callback_everyFrame = callback_;
    }
    


    inline void render_all_goMeshs_with_callback()noexcept{

        auto it = this->goMeshs.begin();
        while( it != this->goMeshs.end() ){
            auto &[name, dataUPtr] = *it;

            // 需要被删除的 gomesh，会在前一帧被设置 flag，
            // 并且在后一帧（本帧）查询 并正式删除    
            if( dataUPtr->goMesh->get_isNeedToBeErase() ){
                it = this->goMeshs.erase( it ); // saft way
                continue; // MUST
            }

            if( dataUPtr->callback_everyFrame ){
                dataUPtr->callback_everyFrame( *(dataUPtr->goMesh) );
            }

            if(dataUPtr->goMeshPvtDataPtr->isLastFrame){
                if( dataUPtr->callback_inLastFrame ){
                    dataUPtr->callback_inLastFrame( *(dataUPtr->goMesh) );
                }
            }
            dataUPtr->goMesh->RenderUpdate_auto();
            //===//
            it++; // MUST
        }
    }
    

    // 即便设置了 callback. 也不会被执行
    // 也不执行自动 erase
    inline void render_all_goMeshs_without_callback()noexcept{
        for( auto &[name, dataUPtr] : this->goMeshs ){
            dataUPtr->goMesh->RenderUpdate_auto();
        }
    }
    
    inline void render_all_groundGoMesh()noexcept{
        for( auto &[name, dataUPtr] : this->goMeshs ){
            dataUPtr->goMesh->RenderUpdate_in_fast_way();
        }
    }
    inline void render_all_floorGoMesh()noexcept{
        for( auto &[name, dataUPtr] : this->goMeshs ){
            dataUPtr->goMesh->RenderUpdate_in_fast_way();
        }
    }


    inline GameObjMesh &get_goMeshRef( const std::string &name_ )noexcept{
        tprAssert( this->goMeshs.find(name_) != this->goMeshs.end() );
        return *(this->goMeshs.at(name_)->goMesh);
    }

    inline std::unordered_map<std::string, std::unique_ptr<GameObjMeshSet::Data>> &get_goMeshs()noexcept{ return this->goMeshs; }


private:
    GameObj &goRef;

    // - rootGoMesh  -- name = “root”; 核心goMesh;
    // - childGoMesh -- 剩下的goMesh，名字随意
    std::unordered_map<std::string, std::unique_ptr<GameObjMeshSet::Data>>  goMeshs {};

                            //- go实例 与 GoMesh实例 强关联
                            // 大部分go不会卸载／增加自己的 GoMesh实例
                            //- 在一个 具象go类实例 的创建过程中，会把特定的 GoMesh实例 存入此容器
                            //- 只存储在 mem态。 在go实例存入 硬盘时，GoMesh实例会被丢弃
                            //- 等再次从section 加载时，再根据 具象go类型，生成新的 GoMesh实例。




};






#endif 


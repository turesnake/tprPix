/*
 * ======================= Job_Field.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Job_Field.h"

//-------------------- Libs --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "config.h"
#include "tprMath.h"
#include "tprCast.h"
#include "GoSpecFromJson.h"

#include "esrc_coordinate.h"
#include "esrc_animFrameSet.h"

#include "Job_Chunk.h"


namespace jobF_inn {//----------- namespace: jobF_inn ----------------//


    //-- 按照象限，遍历 Job_Field::mapEntPtrs 用 --
    const std::vector<std::vector<IntVec2>> whs{
        std::vector<IntVec2>{
            IntVec2{ 0, 0 },
            IntVec2{ 1, 0 },
            IntVec2{ 0, 1 },
            IntVec2{ 1, 1 } // halfField bind mapent
        },
        std::vector<IntVec2>{
            IntVec2{ 2, 0 },
            IntVec2{ 3, 0 },
            IntVec2{ 2, 1 },
            IntVec2{ 3, 1 } // halfField bind mapent
        },
        std::vector<IntVec2>{
            IntVec2{ 0, 2 },
            IntVec2{ 1, 2 },
            IntVec2{ 0, 3 },
            IntVec2{ 1, 3 } // halfField bind mapent
        },
        std::vector<IntVec2>{
            IntVec2{ 2, 2 },
            IntVec2{ 3, 2 },
            IntVec2{ 2, 3 },
            IntVec2{ 3, 3 } // halfField bind mapent
        }
    };

    std::vector<glm::dvec2> halfFieldDposOffs {};
    std::vector<glm::dvec2> mapentDposOffs {};

}//-------------- namespace: jobF_inn end ----------------//

//===== static =====//
bool Job_Field::isStaticInit {false};


void Job_Field::init_for_static()noexcept{

    const auto &worldCoorldRef = esrc::get_worldCoordRef();

    const std::vector<glm::dvec2> origin_halfFieldDposOffs{
        glm::dvec2{ -static_cast<double>(PIXES_PER_MAPENT),
                    -static_cast<double>(PIXES_PER_MAPENT) },// 0

        glm::dvec2{  static_cast<double>(PIXES_PER_MAPENT),
                    -static_cast<double>(PIXES_PER_MAPENT) },// 1

        glm::dvec2{ -static_cast<double>(PIXES_PER_MAPENT),
                     static_cast<double>(PIXES_PER_MAPENT) },// 2

        glm::dvec2{  static_cast<double>(PIXES_PER_MAPENT),
                     static_cast<double>(PIXES_PER_MAPENT) }// 3
    };

    for( const auto &off : origin_halfFieldDposOffs ){
        jobF_inn::halfFieldDposOffs.push_back( worldCoorldRef.calc_outDPos( off ) );
    }

    //-----
    const std::vector<glm::dvec2> origin_mapentDposOffs{
        glm::dvec2{ -static_cast<double>(HALF_PIXES_PER_MAPENT),
                    -static_cast<double>(HALF_PIXES_PER_MAPENT) },// 0

        glm::dvec2{  static_cast<double>(HALF_PIXES_PER_MAPENT),
                    -static_cast<double>(HALF_PIXES_PER_MAPENT) },// 1

        glm::dvec2{ -static_cast<double>(HALF_PIXES_PER_MAPENT),
                     static_cast<double>(HALF_PIXES_PER_MAPENT) },// 2

        glm::dvec2{  static_cast<double>(HALF_PIXES_PER_MAPENT),
                     static_cast<double>(HALF_PIXES_PER_MAPENT) }// 3
    };

    for( const auto &off : origin_mapentDposOffs ){
        jobF_inn::mapentDposOffs.push_back( worldCoorldRef.calc_outDPos( off ) );
    }
}


// 自动检测 4*4 容器，通过 分形思路，分配 groundGo 实例
void Job_Field::apply_job_groundGoEnts()noexcept{

    glm::dvec2          dposOff     {}; // base on field-midDPos
    colorTableId_t      colorTableId {};
    size_t              uWeight {};
    size_t              halfEntIdx { cast_2_size_t(HALF_ENTS_PER_FIELD) };
    const Job_MapEnt    *entPtr {nullptr}; 

    std::vector<std::unique_ptr<Job_GroundGoEnt>> groundGoEnts {};

    //-- field 整个单色 --//
    tprAssert( !this->fields.empty() );
    if( this->fields.size() == 1 ){

        entPtr = this->mapEntPtrs.at(halfEntIdx).at(halfEntIdx);

        dposOff = glm::dvec2{0.0, 0.0};
        colorTableId = *this->fields.begin();
        uWeight = entPtr->uWeight;

        ( this->isHaveBorderEnt ) ?
            groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::Field,       dposOff, colorTableId, uWeight )) :
            groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::SimpleField, dposOff, colorTableId, uWeight ));
        
    }else{

        //--- 现在，field 是一定要被拆分的了 ---
        // 执行一次 自底向上 的递归，
        // 遍历每个 象限的 4mapent，如何能合并，就合并, 并当场制作 1个 Job_GroundGoEnt::halfField
        // 如果不能合并，当场制作 4个 Job_GroundGoEnt::mapent
        size_t innIdx {}; // hlafField 内的 局部idx

        for( size_t hIdx=0; hIdx<this->halfFields.size(); hIdx++ ){// 4-halfField

            const auto &halfWHs     = jobF_inn::whs.at(hIdx);
            const auto &halfDposOff = jobF_inn::halfFieldDposOffs.at(hIdx);

            auto &halfSetRef = this->halfFields.at(hIdx);
            tprAssert( !halfSetRef.empty() );
            if( halfSetRef.size() == 1 ){

                const auto &wh = halfWHs.back(); 
                entPtr = this->mapEntPtrs.at( static_cast<size_t>(wh.y)).at( static_cast<size_t>(wh.x));
                dposOff = halfDposOff;
                colorTableId = entPtr->colorTableId;
                uWeight = entPtr->uWeight;

                groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::HalfField, dposOff, colorTableId, uWeight ));
                continue; // !!!
            }

            //--- 需要将 halfField 拆分为 4-mapent ---
            for( int h=0; h<HALF_ENTS_PER_FIELD; h++ ){
                for( int w=0; w<HALF_ENTS_PER_FIELD; w++ ){ // each mapent in halfField

                    innIdx = cast_2_size_t( h * HALF_ENTS_PER_FIELD + w );
                    const auto &wh = halfWHs.at(innIdx);
                    entPtr = this->mapEntPtrs.at(static_cast<size_t>(wh.y)).at(static_cast<size_t>(wh.x));

                    dposOff = halfDposOff + jobF_inn::mapentDposOffs.at(innIdx);
                    colorTableId = entPtr->colorTableId;
                    uWeight = entPtr->uWeight;

                    groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::MapEnt, dposOff, colorTableId, uWeight ));
                }
            }
        }
    }

    //-------------------------------------//
    // 根据完成的 groundGoEnts 手动组装 groundGoData
    //-------------------------------------//
    this->assemble_groundGoData( groundGoEnts );

}



extern const std::string &calc_groundGoMeshName( GroundGoEntType groundType_, colorTableId_t colorTableId_ )noexcept;


// 在 job 线程中，手动装配一个 groundGo 的 GoDataForCreate 数据
// 丑陋，但能统一 “create go” 函数接口
void Job_Field::assemble_groundGoData( const std::vector<std::unique_ptr<Job_GroundGoEnt>> &groundGoEnts_ )noexcept{

    IntVec2 fieldMPos = fieldKey_2_mpos(this->fieldKey);
    double halfPixesPerField = static_cast<double>(PIXES_PER_FIELD) * 0.5;
    //===
    this->groundGoData = std::make_unique<GoDataForCreate>();
    GoDataForCreate *goDataPtr = this->groundGoData.get();
    //---
    goDataPtr->goSpeciesId = GoSpecFromJson::str_2_goSpeciesId("groundGo"),
    goDataPtr->goLabelId = GoAssemblePlanSet::str_2_goLabelId(""); // 其实是错的，GroundGo 压根就没有 asm 分配方案
    goDataPtr->dpos = mpos_2_dpos(fieldMPos) + glm::dvec2{ halfPixesPerField, halfPixesPerField };
    goDataPtr->direction = NineDirection::Center;
    goDataPtr->brokenLvl = BrokenLvl::Lvl_0;
    goDataPtr->uWeight = this->mapEntPtrs.at(0).at(0)->uWeight; // 暂时等于 左下角 mapent.uWeight

    // find plan
    const GoSpecFromJson &goSpecFromJson = GoSpecFromJson::get_goSpecFromJsonRef( goDataPtr->goSpeciesId );
    tprAssert( goSpecFromJson.goAssemblePlanSetUPtr );
    const GoAssemblePlanSet::Plan &planRef = goSpecFromJson.goAssemblePlanSetUPtr->apply_a_plan( goDataPtr->goLabelId, goDataPtr->uWeight );

    //---
    goDataPtr->goAltiRangeLabel = planRef.goAltiRangeLabel;
    goDataPtr->colliDataFromJsonPtr = planRef.colliDataFromJsonUPtr.get();

    //=== goMeshs
    // 根据 groundGoEnts 数据，动态创建
    size_t goMeshIdx {0};
    std::string specialGoMeshName {};
    size_t randUWeightOff = goDataPtr->uWeight;
    for( const auto &uptr : groundGoEnts_ ){
        const Job_GroundGoEnt &job_groundGoEntRef = *uptr;
        randUWeightOff += 17;

        std::string goMeshName = calc_groundGoMeshName( job_groundGoEntRef.groundType, job_groundGoEntRef.colorTableId );
                    //cout << "goMeshName: " << goMeshName << endl;

        const GoAssemblePlanSet::GoMeshEnt &gmeRef = planRef.get_goMeshEntRef( goMeshName );

        // 手动生成 goMeshName, 防止重名
        if( goMeshIdx == 0 ){
            specialGoMeshName = "root";
        }else{
            specialGoMeshName = tprGeneral::nameString_combine("m_", goMeshIdx, "");
        }

        auto goMeshUPtr = std::make_unique<GoDataForCreate::GoMeshByHand>( &gmeRef, randUWeightOff );
        //---
        goMeshUPtr->goMeshName = specialGoMeshName;
        goMeshUPtr->dposOff = job_groundGoEntRef.dposOff;
        goMeshUPtr->zOff = calc_uWeight_fractValue(randUWeightOff); // (0.0, 1.0)

        goMeshUPtr->isVisible = true;
        goMeshUPtr->isAutoInit = true; // meaningless
    
        goMeshUPtr->set_windDelayIdx( 1 ); // meaningless
    
        //---
        goDataPtr->goMeshEntUPtrs.push_back( std::move( goMeshUPtr ) );
        //===
        goMeshIdx++;
    }
}





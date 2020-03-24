/*
 * ======================= Job_Field.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Job_Field.h"

//-------------------- Engine --------------------//
#include "GoSpecFromJson.h"
#include "Job_GroundGoEnt.h"

#include "esrc_state.h"
#include "esrc_coordinate.h"
#include "esrc_animFrameSet.h"

#include "Job_Chunk.h"


//-------------------- Scrit --------------------//
#include "Script/gameObjs/bioSoup/BioSoupDataForCreate.h"



namespace jobF_inn {//----------- namespace: jobF_inn ----------------//

    // 以下这种直接标记 数字的方式 很不安全
    // 当 ENTS_PER_FIELD / HALF_ENTS_PER_FIELD 发生改变时，就会失效

    //-- 按照象限，遍历 Job_Field::mapEntPtrs 用 --
    const std::vector<std::vector<IntVec2>> whs{
        std::vector<IntVec2>{
            IntVec2{ 0, 0 },
            IntVec2{ 1, 0 },
            IntVec2{ 0, 1 },
            IntVec2{ 1, 1 } // halfField in left-bottom
        },
        std::vector<IntVec2>{
            IntVec2{ 2, 0 },
            IntVec2{ 3, 0 },
            IntVec2{ 2, 1 },
            IntVec2{ 3, 1 } // halfField in right-bottom
        },
        std::vector<IntVec2>{
            IntVec2{ 0, 2 },
            IntVec2{ 1, 2 },
            IntVec2{ 0, 3 },
            IntVec2{ 1, 3 } // halfField in left-top
        },
        std::vector<IntVec2>{
            IntVec2{ 2, 2 },
            IntVec2{ 3, 2 },
            IntVec2{ 2, 3 },
            IntVec2{ 3, 3 } // halfField in right-top
        }
    };

    std::vector<glm::dvec2> halfFieldDposOffs {}; // num:4
    std::vector<glm::dvec2> mapentDposOffs {}; // num:4


    //=============================//
    //        bioSoup    [tmp]
    std::map<gameObjs::bioSoup::State, std::map<FieldFractType, goLabelId_t>> bioSoup_goLabelIds {};


}//-------------- namespace: jobF_inn end ----------------//



// [*main-thread*]
void Job_Field::init_for_static()noexcept{

    tprAssert( ((ENTS_PER_FIELD<>)==4) && ((HALF_ENTS_PER_FIELD<>)==2) ); // MUST
    tprAssert( esrc::is_setState("gameObj") );
    
    //=====//
    const auto &worldCoorldRef = esrc::get_worldCoordRef();

    const std::vector<glm::dvec2> origin_halfFieldDposOffs{
        glm::dvec2{ -PIXES_PER_MAPENT_D, -PIXES_PER_MAPENT_D },// 0
        glm::dvec2{  PIXES_PER_MAPENT_D, -PIXES_PER_MAPENT_D },// 1
        glm::dvec2{ -PIXES_PER_MAPENT_D,  PIXES_PER_MAPENT_D },// 2
        glm::dvec2{  PIXES_PER_MAPENT_D,  PIXES_PER_MAPENT_D }// 3
    };

    for( const auto &off : origin_halfFieldDposOffs ){
        jobF_inn::halfFieldDposOffs.push_back( worldCoorldRef.calc_outDPos( off ) );
    }

    //-----
    const std::vector<glm::dvec2> origin_mapentDposOffs{
        glm::dvec2{ -HALF_PIXES_PER_MAPENT_D, -HALF_PIXES_PER_MAPENT_D },// 0
        glm::dvec2{  HALF_PIXES_PER_MAPENT_D, -HALF_PIXES_PER_MAPENT_D },// 1
        glm::dvec2{ -HALF_PIXES_PER_MAPENT_D,  HALF_PIXES_PER_MAPENT_D },// 2
        glm::dvec2{  HALF_PIXES_PER_MAPENT_D,  HALF_PIXES_PER_MAPENT_D }// 3
    };

    for( const auto &off : origin_mapentDposOffs ){
        jobF_inn::mapentDposOffs.push_back( worldCoorldRef.calc_outDPos( off ) );
    }

    //---------------//
    //    bioSoup [tmp]
    jobF_inn::bioSoup_goLabelIds.insert({
        gameObjs::bioSoup::State::Active,
        std::map<FieldFractType, goLabelId_t>{
            { FieldFractType::MapEnt,       GoAssemblePlanSet::str_2_goLabelId("MapEnt_1m1") },
            { FieldFractType::HalfField,    GoAssemblePlanSet::str_2_goLabelId("MapEnt_2m2") },
            { FieldFractType::Field,        GoAssemblePlanSet::str_2_goLabelId("MapEnt_4m4") }
        }
    });

    jobF_inn::bioSoup_goLabelIds.insert({
        gameObjs::bioSoup::State::Inertia,
        std::map<FieldFractType, goLabelId_t>{
            { FieldFractType::MapEnt,       GoAssemblePlanSet::str_2_goLabelId("MapEnt_1m1_Intertia") },
            { FieldFractType::HalfField,    GoAssemblePlanSet::str_2_goLabelId("MapEnt_2m2_Intertia") },
            { FieldFractType::Field,        GoAssemblePlanSet::str_2_goLabelId("MapEnt_4m4_Intertia") }
        }
    });

}



// 自动检测 4*4 容器，通过 分形思路，分配 bioSoup 数据实例
void Job_Field::apply_bioSoupEnts(){

    size_t                      halfEntIdx { HALF_ENTS_PER_FIELD<size_t> };
    const Job_MapEnt            *entPtr {nullptr}; 

    IntVec2 fieldMPos = fieldKey_2_mpos(this->fieldKey);

    //-- field 整个单色 --//
    size_t inFieldSize = this->bioSoupFract.get_inField_size();
    tprAssert( inFieldSize != 0 );
    if( inFieldSize == 1 ){

        entPtr = this->mapEntPtrs.at(halfEntIdx).at(halfEntIdx);
        auto bioSoupState = entPtr->get_bioSoupState();

        this->create_bioSoupDataUPtr(   FieldFractType::Field, 
                                        fieldMPos + IntVec2{ HALF_ENTS_PER_FIELD<>, HALF_ENTS_PER_FIELD<> },
                                        bioSoupState,
                                        entPtr->get_alti()
                                    );   

        if( bioSoupState == gameObjs::bioSoup::State::Inertia ){
            this->isCoveredBy_InertiaBioSoup = true;
        }

    }else{

        //--- 现在，field 是一定要被拆分的了 ---
        // 执行一次 自底向上 的递归，
        // 遍历每个 象限的 4mapent，如果能合并，就合并, 并当场制作 1个 Job_GroundGoEnt::halfField
        // 如果不能合并，当场制作 4个 Job_GroundGoEnt::mapent
        size_t innIdx {}; // hlafField 内的 局部idx

        const auto &inHalfFields = this->bioSoupFract.get_inHalfFields();
        for( size_t hIdx=0; hIdx<inHalfFields.size(); hIdx++ ){

            const auto &halfWHs     = jobF_inn::whs.at(hIdx);

            auto &halfSetRef = inHalfFields.at(hIdx);
            tprAssert( !halfSetRef.empty() );
            if( halfSetRef.size() == 1 ){

                const auto &wh = halfWHs.back(); // 静态数据，无需担心 back 会访问空
                //const auto &wh = *halfWHs.begin();
                entPtr = this->mapEntPtrs.at( static_cast<size_t>(wh.y)).at( static_cast<size_t>(wh.x));

                this->create_bioSoupDataUPtr(   FieldFractType::HalfField, 
                                                fieldMPos + wh,
                                                entPtr->get_bioSoupState(),
                                                entPtr->get_alti()
                                            ); 
                continue; // !!!
            }

            //--- 需要将 halfField 拆分为 4-mapent ---
            for( size_t h=0; h<HALF_ENTS_PER_FIELD<size_t>; h++ ){
                for( size_t w=0; w<HALF_ENTS_PER_FIELD<size_t>; w++ ){ // each mapent in halfField

                    innIdx = h * HALF_ENTS_PER_FIELD<size_t> + w;
                    const auto &wh = halfWHs.at(innIdx);
                    entPtr = this->mapEntPtrs.at(static_cast<size_t>(wh.y)).at(static_cast<size_t>(wh.x));

                    this->create_bioSoupDataUPtr(   FieldFractType::MapEnt, 
                                                    fieldMPos + wh,
                                                    entPtr->get_bioSoupState(),
                                                    entPtr->get_alti()
                                                ); 
                }
            }
        }
    }

    this->copy_bioSoupGoDataPtrs();
}



void Job_Field::create_bioSoupDataUPtr( FieldFractType fieldFractType_,
                                        IntVec2 mpos_, 
                                        gameObjs::bioSoup::State bioSoupState_,
                                        MapAltitude mapEntAlti_ ){

    if( bioSoupState_ == gameObjs::bioSoup::State::NotExist ){
        return;
    }


    goLabelId_t goLabelId = jobF_inn::bioSoup_goLabelIds.at(bioSoupState_).at(fieldFractType_);

    auto goDUPtr = GoDataForCreate::create_new_goDataForCreate(  
                                                            mpos_,
                                                            mpos_2_midDPos(mpos_), // tmp
                                                            GoSpecFromJson::str_2_goSpeciesId("bioSoup"),
                                                            goLabelId,
                                                            NineDirection::Center,
                                                            BrokenLvl::Lvl_0
                                                            );

    // custumized data
    auto *bioSoupDPtr = goDUPtr->get_binary().init<gameObjs::bioSoup::DataForCreate>();
    bioSoupDPtr->bioSoupState = bioSoupState_;
    bioSoupDPtr->mapEntAlti = mapEntAlti_;

    this->bioSoupGoDatas.push_back( std::move(goDUPtr) );
}





// 自动检测 4*4 容器，通过 分形思路，分配 groundGo 数据实例
void Job_Field::apply_groundGoEnts(){

    if( this->isCoveredBy_InertiaBioSoup ){
        return;
    }

    //======================================//
    glm::dvec2          dposOff     {}; // base on field-midDPos
    colorTableId_t      colorTableId {};
    size_t              uWeight {};
    size_t              halfEntIdx { HALF_ENTS_PER_FIELD<size_t> };
    const Job_MapEnt    *entPtr {nullptr}; 

    std::vector<std::unique_ptr<Job_GroundGoEnt>> groundGoEnts {};

    //-- field 整个单色 --//
    size_t inFieldSize = this->colorTableIdFract.get_inField_size();
    tprAssert( inFieldSize != 0 );
    if( inFieldSize == 1 ){

        entPtr = this->mapEntPtrs.at(halfEntIdx).at(halfEntIdx);

        dposOff = glm::dvec2{0.0, 0.0};
        colorTableId = *(this->colorTableIdFract.get_inField().begin());
        uWeight = entPtr->get_uWeight();

        // 不再区分 isHaveBorderEnt
        groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( FieldFractType::Field, dposOff, colorTableId, uWeight ));
        
    }else{

        //--- 现在，field 是一定要被拆分的了 ---
        // 执行一次 自底向上 的递归，
        // 遍历每个 象限的 4mapent，如果能合并，就合并, 并当场制作 1个 Job_GroundGoEnt::halfField
        // 如果不能合并，当场制作 4个 Job_GroundGoEnt::mapent
        size_t innIdx {}; // hlafField 内的 局部idx

        const auto &inHalfFields = this->colorTableIdFract.get_inHalfFields();
        for( size_t hIdx=0; hIdx<inHalfFields.size(); hIdx++ ){

            const auto &halfWHs     = jobF_inn::whs.at(hIdx);
            const auto &halfDposOff = jobF_inn::halfFieldDposOffs.at(hIdx);

            auto &halfSetRef = inHalfFields.at(hIdx);
            tprAssert( !halfSetRef.empty() );
            if( halfSetRef.size() == 1 ){

                const auto &wh = halfWHs.back(); // 静态数据，无需担心 back 会访问空
                //const auto &wh = *halfWHs.begin();
                entPtr = this->mapEntPtrs.at( static_cast<size_t>(wh.y)).at( static_cast<size_t>(wh.x));
                dposOff = halfDposOff;
                colorTableId = entPtr->get_colorTableId();
                uWeight = entPtr->get_uWeight();

                groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( FieldFractType::HalfField, dposOff, colorTableId, uWeight ));
                continue; // !!!
            }

            //--- 需要将 halfField 拆分为 4-mapent ---
            for( size_t h=0; h<HALF_ENTS_PER_FIELD<size_t>; h++ ){
                for( size_t w=0; w<HALF_ENTS_PER_FIELD<size_t>; w++ ){ // each mapent in halfField

                    innIdx = h * HALF_ENTS_PER_FIELD<size_t> + w;
                    const auto &wh = halfWHs.at(innIdx);
                    entPtr = this->mapEntPtrs.at(static_cast<size_t>(wh.y)).at(static_cast<size_t>(wh.x));

                    dposOff = halfDposOff + jobF_inn::mapentDposOffs.at(innIdx);
                    colorTableId = entPtr->get_colorTableId();
                    uWeight = entPtr->get_uWeight();

                    groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( FieldFractType::MapEnt, dposOff, colorTableId, uWeight ));
                }
            }
        }
    }

    //-------------------------------------//
    // 根据完成的 groundGoEnts 手动组装 groundGoData
    //-------------------------------------//
    this->groundGoData = GoDataForCreate::create_new_groundGoDataForCreate( *this, groundGoEnts );
}




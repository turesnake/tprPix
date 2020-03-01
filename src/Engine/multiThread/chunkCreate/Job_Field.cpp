/*
 * ======================= Job_Field.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Job_Field.h"

//-------------------- Engine --------------------//
#include "config.h"
#include "tprMath.h"
#include "tprCast.h"
#include "GoSpecFromJson.h"
#include "Job_GroundGoEnt.h"

#include "esrc_state.h"
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


    //const std::vector<>




    goLabelId_t bioSoupLabel_1m1 {};
    goLabelId_t bioSoupLabel_2m2 {};
    goLabelId_t bioSoupLabel_4m4 {};

}//-------------- namespace: jobF_inn end ----------------//



// [*main-thread*]
void Job_Field::init_for_static()noexcept{

    tprAssert( esrc::is_setState("gameObj") );
    
    //=====//
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

    //---------------//
    jobF_inn::bioSoupLabel_1m1 = GoAssemblePlanSet::str_2_goLabelId("MapEnt_1m1");
    jobF_inn::bioSoupLabel_2m2 = GoAssemblePlanSet::str_2_goLabelId("MapEnt_2m2");
    jobF_inn::bioSoupLabel_4m4 = GoAssemblePlanSet::str_2_goLabelId("MapEnt_4m4");
}



// 自动检测 4*4 容器，通过 分形思路，分配 bioSoup 数据实例
void Job_Field::apply_bioSoupEnts(){

    //glm::dvec2          dposOff     {}; // base on field-midDPos
    IntVec2             mpos        {};
    BioSoupState        bioSoupState {};
    size_t              halfEntIdx { cast_2_size_t(HALF_ENTS_PER_FIELD) };
    const Job_MapEnt    *entPtr {nullptr}; 

    //-- field 整个单色 --//
    size_t inFieldSize = this->bioSoupFract.get_inField_size();
    tprAssert( inFieldSize != 0 );
    if( inFieldSize == 1 ){

        entPtr = this->mapEntPtrs.at(halfEntIdx).at(halfEntIdx);

        mpos = fieldKey_2_mpos(this->fieldKey) + IntVec2{ HALF_ENTS_PER_FIELD, HALF_ENTS_PER_FIELD };

        //dposOff = glm::dvec2{0.0, 0.0};
        bioSoupState = *(this->bioSoupFract.get_inField().begin());

        if( bioSoupState == BioSoupState::Active ){

            this->create_bioSoupDataUPtr( mpos, jobF_inn::bioSoupLabel_4m4 );
                                        // 未来要表达出 “激活态”

        }else if( bioSoupState == BioSoupState::Inertia ){

            this->create_bioSoupDataUPtr( mpos, jobF_inn::bioSoupLabel_4m4 );
                                        // 未来要表达出 “惰性”

        }else{
            // do nothing
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
            //const auto &halfDposOff = jobF_inn::halfFieldDposOffs.at(hIdx);

            auto &halfSetRef = inHalfFields.at(hIdx);
            tprAssert( !halfSetRef.empty() );
            if( halfSetRef.size() == 1 ){

                const auto &wh = halfWHs.back(); 
                entPtr = this->mapEntPtrs.at( static_cast<size_t>(wh.y)).at( static_cast<size_t>(wh.x));
                //dposOff = halfDposOff;
                bioSoupState = entPtr->get_bioSoupState();



                //groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::HalfField, dposOff, colorTableId, uWeight ));


                continue; // !!!
            }

            //--- 需要将 halfField 拆分为 4-mapent ---
            for( int h=0; h<HALF_ENTS_PER_FIELD; h++ ){
                for( int w=0; w<HALF_ENTS_PER_FIELD; w++ ){ // each mapent in halfField

                    innIdx = cast_2_size_t( h * HALF_ENTS_PER_FIELD + w );
                    const auto &wh = halfWHs.at(innIdx);
                    entPtr = this->mapEntPtrs.at(static_cast<size_t>(wh.y)).at(static_cast<size_t>(wh.x));

                    //dposOff = halfDposOff + jobF_inn::mapentDposOffs.at(innIdx);
                    bioSoupState = entPtr->get_bioSoupState();
               

                    //groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::MapEnt, dposOff, colorTableId, uWeight ));




                }
            }
        }
    }

}



void Job_Field::create_bioSoupDataUPtr( IntVec2 mpos_, goLabelId_t goLabelId_ ){

    auto uptr = GoDataForCreate::create_new_goDataForCreate(  
                                                            mpos_,
                                                            mpos_2_midDPos(mpos_), // tmp
                                                            GoSpecFromJson::str_2_goSpeciesId("bioSoup"),
                                                            goLabelId_,
                                                            NineDirection::Center,
                                                            BrokenLvl::Lvl_0
                                                            );
    this->bioSoupGoDatas.push_back( std::move(uptr) );
}





// 自动检测 4*4 容器，通过 分形思路，分配 groundGo 数据实例
void Job_Field::apply_groundGoEnts(){

    glm::dvec2          dposOff     {}; // base on field-midDPos
    colorTableId_t      colorTableId {};
    size_t              uWeight {};
    size_t              halfEntIdx { cast_2_size_t(HALF_ENTS_PER_FIELD) };
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

        ( this->isHaveBorderEnt ) ?
            groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::Field,       dposOff, colorTableId, uWeight )) :
            groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::SimpleField, dposOff, colorTableId, uWeight ));
        
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

                const auto &wh = halfWHs.back(); 
                entPtr = this->mapEntPtrs.at( static_cast<size_t>(wh.y)).at( static_cast<size_t>(wh.x));
                dposOff = halfDposOff;
                colorTableId = entPtr->get_colorTableId();
                uWeight = entPtr->get_uWeight();

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
                    colorTableId = entPtr->get_colorTableId();
                    uWeight = entPtr->get_uWeight();

                    groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::MapEnt, dposOff, colorTableId, uWeight ));
                }
            }
        }
    }

    //-------------------------------------//
    // 根据完成的 groundGoEnts 手动组装 groundGoData
    //-------------------------------------//
    this->groundGoData = GoDataForCreate::create_new_groundGoDataForCreate( *this, groundGoEnts );
}




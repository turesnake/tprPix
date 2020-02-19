/*
 * ======================= Job_Field.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Job_Field.h"

//-------------------- Engine --------------------//s
#include "esrc_coordinate.h"
#include "tprCast.h"

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



void Job_Field::bind_functors( Job_Chunk &jChunkRef_ )noexcept{
    this->getnc_mapEntUWeight = [&jChunkRef_]( IntVec2 mposOff_ ){
        return jChunkRef_.getnc_mapEntInnRef( mposOff_ ).uWeight;
    };
}



// 自动检测 4*4 容器，通过 分形思路，分配 groundGo 实例
void Job_Field::apply_job_groundGoEnts()noexcept{

    glm::dvec2      dposOff     {}; // base on field-midDPos
    colorTableId_t  colorTableId {};
    size_t          uWeight {};

    const Job_MapEnt *entPtr {nullptr}; 

    //-- field 整个单色 --//
    tprAssert( !this->fields.empty() );
    if( this->fields.size() == 1 ){

        entPtr = this->mapEntPtrs.at(cast_2_size_t(HALF_ENTS_PER_FIELD)).at(cast_2_size_t(HALF_ENTS_PER_FIELD));

        dposOff = glm::dvec2{0.0, 0.0};
        //ecoObjKey = *this->fields.begin();
        colorTableId = *this->fields.begin();
        uWeight = entPtr->uWeight;

        ( this->isHaveBorderEnt ) ?
            this->groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::Field,       dposOff, colorTableId, uWeight )) :
            this->groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::SimpleField, dposOff, colorTableId, uWeight ));
        
        return; // !!!
    }

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
            //ecoObjKey = entPtr->ecoObjKey;
            colorTableId = entPtr->colorTableId;
            uWeight = entPtr->uWeight;

            this->groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::HalfField, dposOff, colorTableId, uWeight ));
            continue; // !!!
        }

        //--- 需要将 halfField 拆分为 4-mapent ---
        for( int h=0; h<HALF_ENTS_PER_FIELD; h++ ){
            for( int w=0; w<HALF_ENTS_PER_FIELD; w++ ){ // each mapent in halfField

                innIdx = cast_2_size_t( h * HALF_ENTS_PER_FIELD + w );
                const auto &wh = halfWHs.at(innIdx);
                entPtr = this->mapEntPtrs.at(static_cast<size_t>(wh.y)).at(static_cast<size_t>(wh.x));

                dposOff = halfDposOff + jobF_inn::mapentDposOffs.at(innIdx);
                //ecoObjKey = entPtr->ecoObjKey;
                colorTableId = entPtr->colorTableId;
                uWeight = entPtr->uWeight;

                this->groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::MapEnt, dposOff, colorTableId, uWeight ));
            }
        }
    }
}





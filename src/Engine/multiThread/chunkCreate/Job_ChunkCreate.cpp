/*
 * ========================== Job_ChunkCreate.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Job_ChunkCreate.h"


//-------------------- Engine --------------------//
#include "simplexNoise.h"
#include "MapEnt.h"
#include "random.h"

#include "esrc_gameSeed.h"


namespace job_cc_inn {//----------- namespace: job_cc_inn ----------------//

    //- 在未来，freq 这组值也会收到 ecosys 影响 --
    const double freqSeaLvl { 0.05 };
    const double freqBig    { 0.4 };
    const double freqMid    { 1.6 };
    const double freqSml    { 4.0 };

    const glm::dvec2  worldCenter { 0.0, 0.0 };


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

    const std::vector<glm::dvec2> halfFieldDposOffs{
        glm::dvec2{ -static_cast<double>(PIXES_PER_MAPENT),
                    -static_cast<double>(PIXES_PER_MAPENT) },// 0

        glm::dvec2{  static_cast<double>(PIXES_PER_MAPENT),
                    -static_cast<double>(PIXES_PER_MAPENT) },// 1

        glm::dvec2{ -static_cast<double>(PIXES_PER_MAPENT),
                     static_cast<double>(PIXES_PER_MAPENT) },// 2

        glm::dvec2{  static_cast<double>(PIXES_PER_MAPENT),
                     static_cast<double>(PIXES_PER_MAPENT) }// 3
    };

    const std::vector<glm::dvec2> mapentDposOffs{
        glm::dvec2{ -static_cast<double>(HALF_PIXES_PER_MAPENT),
                    -static_cast<double>(HALF_PIXES_PER_MAPENT) },// 0

        glm::dvec2{  static_cast<double>(HALF_PIXES_PER_MAPENT),
                    -static_cast<double>(HALF_PIXES_PER_MAPENT) },// 1

        glm::dvec2{ -static_cast<double>(HALF_PIXES_PER_MAPENT),
                     static_cast<double>(HALF_PIXES_PER_MAPENT) },// 2

        glm::dvec2{  static_cast<double>(HALF_PIXES_PER_MAPENT),
                     static_cast<double>(HALF_PIXES_PER_MAPENT) }// 3
    };

}//-------------- namespace: job_cc_inn end ----------------//


//===== static =====//
const IntVec2 Job_MapEntInn::pixesPerHalfMapent {HALF_PIXES_PER_MAPENT, HALF_PIXES_PER_MAPENT};


void Job_MapEntInn::init( IntVec2 mpos_ )noexcept{
    this->mpos = mpos_;
    this->midPPos = mpos_2_ppos(mpos_) + Job_MapEntInn::pixesPerHalfMapent;
    this->alti.set( calc_pixAlti( this->midPPos ) );

    //--- uWeight ---//
    glm::dvec2 fDPos = mpos_2_dpos(this->mpos) * 0.177 + esrc::get_gameSeed().get_field_dposOff();//- 未来更换
    const double freq = 1.0 / 13.0; //- tmp
    this->originPerlin = simplex_noise2( fDPos * freq );//- [-1.0, 1.0]
    this->uWeight = blender_the_perlinNoise( this->originPerlin, 617.7, 97 ); //[0.0, 97.0]
}


void Job_MapEntInn::write_2_mapEnt( MemMapEnt &entRef_ )const noexcept{
    entRef_.set_ecoObjKey( this->ecoObjKey );
    entRef_.set_colorTableId( this->colorRableId );
    entRef_.set_density( this->density );
    entRef_.set_mapAlti( this->alti );
    entRef_.set_perlin( this->originPerlin, this->uWeight );
    entRef_.set_isBorder( this->isBorder );
}


/* ===========================================================
 *                 calc_pixAlti
 * -----------------------------------------------------------
 * 计算单个pix 的 alti
 * ---
 * 这部分算法，应当和 waterAnimCanvas 中的完全一致
 */
double Job_MapEntInn::calc_pixAlti( IntVec2 pixPPos_ )noexcept{

    GameSeed &gameSeedRef = esrc::get_gameSeed();
    glm::dvec2  altiSeed_pposOffBig = gameSeedRef.get_altiSeed_pposOffBig();
    glm::dvec2  altiSeed_pposOffMid = gameSeedRef.get_altiSeed_pposOffMid();
    glm::dvec2  altiSeed_pposOffSml = gameSeedRef.get_altiSeed_pposOffSml();
                            //-- 此处有问题，从 job线程 访问 gameSeed，不够安全...

    double    pixDistance {}; //- pix 距离 世界中心的距离。 暂时假设，(0,0) 为世界中心
    double    seaLvl {};

    double    pnValBig {};
    double    pnValMid {};
    double    pnValSml {};
    double    altiVal  {};  //- target val

    glm::dvec2 pixCFPos {};

            pixCFPos.x = static_cast<double>(pixPPos_.x) / static_cast<double>(PIXES_PER_CHUNK);
            pixCFPos.y = static_cast<double>(pixPPos_.y) / static_cast<double>(PIXES_PER_CHUNK);
            //------------------//
            //     seaLvl
            //------------------//
            pixDistance = glm::distance( pixCFPos, job_cc_inn::worldCenter );
            pixDistance /= 10.0;
            //--------
            seaLvl = simplex_noise2( pixCFPos * job_cc_inn::freqSeaLvl ) * 50.0; // [-50.0, 50.0]
            seaLvl += pixDistance;
            if( seaLvl < 0.0 ){ //- land
                seaLvl *= 0.3;  // [-15.0, 50.0]
            }
            //------------------//
            //    alti.val
            //------------------//
            //--- 使用速度最快的 2D-simplex-noise ---
            pnValBig = simplex_noise2( (pixCFPos + altiSeed_pposOffBig) * job_cc_inn::freqBig ) * 100.0 - seaLvl; // [-100.0, 100.0]
            pnValMid = simplex_noise2( (pixCFPos + altiSeed_pposOffMid) * job_cc_inn::freqMid ) * 50.0  - seaLvl; // [-50.0, 50.0]
            pnValSml = simplex_noise2( (pixCFPos + altiSeed_pposOffSml) * job_cc_inn::freqSml ) * 20.0  - seaLvl; // [-20.0, 20.0]
            //---------
            altiVal = floor(pnValBig + pnValMid + pnValSml);

            //------- 抹平头尾 -------//
            if( altiVal > 100.0 ){
                altiVal = 100.0;
            }else if( altiVal < -100.0 ){
                altiVal = -100.0;
            }
            // now, altiVal: [-100.0, 100.0]=

    return altiVal;
}


// 自动检测 4*4 容器，通过 分形思路，分配 groundGo 实例
void Job_Field::apply_job_groundGoEnts()noexcept{

    glm::vec2       fposOff     {}; // base on field-midDPos
    //sectionKey_t    ecoObjKey   {};
    colorTableId_t  colorTableId {};
    double          uWeight {};

    const Job_MapEntInn *entPtr {nullptr}; 

    //-- field 整个单色 --//
    tprAssert( !this->fields.empty() );
    if( this->fields.size() == 1 ){

        entPtr = this->mapEntPtrs.at(HALF_ENTS_PER_FIELD).at(HALF_ENTS_PER_FIELD);

        fposOff = glm::vec2{0.0, 0.0};
        //ecoObjKey = *this->fields.begin();
        colorTableId = *this->fields.begin();
        uWeight = entPtr->uWeight;

        ( this->isHaveBorderEnt ) ?
            this->groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::Field,       fposOff, colorTableId, uWeight )) :
            this->groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::SimpleField, fposOff, colorTableId, uWeight ));
        
        return; // !!!
    }

    //--- 现在，field 是一定要被拆分的了 ---
    // 执行一次 自底向上 的递归，
    // 遍历每个 象限的 4mapent，如何能合并，就合并, 并当场制作 1个 Job_GroundGoEnt::halfField
    // 如果不能合并，当场制作 4个 Job_GroundGoEnt::mapent
    size_t innIdx {}; // hlafField 内的 局部idx

    for( size_t hIdx=0; hIdx<this->halfFields.size(); hIdx++ ){// 4-halfField

        const auto &halfWHs     = job_cc_inn::whs.at(hIdx);
        const auto &halfDposOff = job_cc_inn::halfFieldDposOffs.at(hIdx);

        auto &halfSetRef = this->halfFields.at(hIdx);
        tprAssert( !halfSetRef.empty() );
        if( halfSetRef.size() == 1 ){

            const auto &wh = halfWHs.back(); 
            entPtr = this->mapEntPtrs.at( static_cast<size_t>(wh.y)).at( static_cast<size_t>(wh.x));
            fposOff = halfDposOff;
            //ecoObjKey = entPtr->ecoObjKey;
            colorTableId = entPtr->colorRableId;
            uWeight = entPtr->uWeight;

            this->groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::HalfField, fposOff, colorTableId, uWeight ));
            continue; // !!!
        }

        //--- 需要将 halfField 拆分为 4-mapent ---
        for( size_t h=0; h<HALF_ENTS_PER_FIELD; h++ ){
            for( size_t w=0; w<HALF_ENTS_PER_FIELD; w++ ){ // each mapent in halfField

                innIdx = h * HALF_ENTS_PER_FIELD + w;
                const auto &wh = halfWHs.at(innIdx);
                entPtr = this->mapEntPtrs.at(static_cast<size_t>(wh.y)).at(static_cast<size_t>(wh.x));

                fposOff = halfDposOff + job_cc_inn::mapentDposOffs.at(innIdx);
                //ecoObjKey = entPtr->ecoObjKey;
                colorTableId = entPtr->colorRableId;
                uWeight = entPtr->uWeight;

                this->groundGoEnts.push_back( std::make_unique<Job_GroundGoEnt>( GroundGoEntType::MapEnt, fposOff, colorTableId, uWeight ));
            }
        }
    }
}






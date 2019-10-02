/*
 * ================= json_mapSurfaceRandMesh.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//--------------- CPP ------------------//
#include <unordered_map>
#include <string>
#include <utility>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"
#include "fileIO.h"
#include "MapSurfaceRandLvl.h"

#include "esrc_state.h"
#include "esrc_mapSurfaceRand.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"
#include "Script/json/json_oth.h"
#include "Script/resource/ssrc.h" 

using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;


namespace msrm_inn {//-------- namespace: msrm_inn --------------//

    const std::vector<std::string> lpath_files {
        "/mapSurface/mapSurface_meshs_1f1.json",
        "/mapSurface/mapSurface_meshs_2f2.json",
        "/mapSurface/mapSurface_meshs_3f3.json"
    };

    void parse_single_mapSurfaceRandMeshJsonFile( const std::string &lPath_file_ );
    mapSurface::RandMeshLvl str_2_randMeshLvl_from_json( const std::string &str_ )noexcept;

}//------------- namespace: msrm_inn end --------------//



/* ===========================================================
 *           parse_from_mapSurfaceRandMeshJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_mapSurfaceRandMeshJsonFile(){

    cout << "   ----- parse_from_mapSurfaceRandMeshJsonFile: start ----- " << endl;

    esrc::is_setState("mapSurfaceRandSet");

    for( const auto &i : msrm_inn::lpath_files ){
        msrm_inn::parse_single_mapSurfaceRandMeshJsonFile(i);
    }

    esrc::insertState("json_mapSurfaceRandMesh");
    cout << "   ----- parse_from_mapSurfaceRandMeshJsonFile: end ----- " << endl;
}



namespace msrm_inn {//-------- namespace: msrm_inn --------------//


void parse_single_mapSurfaceRandMeshJsonFile( const std::string &lPath_file_ ){
    //-----------------------------//
    //         load file
    //-----------------------------//
    std::string path_file = tprGeneral::path_combine(path_jsons, lPath_file_);
    auto jsonBufUPtr = read_a_file( path_file );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    mapSurface::RandEntLvl  entLvl {};
    mapSurface::RandMeshLvl meshLvl {};
    glm::dvec2              dposOff {}; // offset from field MidDPos
    IntVec2                 mposOff {}; // offset from field LeftBottom
    
    
    double halfFieldRadius = static_cast<double>( HALF_ENTS_PER_FIELD * PIXES_PER_MAPENT );
    glm::dvec2 halfField { halfFieldRadius, halfFieldRadius };


    tprAssert( doc.IsObject() );
    {//--- entLvl ---//
        const auto &a = json_inn::check_and_get_value( doc, "entLvl", json_inn::JsonValType::String );
        entLvl = mapSurface::str_2_randEntLvl( a.GetString() );
    }
    //--- data ---//
    const auto &data = json_inn::check_and_get_value( doc, "data", json_inn::JsonValType::Array );

    for( auto &points : data.GetArray() ){
        tprAssert( points.IsArray() );

        auto &randMeshRef = esrc::apply_new_mapSurfaceRandMeshData(entLvl);

        for( auto &point : points.GetArray() ){
            {//--- meshLvl ---//
                const auto &a = json_inn::check_and_get_value( point, "meshLvl", json_inn::JsonValType::String );
                meshLvl = str_2_randMeshLvl_from_json( a.GetString() );
            }
            {//--- dpos ---//
                const auto &a = json_inn::check_and_get_value( point, "dpos", json_inn::JsonValType::Array );
                tprAssert( a[0].IsDouble() );
                tprAssert( a[1].IsDouble() );
                dposOff.x = a[0].GetDouble();
                dposOff.y = a[1].GetDouble();
            }
            //--- mposOff ---//
            mposOff = dpos_2_mpos( dposOff + halfField ); // offset from field LeftBottom
            //---
            randMeshRef.push_back( meshLvl, dposOff, mposOff );
        }
    }        
}


//-- just used for json parse
mapSurface::RandMeshLvl str_2_randMeshLvl_from_json( const std::string &str_ )noexcept{
    if(         str_ == "1m1" ){ return mapSurface::RandMeshLvl::MapEnt_1m1;
    }else if(   str_ == "2m2" ){ return mapSurface::RandMeshLvl::MapEnt_2m2;
    }else if(   str_ == "3m3" ){ return mapSurface::RandMeshLvl::MapEnt_3m3;
    }else if(   str_ == "4m4" ){ return mapSurface::RandMeshLvl::MapEnt_4m4;
    }else if(   str_ == "Nil"       ){ 
        tprAssert(0); // tmp
        return mapSurface::RandMeshLvl::Nil;
    }else{
        tprAssert(0);
        return mapSurface::RandMeshLvl::Nil;
    }
}


}//------------- namespace: msrm_inn end --------------//

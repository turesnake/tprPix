/*
 * ======================= speedLog.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "speedLog.h"

//------------------- C --------------------//
#include <cmath>

//------------------- CPP --------------------//
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <map>

//------------------- Libs --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "global.h"
#include "timeLog.h"

using std::cout;
using std::endl;


namespace tprDebug {//---------- namespace: tprDebug --------------//
namespace speedLog_inn {//------------ namespace: speedLog_inn --------------//

    class SpeedData{
    public:
        SpeedData( const glm::dvec2 &speed_ ):
            speed(speed_),
            len(glm::length(speed_))
            {}

        glm::dvec2 speed {};
        double     len   {};
    };

    std::unordered_map< size_t, std::unique_ptr<SpeedData>> speedDatas {};
    
}//---------------- namespace: timeLog_inn end --------------//


void init_speedLog(){
    speedLog_inn::speedDatas.reserve( 100000 ); //- support 20 mins
}


void collect_playerSpeed( const glm::dvec2 &speedV_ ){

    speedLog_inn::speedDatas.insert({
            get_frameIdx(),
            std::make_unique<speedLog_inn::SpeedData>( speedV_ ) });
}



void process_and_echo_speedLog(){

    //--------------------------//
    //         process
    //--------------------------//
    double minLen {20000.0};
    double maxLen {0.0};
    size_t minIdx {};
    size_t maxIdx {};
    for( const auto &pair : speedLog_inn::speedDatas ){
        const auto &i = *(pair.second.get());
        if( i.len < minLen ){
            minLen = i.len;
            minIdx = pair.first;
        }
        if( i.len > maxLen ){
            maxLen = i.len;
            maxIdx = pair.first;
        }
    }

    //----- sort rankingList ------//
    std::multimap<double, size_t> rankingList {};
    for( const auto &pair : speedLog_inn::speedDatas ){
        rankingList.insert({ pair.second->len, pair.first });
    }

    //--------------------------//
    //    write to file
    //--------------------------//
    std::string path_timeLog = tprGeneral::path_combine(path_tprLog, "speedLog.txt");

    std::ofstream writeFile;
    writeFile.open( path_timeLog );
    if( writeFile ){

        writeFile.precision(12);

        //------------------------//
        //    Kernel Info
        //------------------------//
        writeFile << "~~~~~~~~~~~~~~~~~~~~ Kernel Info: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
                << "\nidx:" << minIdx << "; minSpeedLen = " << minLen
                << "\nidx:" << maxIdx << "; maxSpeedLen = " << maxLen
                << "\n"
                << endl; 

        //------------------------//
        //    rankingList
        //------------------------//
        {
            writeFile << "~~~~~~~~~~~~~~~~~~~~ rankingList: Begin ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

            size_t times {0};
            for( auto it = rankingList.rbegin(); it!=rankingList.rend(); it++ ){

                const auto &frameIdx = it->second;
                const auto &speedData = *(speedLog_inn::speedDatas.at(frameIdx).get());

                writeFile   << "speedLen:   " << speedData.len
                            << ";    speed: " << speedData.speed.x << ", " << speedData.speed.y
                            << ";   frameIdx: " << frameIdx
                            << ";\n";
                times++;
                if( times > 100 ){ // only print 100-ents
                    break;
                }
            }

            writeFile << "~~~~~~~~~~~~~~~~~~~~ rankingList: End ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
                    << endl; 
        }
    }
}





}//-------------------- namespace: tprDebug end --------------//


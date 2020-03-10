/*
 * ======================= speedLog.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "speedLog.h"

//------------------- CPP --------------------//
#include <sstream>
#include <fstream>
#include <iomanip>

//------------------- Libs --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "global.h"
#include "timeLog.h"


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

    std::unordered_map< size_t, std::unique_ptr<SpeedData>> playerGoSpeedDatas {};
    std::unordered_map< size_t, std::unique_ptr<SpeedData>> cameraSpeedDatas {};

    
}//---------------- namespace: timeLog_inn end --------------//


void init_speedLog(){
    speedLog_inn::playerGoSpeedDatas.reserve( 100000 ); //- support 20 mins
    speedLog_inn::cameraSpeedDatas.reserve( 100000 ); //- support 20 mins
}


void collect_playerSpeed( const glm::dvec2 &speedV_ ){
    speedLog_inn::playerGoSpeedDatas.insert({
            get_frameIdx(),
            std::make_unique<speedLog_inn::SpeedData>( speedV_ ) });
}


void collect_cameraSpeed( const glm::dvec2 &speedV_ ){

    size_t frameIdx = get_frameIdx();

    // skip first 2-secs
    if( frameIdx < 120 ){ 
        return;
    }
    // skip Idle State
    if( is_closeEnough( speedV_, glm::dvec2{0.0,0.0}, 0.01 ) ){
        return;
    }
    speedLog_inn::cameraSpeedDatas.insert({
            frameIdx,
            std::make_unique<speedLog_inn::SpeedData>( speedV_ ) });
}



void process_and_echo_speedLog(){

    //--------------------------//
    //         process
    //--------------------------//

    //----- playerGo -----//
    double minLen_playerGo {20000.0};
    double maxLen_playerGo {0.0};
    size_t minIdx_playerGo {};
    size_t maxIdx_playerGo {};
    for( const auto &pair : speedLog_inn::playerGoSpeedDatas ){
        const auto &i = *(pair.second.get());
        if( i.len < minLen_playerGo ){
             minLen_playerGo = i.len;
            minIdx_playerGo = pair.first;
        }
        if( i.len > maxLen_playerGo ){
            maxLen_playerGo = i.len;
            maxIdx_playerGo = pair.first;
        }
    }
    
    //----- camera -----//
    double minLen_camera {20000.0};
    double maxLen_camera {0.0};
    size_t minIdx_camera {};
    size_t maxIdx_camera {};
    for( const auto &pair : speedLog_inn::cameraSpeedDatas ){
        const auto &i = *(pair.second.get());
        if( i.len < minLen_camera ){
             minLen_camera = i.len;
            minIdx_camera = pair.first;
        }
        if( i.len > maxLen_camera ){
            maxLen_camera = i.len;
            maxIdx_camera = pair.first;
        }
    }


    //----- sort playerGo RankingList ------//
    std::multimap<double, size_t> playerGoRankingList {};
    for( const auto &pair : speedLog_inn::playerGoSpeedDatas ){
        playerGoRankingList.insert({ pair.second->len, pair.first });
    }

    //----- sort camera RankingList ------//
    std::multimap<double, size_t> cameraRankingList {};
    for( const auto &pair : speedLog_inn::cameraSpeedDatas ){
        cameraRankingList.insert({ pair.second->len, pair.first });
    }


    //--------------------------//
    //    write to file
    //--------------------------//
    std::string path_timeLog = tprGeneral::path_combine(path_tprLog, "speedLog.txt");

    std::ofstream writeFile;
    writeFile.open( path_timeLog );
    if( writeFile ){

        writeFile.precision(8);

        //------------------------//
        //    Kernel Info
        //------------------------//
        writeFile << "~~~~~~~~~~~~~~~~~~~~ Kernel Info: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
                << "\nplayerGo:"
                << "\n    idx:" << minIdx_playerGo << "; minSpeedLen = " << minLen_playerGo
                << "\n    idx:" << maxIdx_playerGo << "; maxSpeedLen = " << maxLen_playerGo
                << "\n\ncamera:"
                << "\n    idx:" << minIdx_camera << "; minSpeedLen = " << minLen_camera
                << "\n    idx:" << maxIdx_camera << "; maxSpeedLen = " << maxLen_camera
                << "\n"
                << endl; 

        //------------------------//
        //  playerGo RankingList
        //------------------------//
        {
            writeFile << "~~~~~~~~~~~~~~~~~~~~ playerGo speed RankingList: Begin ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

            size_t times {0};
            for( auto it=playerGoRankingList.rbegin(); it!=playerGoRankingList.rend(); it++ ){

                const auto &frameIdx = it->second;
                const auto &speedData = *(speedLog_inn::playerGoSpeedDatas.at(frameIdx).get());

                writeFile   << "speedLen: " << std::setw(10) << speedData.len
                            << ";    speed: " << std::setw(10) << speedData.speed.x << ", " << std::setw(10) << speedData.speed.y
                            << std::setw(16) << ";   frameIdx:  " << frameIdx
                            << ";\n";
                times++;
                if( times > 100 ){ // only print 100-ents
                    break;
                }
            }

            writeFile << "~~~~~~~~~~~~~~~~~~~~ playerGo speed RankingList: End ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
                    << endl; 
        }

        //------------------------//
        //   camera RankingList
        //------------------------//
        {
            writeFile << "~~~~~~~~~~~~~~~~~~~~ camera speed RankingList: Begin ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

            size_t times {0};
            for( auto it=cameraRankingList.rbegin(); it!=cameraRankingList.rend(); it++ ){

                const auto &frameIdx = it->second;
                const auto &speedData = *(speedLog_inn::cameraSpeedDatas.at(frameIdx).get());

                writeFile   << "speedLen: " << std::setw(10) << speedData.len
                            << ";    speed: " << std::setw(10) << speedData.speed.x << ", " << std::setw(10) << speedData.speed.y
                            << std::setw(16) << ";   frameIdx:  " << frameIdx
                            << ";\n";
                times++;
                if( times > 100 ){ // only print 100-ents
                    break;
                }
            }

            writeFile << "~~~~~~~~~~~~~~~~~~~~ camera speed RankingList: End ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
                    << endl; 
        }

    }
}





}//-------------------- namespace: tprDebug end --------------//


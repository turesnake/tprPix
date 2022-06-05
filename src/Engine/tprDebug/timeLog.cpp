/*
 * ========================= timeLog.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.14
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "timeLog.h"

//------------------- CPP --------------------//
#include <utility>
#include <sstream>
#include <fstream>

//------------------- Libs --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "global.h"
#include "fileIO.h"


using std::endl;





namespace tprDebug {//---------- namespace: tprDebug --------------//
namespace timeLog_inn {//------------ namespace: timeLog_inn --------------//

    //-- data in one frame --
    class FrameData{
    public:
        FrameData( size_t idx_, double deltaTime_, size_t frame_ ):
            idx(idx_),
            deltaTime(deltaTime_),
            frame(frame_)
            {}
        //===== vals =====//
        size_t idx;
        double deltaTime;
        size_t frame;
    };

    size_t   idx {};
    std::unordered_map< size_t, std::unique_ptr<FrameData>> frameRawDatas {};
    

    
}//---------------- namespace: timeLog_inn end --------------//

void init_timeLog(){
    timeLog_inn::idx = 0;
    timeLog_inn::frameRawDatas.reserve( 100000 ); //- support 20 mins
}

void collect_deltaTime(double deltaTime_){
    size_t frame =  cast_2_size_t( floor(1.0 / deltaTime_) );
    auto [insertIt, insertBool] = timeLog_inn::frameRawDatas.emplace( timeLog_inn::idx, 
                                        std::make_unique<timeLog_inn::FrameData>( timeLog_inn::idx, deltaTime_, frame ) );
    tprAssert( insertBool );
    timeLog_inn::idx++;
}


size_t get_frameIdx(){
    return timeLog_inn::idx;
}


/* ===========================================================
 *              process_and_echo_timeLog
 * -----------------------------------------------------------
 * 
 */
void process_and_echo_timeLog(){

    //--------------------------//
    //         process
    //--------------------------//
    double minFrame {200.0};
    double maxFrame {0.0};
    size_t minIdx {};
    size_t maxIdx {};

    for( auto &pair : timeLog_inn::frameRawDatas ){
        auto &tf = *(pair.second);

        double tf_frame_d = static_cast<double>(tf.frame);

        if( tf_frame_d > maxFrame ){
            maxFrame = tf_frame_d;
            maxIdx = tf.idx;
        }
        if( tf_frame_d < minFrame ){
            minFrame = tf_frame_d;
            minIdx = tf.idx;
        }
    }

    //----- sort rankingList ------//
    std::multimap<size_t,size_t> rankingList {};
    for( auto &pair : timeLog_inn::frameRawDatas ){
        auto &tmpIdx = pair.first;
        auto &tmpFrame = pair.second->frame;
        rankingList.emplace( tmpFrame, tmpIdx );// multi
    }


    //--------------------------//
    //    write to file
    //--------------------------//
    std::string path_timeLog = tprGeneral::path_combine(path_tprLog, "timeLog.txt");

    std::ofstream writeFile;
    writeFile.open( path_timeLog );
    if( writeFile ){

        writeFile.precision(12);
        
        //------------------------//
        //    Kernel Info
        //------------------------//
        writeFile << "~~~~~~~~~~~~~~~~~~~~ Kernel Info: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
                << "\nidx:" << minIdx << "; minFrame = " << minFrame
                << "\nidx:" << maxIdx << "; maxFrame = " << maxFrame
                << "\n"
                << endl; 


        //------------------------//
        // rankingList: Minframes
        //------------------------//
        {
            writeFile << "~~~~~~~~~~~~~~~~~~~~ rankingList: Minframes: Begin ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

            size_t times {0};
            for( const auto &pair : rankingList ){
                const auto &tmpFrame = pair.first;
                const auto &tmpIdx = pair.second;
                writeFile   << "frame: " << tmpFrame
                            << "; idx: " << tmpIdx
                            << ";\n";
                times++;
                if( times > 100 ){ // only print 100-ents
                    break;
                }
            }
            writeFile << "~~~~~~~~~~~~~~~~~~~~ rankingList: Minframes: End ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
                    << endl; 
        }

        //------------------------//
        // rankingList: Maxframes
        //------------------------//
        {
            writeFile << "~~~~~~~~~~~~~~~~~~~~ rankingList: Maxframes: Begin ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

            size_t times {0};
            for( auto it=rankingList.rbegin(); it!=rankingList.rend(); it++  ){
                const auto &tmpFrame = it->first;
                const auto &tmpIdx = it->second;
                writeFile   << "frame: " << tmpFrame
                            << "; idx: " << tmpIdx
                            << ";\n";
                times++;
                if( times > 100 ){ // only print 100-ents
                    break;
                }
            }
            writeFile << "~~~~~~~~~~~~~~~~~~~~ rankingList: Maxframes: End ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
                    << endl; 
        }




        //------------------------//
        //    frameRawDatas
        //------------------------//

        /*
        writeFile << "~~~~~~~~~~~~~~~~~~~~ frameRawDatas: Begin ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        for( auto &pair : timeLog_inn::frameRawDatas ){
            auto &tf = *(pair.second);
            writeFile << "" << tf.idx
                << ": deltaTime: " << tf.deltaTime
                << "; frame: " << tf.frame
                << ";\n";
        }
        writeFile << "~~~~~~~~~~~~~~~~~~~~ frameRawDatas: End ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
                << endl;  
        */

    }
}



}//-------------------- namespace: tprDebug end --------------//



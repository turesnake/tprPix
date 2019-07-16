/*
 * ========================= timeLog.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.14
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "timeLog.h"

//------------------- C --------------------//
#include <cmath>

//------------------- CPP --------------------//
#include <utility>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <map>

//------------------- Libs --------------------//
#include "tprGeneral.h"


//-------------------- Engine --------------------//
#include "global.h"
#include "fileIO.h"
#include "tprCast.h"

using std::cout;
using std::endl;


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


namespace timeLog_inn {//------------ namespace: timeLog_inn --------------//

    size_t   idx {};
    std::unordered_map< size_t, std::unique_ptr<FrameData>> frameRawDatas {};

    //-- sorted containers ---//
    std::multimap<size_t,size_t> frame_idxs {};


    //----- funcs -----//
    void sort_frame_idxs();
    
}//---------------- namespace: timeLog_inn end --------------//

void init_timeLog(){
    timeLog_inn::idx = 0;
    timeLog_inn::frameRawDatas.reserve(100000); //- support 20 mins
}

void collect_deltaTime(double deltaTime_){

    size_t frame =  cast_2_size_t( floor(1.0 / deltaTime_) );

    auto tfUPtr = std::make_unique<FrameData>( timeLog_inn::idx, deltaTime_, frame );
    timeLog_inn::frameRawDatas.insert({ timeLog_inn::idx, std::move(tfUPtr) });

    timeLog_inn::idx++;
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
        auto &tf = *(pair.second.get());
        if( tf.frame > maxFrame ){
            maxFrame = tf.frame;
            maxIdx = tf.idx;
        }
        if( tf.frame < minFrame ){
            minFrame = tf.frame;
            minIdx = tf.idx;
        }
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
        //    frame_idxs
        //------------------------//
        {
            writeFile << "~~~~~~~~~~~~~~~~~~~~ frame_idxs: Begin ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

            timeLog_inn::sort_frame_idxs();

            size_t times {0};
            for( const auto &pair : timeLog_inn::frame_idxs ){
                const auto &tmpFrame = pair.first;
                const auto &tmpIdx = pair.second;
                writeFile   << "frame: " << tmpFrame
                            << "; idx: " << tmpIdx
                            << ";\n";
                times++;
                if( times > 50 ){
                    break;
                }
            }
            writeFile << "~~~~~~~~~~~~~~~~~~~~ frame_idxs: End ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
                    << endl; 
        }
        //------------------------//
        //    frameRawDatas
        //------------------------//
        writeFile << "~~~~~~~~~~~~~~~~~~~~ frameRawDatas: Begin ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        for( auto &pair : timeLog_inn::frameRawDatas ){
            auto &tf = *(pair.second.get());
            writeFile << "" << tf.idx
                << ": deltaTime: " << tf.deltaTime
                << "; frame: " << tf.frame
                << ";\n";
        }
        writeFile << "~~~~~~~~~~~~~~~~~~~~ frameRawDatas: End ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"
                << endl;  


    }
}


namespace timeLog_inn {//------------ namespace: timeLog_inn --------------//

void sort_frame_idxs(){

    frame_idxs.clear();
    for( auto &pair : frameRawDatas ){
        auto &tmpIdx = pair.first;
        auto &tmpFrame = pair.second->frame;
        frame_idxs.insert({ tmpFrame, tmpIdx });
    }
}



}//---------------- namespace: timeLog_inn end --------------//





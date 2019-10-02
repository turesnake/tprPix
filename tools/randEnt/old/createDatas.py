import os
import sys
import json
import random # [0,1)

from tools import *

#------------ INPUT ------------#
#  need user input:
entNums_1 = 2
entNums_2 = 4
entNums_3 = 3
entNums_4 = 2

#------------ global Vals ------------#
PIXES_PER_MAPENT = 64
ENTS_PER_CHUNK = 32
ENTS_PER_FIELD = 4
FIELDS_PER_CHUNK = 8

#- mesh radius:
RADIUS_1 = (64 * 1) * 0.5 # 32
RADIUS_2 = (64 * 2) * 0.5 # 64
RADIUS_3 = (64 * 3) * 0.5 # 96
RADIUS_4 = (64 * 4) * 0.5 # 128

windowSZ = PIXES_PER_MAPENT * ENTS_PER_CHUNK
windowRadius = windowSZ * 0.5
windowCenter = Vec2( windowRadius, windowRadius )


#--- not detect collide yet ---#
# param: entRadius_ float
# return: Cir
def apply_a_cir( entRadius_ ):
    rad = windowRadius - entRadius_
    dpos = apply_new_randVec2(rad) + windowCenter
    cir = Cir( dpos, entRadius_ )
    return cir
    #------------------#



# param: filePath_  str
# param: buf_       str
def write_to_file( filePath_, buf_ ):
    f = open( filePath_, "w+"  )
    f.write( buf_ )
    f.close()



if __name__=="__main__":
    print("======== create random dpos for ents ===========")

    #----- path -----#
    path_root = os.path.abspath('.') # current dir path "/xxx/xx"
    path_out   = os.path.join( path_root, "data" ) # out dir


    #----- apply and detect -----#
    cirs = []

    #- 4 * 4 mapents:
    for i in range(entNums_4) :
        cir = apply_a_cir(RADIUS_4)

        # detect ....

        cirs.append( cir )

    
    #----- dump to JSON data -----#
    jsonData = []
    for cir in cirs:
        dpos = []
        dpos.append( cir.dpos.x - windowRadius )
        dpos.append( cir.dpos.y - windowRadius )
        dic = { "tmp" : 1 } # tmp val
        dic["dpos"] = dpos
        dic["radius"] = cir.radius
        del dic["tmp"] # no need 
        jsonData.append( dic )
    jsonDataBuf = json.dumps(jsonData)

    #--- write ---#
    path_outFile = os.path.join( path_out, "randEnt.json" ) # out
    write_to_file( path_outFile, jsonDataBuf )


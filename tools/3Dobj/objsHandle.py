import os
import sys
import json


class ObjEnt(object):

    def __init__(self):
        self.objName = ""
        self.points = []

    # param: buf_   str
    def handle(self, buf_ ):
        tmpPoints = [] # [][]
        tmpFaces = []  # []
        for line in buf_:
            self.handle_line( line, tmpPoints, tmpFaces )

        for idx in tmpFaces:
            self.points.append( tmpPoints[idx] )


    # param: line_   str
    # param: points_ list [][]
    # param: faces_  list [][]
    def handle_line( self, line_, points_, faces_ ):
        words = line_.split( " " )

        # only once
        if( words[0] == "o" ):
            assert len(words) == 2
            self.objName = parse_obj_name( words[1] )
            return
        
        # several times
        if( words[0] == "v" ):
            del words[0]    
            point = [] # xyz: 3 float
            for i in words:
                point.append( float(i) )
            points_.append( point )
            return

        # only once
        if( words[0] == "f" ):
            del words[0]
            for word in words:
                nums = word.split("/")
                faces_.append( int(nums[0]) - 1 ) # align to 0

#--------------------------------

# param:  "x_xx_xxx.1_Plane.001"
# return: "x_xx_xxx.1"
def parse_obj_name( objName_ ):
    underLine = "_"
    tmps = objName_.split(underLine)
    tmps.pop() # erase last ent
    out = underLine.join(tmps) # "x_xx_xxx.1"
    return out


# detect if a line is useful
def is_useful_line( line_ ):
    words = line_.split( " " )
    if( words[0]=="o" or words[0]=="v" or words[0]=="f" ):
        return True
    else:
        return False


def is_have_target_head( line_, word_ ):
    words = line_.split( " " )
    if( words[0]==word_ ):
        return True
    else:
        return False







def read_a_objFile( filePath_ ):

    f = open( filePath_, "r" )
    buf = f.read()
    lines = buf.splitlines(False) # abandon "\n"

    #-- erase useless lines --
    lines_2 = []
    for line in lines:
        if( is_useful_line(line) ):
            lines_2.append(line)

    objBufs = [] # two-dimension
    for line in lines_2:
        if( is_have_target_head(line, "o") ):
            #-- add new entList --
            tmp = []
            objBufs.append( tmp )
        #-- append to lastEntList in objBufs --
        idx = len(objBufs) - 1
        objBufs[idx].append( line )

    objEnts = []
    for objBuf in objBufs:
        objEnt = ObjEnt()
        objEnt.handle( objBuf )
        objEnts.append( objEnt )
        
    f.close()
    return objEnts # list of ObjEnt


# param: filePath_  str
# param: buf_       str
def write_to_file( filePath_, buf_ ):
    f = open( filePath_, "w+"  )
    f.write( buf_ )
    f.close()


if __name__=="__main__":
    print("======== handle all .obj files in this Dir ===========")

    #----- path -----#
    path_root = os.path.abspath('.') # current dir path "/xxx/xx"
    path_datas = os.path.join( path_root, "datas" ) # data dir
    path_out   = os.path.join( path_root, "out" ) # out

    #----- get all file path in data dir -----#
    fileNames = os.listdir( path_datas )
    objEnts = [] 
    for fileName in fileNames:
        #-- skip oth files --
        name,suffix = os.path.splitext(fileName)
        if( suffix != ".obj" ):
            continue

        path = os.path.join( path_datas, fileName )
        print( path )
        assert os.path.isfile( path )
        #---
        objEnts.extend( read_a_objFile(path) )

    #--- dump to JSON data ---#
    jsonData = [] 
    for objEnt in objEnts:
        dic = { "tmp" : 1 } # tmp val
        dic["objName"] = objEnt.objName
        dic["points"] = objEnt.points
        del dic["tmp"] # no need 
        jsonData.append( dic )
    jsonDataBuf = json.dumps(jsonData)

    #--- write ---#
    path_outFile = os.path.join( path_out, "polyObjs.json" ) # out
    write_to_file( path_outFile, jsonDataBuf )


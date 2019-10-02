//
// meshs_in_2m2_field
// ( display without scale )


int PIXES_PER_MAPENT = 64;// 64
int ENTS_PER_FIELD = 4; // 4
int FIELDS_PER_CHUNK = 8; // 8

int FIELDS = 2;

float windowSZ = FIELDS * ENTS_PER_FIELD * PIXES_PER_MAPENT;
FVec2 windowCenter = new FVec2( windowSZ/2, windowSZ/2);


JSONArray values;
String   fileName; // in app/data/ dir


void setup(){
  
  //---- size ----//
  // 2*2 field: {512,512}
  size( 512, 512 );
                            
  //----------------//                
  fileName = "randEnt.json";                          
                       
  //---- ground grid -----//
  background( 240, 240, 240 );
  
  //- mapEnt lines:
  stroke(220);
  strokeWeight(2);
  int step = PIXES_PER_MAPENT;
  for( int i=0; i<ENTS_PER_FIELD * FIELDS; i++ ){
    //-- y lines --//
    line( 0, 
          i * step, 
          windowSZ,
          i * step );
    //-- x lines --//
    line( i * step,
          0, 
          i * step,
          windowSZ );                 
  }
  
  //- field lines:
  stroke(200);
  strokeWeight(4);
  step = PIXES_PER_MAPENT * ENTS_PER_FIELD;
  for( int i=0; i<FIELDS; i++ ){
    //-- y lines --//
    line( 0, 
          i * step, 
          windowSZ,
          i * step );
    //-- x lines --//
    line( i * step,
          0, 
          i * step,
          windowSZ );                 
  }
  
 
  //--- JSON data ----//
  values = loadJSONArray(fileName);

  for (int i = 0; i < values.size(); i++) {
    JSONObject ent = values.getJSONObject(i); 
    
    JSONArray dpos = ent.getJSONArray("dpos");
    float x = dpos.getFloat(0);
    float y = dpos.getFloat(1);
    float radius = str_2_radius( ent.getString("meshLvl") );
    //---
    println( "x = ", x );
    println( "y = ", y );
    println( "radius = ", radius );
    //---    
    draw_jsonCir( x, y, radius );
  }
  
  //--- window CENTER ----//
  stroke(150,50,50);
  strokeWeight(20);
  point( windowCenter.x, windowCenter.y );
}



class FVec2{
  float x;
  float y;
  FVec2( float x_, float y_ ){
    x = x_;
    y = y_;
  }
  int get_intX(){ return int(x); }
  int get_intY(){ return int(y); }
}



void draw_jsonCir( float dposOffX_, float dposOffY_, float radius_ ){
  float x =   dposOffX_ + windowCenter.x;
  float y = - dposOffY_ + windowCenter.y; // negative
  //--- rect ---//
  stroke(150,160,180);
  strokeWeight(2);
  fill( 220,250,150, 40 );
  rect( x - radius_, 
        y - radius_, 
        radius_*2, 
        radius_*2 );
  //--- circle ---//
  stroke(150,160,180);
  strokeWeight(5);
  fill( 200,250,230, 80 );
  ellipse( x, y, radius_*2, radius_*2 );
  //--- center point ---//
  stroke(200,180,160);
  strokeWeight(10);
  point( x, y );
}

float str_2_radius( String str_ ){
    if(       str_.equals("1m1") ){
        return 32;
    }else if( str_.equals("2m2") ){
        return 64;
    }else if( str_.equals("3m3") ){
        return 96;
    }else if( str_.equals("4m4") ){
        return 128;
    }else{
        println("ERROR: str_2_radius; str_ = ", str_);
        exit();
        return 0; //never reach 
    }
}


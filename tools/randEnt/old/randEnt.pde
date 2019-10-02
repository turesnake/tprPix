//
// display the data from json file
//

float scale = 2.0;

FVec2 windowSZ = new FVec2( 32*64/scale, 32*64/scale );
FVec2 windowCenter = new FVec2( windowSZ.x/2,
                                windowSZ.y/2);

int PIXES_PER_MAPENT = 64;// 64
int ENTS_PER_CHUNK = 32; // 32
int ENTS_PER_FIELD = 4; // 4
int FIELDS_PER_CHUNK = 8; // 8


JSONArray values;
String   fileName; // in app/data/ dir

int x= 800;
int y=600;

void setup(){
  
  //---- size ----//
  // 1*1 field: {128,128}
  // 2*2 field: {256,256}
  // 3*3 field: {384,384}
  // 1*1 chunk: {1024,1024}
  size( 1024, 1024);
  
  //----------------//
  scale = 2.0;
  PIXES_PER_MAPENT = 64 / int(scale);

                            
  //----------------//                
  fileName = "randEnt.json";                          
                       
  //---- ground grid -----//
  background( 240, 240, 240 );
  
  //- mapEnt lines:
  stroke(220);
  strokeWeight(2);
  int step = PIXES_PER_MAPENT;
  for( int i=0; i<ENTS_PER_CHUNK; i++ ){
    //-- y lines --//
    line( 0, 
          i * step, 
          windowSZ.get_intY(),
          i * step );
    //-- x lines --//
    line( i * step,
          0, 
          i * step,
          windowSZ.get_intX() );                 
  }
  
  //- field lines:
  stroke(200);
  strokeWeight(4);
  step = PIXES_PER_MAPENT * ENTS_PER_FIELD;
  for( int i=0; i<FIELDS_PER_CHUNK; i++ ){
    //-- y lines --//
    line( 0, 
          i * step, 
          windowSZ.get_intY(),
          i * step );
    //-- x lines --//
    line( i * step,
          0, 
          i * step,
          windowSZ.get_intX() );                 
  }
  
  
 
  //--- JSON data ----//
  values = loadJSONArray(fileName);

  for (int i = 0; i < values.size(); i++) {
    JSONObject ent = values.getJSONObject(i); 
    
    JSONArray dpos = ent.getJSONArray("dpos");
    float x = dpos.getFloat(0);
    float y = dpos.getFloat(1);
    float radius = ent.getFloat("radius");
    //---
    println( "x = ", x );
    println( "y = ", y );
    println( "radius = ", radius );
    //---
    x = x / scale;
    y = y / scale;
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

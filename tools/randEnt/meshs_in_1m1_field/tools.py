
import math
import random # [0,1)


# param: x_ float
# param: y_ float
class Vec2(object):
    def __init__(self, x_, y_):
        self.x = x_
        self.y = y_
    def __add__(self,oth_):
        return Vec2( self.x+oth_.x, self.y+oth_.y )
    def __sub__(self,oth_):
        return Vec2( self.x-oth_.x, self.y-oth_.y )
    #------------------#


# param: dpos_   Vec2
# param: radius_ float
class Cir(object):
    def __init__(self, dpos_, radius_):
        self.dpos   = dpos_
        self.radius = radius_
    #------------------#


# param: radius_ float
# return: Vec2 
def apply_new_randVec2( radius_ ):
    rx = random.random() # [0,1)
    ry = random.random() # [0,1)
    x = (rx*rx) * radius_
    y = (ry*ry) * radius_
    isNegaX = 1 if (random.randint(-10,9)<0) else -1
    isNegaY = 1 if (random.randint(-10,9)<0) else -1
    #---
    x = x * isNegaX
    y = y * isNegaY
    #---
    x = int(x) # align to pix
    y = int(y) # align to pix
    #---
    v = Vec2( x, y )
    return v
    #------------------#

# param: a_ Vec2
# param: b_ Vec2
# return: float
def distance_of_2_vec2( a_, b_ ):
    v= a_ - b_
    return math.hypot( v.x, v.y )
    #------------------#


# param: a_ Cir
# param: b_ Cir
# return: bool
def is_2_cir_collide( a_,  b_ ):
    dis = distance_of_2_vec2( a_.dpos, b_.dpos )
    sum_of_rads = a_.radius + b_.radius
    return (True if (dis<sum_of_rads) else False )
    #------------------#


# param: cir_  Cir
# param: cirs_ Cir[]
# return: bool
def detect_is_collide( cir_, cirs_ ):
    for be in cirs_:
        if( is_2_cir_collide(cir_, be) ):
            return True
    return False
    #------------------#


# param: a_  float
# param: b_  float
# param: threshold_  float
# return: bool
def is_closeEnough( a_, b_, threshold_ ):
    assert threshold_ > 0
    off = math.fabs( a_ - b_ )
    return (True if (off<threshold_) else False)
    #------------------#




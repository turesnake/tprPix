
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
    x = random.random() * radius_
    y = random.random() * radius_
    isNegaX = 1 if (random.randint(-10,9)<0) else -1
    isNegaY = 1 if (random.randint(-10,9)<0) else -1
    #---
    x = x * isNegaX
    y = y * isNegaY
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









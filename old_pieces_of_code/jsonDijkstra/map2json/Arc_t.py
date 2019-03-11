from Point_t import Point_t
import math
from Leg import *

class Arc_t(Leg):
    """Classe définissant un arc"""

    def __init__(self,centre_tulpe,rayon, angle_start, angle_end, id):
        Leg.__init__(self, id)#classe mere Leg
        self._centre = Point_t(centre_tulpe)#position du centre de l'arc
        self._rayon = rayon #rayon de l'arc
        self._angle_start = angle_start#angle du point start
        self._angle_end = angle_end#angle du point end
        self._longueur = self._rayon*((self._angle_end-self._angle_start)%360)*math.pi/180 #longueur de l'arc

    def create_start_end(self):
        """A partir des informations, créent les points start et end du leg"""
        x = self._centre.get_x() + self._rayon*math.cos(self._angle_start*math.pi/180)
        y = self._centre.get_y() + self._rayon*math.sin(self._angle_start*math.pi/180)
        point_start = (x,y,self._centre.get_z())
        x = self._centre.get_x() + self._rayon*math.cos(self._angle_end*math.pi/180)
        y = self._centre.get_y() + self._rayon*math.sin(self._angle_end*math.pi/180)
        point_end = (x,y,self._centre.get_z())

        self._point_start = Point_t(point_start)
        self._point_end = Point_t(point_end)

    def get_centre(self):
        """Retourne la position du centre"""
        return self._centre

    def get_rayon(self):
        """Retourne le rayon de l'arc"""
        return self._rayon

    def get_angle_start(self):
        """Retourne l'angle du point start"""
        return self._angle_start

    def get_angle_end(self):
        """Retourne l'angle du point end"""
        return self._angle_end

    def set_rayon(self, r):
        """Set le rayon de l'arc"""
        self._rayon = r

    def set_angle_start(self, angle):
        """Set l'angle de start"""
        self._angle_start = angle

    def set_angle_end(self, angle):
        """Set l'angle end"""
        self._angle_end = angle

    def set_centre(self, point):
        """Set le centre de l'arc"""
        self._centre = point
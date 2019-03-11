from Point_t import Point_t
from Leg import *
import math

class Line_t(Leg):
    """Classe représentant une ligne"""

    def __init__(self, coo_tulpe_start, coo_tulpe_end, id):
        Leg.__init__(self, id) #classe mere leg
        self._point_start = Point_t(coo_tulpe_start) #point start du leg
        self._point_end = Point_t(coo_tulpe_end) #point end du leg
        self._longueur = math.sqrt(math.pow((self._point_start.get_x()-self._point_end.get_x()),2)+math.pow((self._point_start.get_y()-self._point_end.get_y()),2)+math.pow((self._point_start.get_z()-self._point_end.get_z()),2))#longueur du leg
    def get_start_x(self):
        test=self._point_start
        x=[test.get_x,test.get_y,test.get_z]
        return x

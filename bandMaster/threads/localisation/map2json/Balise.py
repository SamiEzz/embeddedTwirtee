from Point_t import Point_t

class Balise(Point_t):
    """Classe définissant les balises radios"""

    def __init__(self, coo_tulpe, id):
        self._id = id #identifiant de la balise
        Point_t.__init__(self,coo_tulpe) #position de la balise

    def get_id(self):
        """Retourne l'id"""
        return self._id
    def get_pos(self):
        """Retourne l'id"""
        pos=[str(self.get_x),str(self.get_y),str(self.get_z)]
        return pos

    def set_id(self, id):
        """Set l'id"""
        self._id = id
    
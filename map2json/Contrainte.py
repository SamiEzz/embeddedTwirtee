from Point_t import Point_t

class Contrainte(Point_t):
    """Classe définissant les contraintes"""

    def __init__(self, coo_tulpe, value, type, nom):
        self._value = value #valeur de la contrainte
        self._type = type #type de contrainte 'dwn constraint' , 'Up constraint' ...
        self._nom = nom #nom de la contrainte 'v1'....
        Point_t.__init__(self,coo_tulpe)

    def get_value(self):
        """Retourne la valeur de la contrainte"""
        return self._value

    def set_value(self, value):
        """Set la valeur de la contrainte"""
        self._value = value

    def set_point(self, pt):
        """Set le point, ie la position, de la contrainte"""
        self._pos = pt

    def get_nom(self):
        """Retourne le nom de la contrainte"""
        return self._nom

    def get_type(self):
        """Retourne le type de la contrainte"""
        return self._type
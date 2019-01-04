from Point_t import Point_t

class WPT(Point_t):
    """Classe définissant les waypoints"""

    def __init__(self, coo_tulpe, description, id):
        self._waypoint = description #description sous forme de string du waypoints : 'waypoint', 'node standard'...
        self._id = id #identifiant numerique du waypoint
        Point_t.__init__(self,coo_tulpe) #position du waypoint
        self._on_leg = {} #dictionnaire ayant pour clé un leg. Renvoi la distance entre le waypoint et le point start du leg.

    def is_waypoint(self):
        """retourne la description du waypoint"""
        return self._waypoint

    def set_waypoint(self, des):
        """Set la description"""
        self._waypoint = des

    def set_point(self, pt):
        """Set point par la position"""
        self._pos = pt

    def get_id(self):
        """Retourne l'id du waypoint"""
        return self._id
    
    def get_pos(self):
        """Return position"""
        return self._pos

    def add_on_leg(self, leg, dist_to_start):
        """ajout d'un leg au dictionnaire d'appartenance du waypoint."""
        self._on_leg[leg] = dist_to_start

    def get_on_leg(self):
        """retourne le dictionnaire d'appartenance du waypoint"""
        return self._on_leg
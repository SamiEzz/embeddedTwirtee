
class Leg:
    """Défini les legs arc ou lignes et leurs méthodes"""
    def __init__(self, id):
        self._longueur = 0 #la longueur du leg
        self._point_start = None #le point start du leg
        self._point_end = None #le point end du leg
        self._leg_start = [] #liste des legs au point start
        self._leg_end = [] #liste des legs au point end
        self._has_waypoints = {} #dictionnaires des waypoints sur le leg avec en valeurs leur distance au point start de self._point_start
        self._id = id
        self._contrainte = None

    def set_contrainte(self,contrainte):
        """Set l'argument contrainte"""
        self._contrainte = contrainte

    def get_contrainte(self,contrainte):
        """Get de la contrainte"""
        return self._contrainte

    def get_id(self):
        """Retourne l'id du leg"""
        return self._id

    def get_waypoints_owned(self):
        """Retourne la liste des waypoints appartenant au leg"""
        return self._has_waypoints

    def add_waypoints(self, waypoint, dist_to_start):
        """ajout d'un lwpt au dictionnaire des waypoints."""
        self._has_waypoints[waypoint] = dist_to_start

    def get_longueur(self):
        """Retourne la longueur du leg"""
        return self._longueur

    def get_leg_start(self):
        """Renvoi la liste de legs au point start"""
        return self._leg_start

    def get_leg_end(self):
        """Renvoi la liste de legs au point start"""
        return self._leg_end

    def get_point_start(self):
        """Renvoi le point start"""
        return self._point_start

    def get_point_end(self):
        """Renvoi le point end"""
        return self._point_end

    def set_point_start(self, point):
        """Set Point start"""
        self._point_start = point

    def set_point_end(self, point):
        """Set Point end"""
        self._end = point

    def is_at_start(self,leg):
        """Retourne True si le leg est voisin au point start. Approximation à 5 cm près"""
        point_start = leg.get_point_start()
        point_end = leg.get_point_end()
        if self._point_start.distance(point_start)< 5 or self._point_start.distance(point_end)< 5:
            return True
        return False

    def is_at_end(self,leg):
        """Retourne True si le leg est voisin au point end. Approximation à 5cm près"""
        point_start = leg.get_point_start()
        point_end = leg.get_point_end()
        if self._point_end.distance(point_start)< 5 or self._point_end.distance(point_end)< 5:
            return True
        return False

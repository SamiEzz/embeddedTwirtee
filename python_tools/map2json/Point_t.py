import math
import Fonctions_extraction

class Point_t:
    """Classe représentant un point"""
    def __init__(self, coo):
        self._x = coo[0] #coordonnée selon x
        self._y = coo[1] #coordonnée selon y
        self._z = coo[2] #coordonnée selon z

    def get_x(self):
        """Retourne la coordonnée selon x"""
        return self._x

    def get_y(self):
        """Retourne la coordonnée selon y"""
        return self._y

    def get_z(self):
        """Retourne la coordonnée selon z"""
        return self._z

    def set_x(self, x):
        """Set la coordonnée selon x"""
        self._x = x

    def set_y(self, y):
        """Set la coordonnée selon y"""
        self._y = y

    def set_z(self, z):
        """Set la coordonnée selon z"""
        self._z = z

    def modify(self, translation, rotX, rotY):
        """Permet de modifier un point après une translation et une rotation de matrice [rotX rotY]"""
        a = self._x - translation[0]
        b = self._y - translation[1]
        c = self._z - translation[2]

        self._x = a*rotX[0] + b*rotX[1]
        self._y = a * rotY[0] + b * rotY[1]

    def distance(self, point):
        """Calcule la distance entre ce point et un autre"""
        d_x_2 = math.pow(self._x - point.get_x(),2)
        d_y_2 = math.pow(self._y - point.get_y(),2)
        d_z_2 = math.pow(self._z - point.get_z(),2)

        return math.sqrt(d_x_2 + d_y_2 + d_z_2)

    def is_on_arc(self,arc):
        """Savoir si un point est sur un arc. Return (True/False, angle au point start en radian) avec True si le point est sur l'arc. Approximation au cm"""
        # verification au cm de la distance au rayon
        dist = self.distance(arc.get_centre())
        if abs(dist - arc.get_rayon()) < 1:
            # on regarde si on est entre les angles de début et de fin
            vecteur_centre_start = [arc.get_point_start().get_x() - arc.get_centre().get_x(),
                                    arc.get_point_start().get_y() - arc.get_centre().get_y()]
            vecteur_centre_end = [arc.get_point_end().get_x() - arc.get_centre().get_x(),
                                  arc.get_point_end().get_y() - arc.get_centre().get_y()]
            vecteur_centre_point = [self.get_x() - arc.get_centre().get_x(),
                                    self.get_y() - arc.get_centre().get_y()]

            # calcul des angles
            angle_s_e = math.acos(Fonctions_extraction.produit_scalaire_norm(vecteur_centre_end, vecteur_centre_start))
            angle_e_p = math.acos(Fonctions_extraction.produit_scalaire_norm(vecteur_centre_end, vecteur_centre_point))
            angle_s_p = math.acos(Fonctions_extraction.produit_scalaire_norm(vecteur_centre_point, vecteur_centre_start))

            # vérification angle
            if angle_e_p < angle_s_e and angle_s_p < angle_s_e:
                if str(type(self)) == "<class 'WPT.WPT'>":
                    self.add_on_leg(arc, angle_s_p*arc.get_rayon())
                    arc.add_waypoints(self, angle_s_p*arc.get_rayon())
                return (True,angle_s_p)

        return (False,0)

    def is_on_line(self,line):
        """Savoir si un point est sur une line. Return True si le point est sur la line. Approximation à 1° pour la colinéarité"""
        # initialisation des vecteurs afin de connaître sicolinéaire ou non
        vecteur_start_end = [line.get_point_end().get_x() - line.get_point_start().get_x(),
                             line.get_point_end().get_y() - line.get_point_start().get_y()]
        vecteur_start_wpt = [self.get_x() - line.get_point_start().get_x(), self.get_y() - line.get_point_start().get_y()]
        norme_start_end = Fonctions_extraction.norme_vecteur(vecteur_start_end)
        norme_start_wpt = Fonctions_extraction.norme_vecteur(vecteur_start_wpt)

        # détermination de la colinéarité
        det = Fonctions_extraction.determinant(vecteur_start_end, vecteur_start_wpt)
        sinus_de_l_angle = det / (norme_start_wpt * norme_start_end)

        # on considère que sin = angle et 1°=0,017 rad
        if abs(sinus_de_l_angle) < 0.017:
            # cas ou l'on est colinéaire
            if Fonctions_extraction.produit_scalaire_norm(vecteur_start_end,vecteur_start_wpt)>0 and norme_start_end>norme_start_wpt:
                if str(type(self)) == "<class 'WPT.WPT'>":
                    self.add_on_leg(line, norme_start_wpt)
                    line.add_waypoints(self, norme_start_wpt)
                return True

        return False
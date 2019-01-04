import dxfgrabber
from dxfgrabber.dxfentities import Insert, Line, Arc
from Point_t import *
from Balise import *
from WPT import WPT
from Line_t import Line_t
from Arc_t import Arc_t
from Contrainte import *
from node import *
import math
import json, pdb
import numpy as np



def produit_scalaire_norm(vect_1, vect_2):
    """Calcul le produit scalaire normalisé en 2D"""
    ps = vect_1[0]*vect_2[0] + vect_1[1]*vect_2[1]
    norm_vect_1 = norme_vecteur(vect_1)
    norm_vect_2 = norme_vecteur(vect_2)
    return ps / (norm_vect_1*norm_vect_2)

def determinant(vect_1,vect_2):
    """Calcul d'un déterminant en dimension 2"""
    return vect_1[0]*vect_2[1] - vect_1[1]*vect_2[0]

def norme_vecteur(vect):
    """Calcul de la norme d'un vecteur en dimension 2"""
    return math.sqrt(math.pow(vect[0],2)+math.pow(vect[1],2))


def load_dxf_file(fichier):
    """Permet de charger les informations du fichier dxf représentant la carte 2D"""
    dxf = dxfgrabber.readfile(fichier)
    headers = dxf.header

    #translation et rotation du fichier autocad
    translation = headers['$UCSORG']
    rotX = headers['$UCSXDIR']
    rotY = headers['$UCSYDIR']
    rotation = math.degrees(math.acos(rotX[0]))

    
    mapping = {}

    mapping["Constants"]=[]
    mapping["Constraints"] = []
    mapping["Beacons"] = []
    mapping["Waypoints"] = []
    mapping["Legs"] = []
    mapping["nodes"] = []
    

    tableau_contraintes = {}

    id_wpt = 1
    id_leg = 1

    print("Loop on file")
    

    #recuperation des Constants
    for i in dxf.entities:
        if i.layer == "Twirtee-tracks" and type(i) is Insert and i.name == "Constants":
            values = i.attribs
            for v in values:
                contraintes = v.text.split("\n")
                for c in contraintes:
                    infos = c.split("=")
                    name = infos[0]
                    value = (float) (infos[1].split("m/")[0])
                    tableau_contraintes[name.upper()] = value
            mapping["Constants"]=tableau_contraintes
            # tableau = {'VDEF': 0.5, 'ADEF': 0.5, 'V1': 0.25, 'V2': 0.15}

            

    #analyse des autres données
    
    for i in dxf.entities:
        if i.layer == "Twirtee-tracks":
            #print(type(i))
            if type(i) is Insert:
                # pour catcher les contraintes
                if i.name == "Up constraint":

                    pos = i.insert
                    contrainte = (i.attribs[0].text).upper()
                    value = tableau_contraintes[contrainte]
                    ctr = Contrainte(pos, value, i.name, contrainte)
                    ctr.modify(translation, rotX, rotY)
                    mapping["Constraints"].append(ctr)


                if i.name == "dwn constraint":
                    pos = i.insert
                    contrainte = (i.attribs[0].text).upper()
                    value = tableau_contraintes[contrainte]
                    ctr = Contrainte(pos, value, i.name, contrainte)
                    ctr.modify(translation, rotX, rotY)
                    mapping["Constraints"].append(ctr)

                # pour catcher les waypoint wait-items et les points standarts
                if i.name == "Waypoints":
                    pos = i.insert
                    wpt = WPT(pos, "Waypoints", id_wpt)
                    wpt.modify(translation, rotX, rotY)
                    id_wpt += 1
                    mapping["Waypoints"].append(wpt)

                if i.name == "node wait":
                    pos = i.insert
                    wpt = WPT(pos, "node wait", id_wpt)
                    wpt.modify(translation, rotX, rotY)
                    id_wpt += 1
                    mapping["Waypoints"].append(wpt)

                if i.name == "node standard":
                    pos = i.insert
                    wpt = WPT(pos, "node standard", id_wpt)
                    wpt.modify(translation, rotX, rotY)
                    id_wpt += 1
                    mapping["Waypoints"].append(wpt)

            elif type(i) == Line:
                #pour selectionner les lines de la piste
                start = i.start
                end = i.end
                line = Line_t(start, end, id_leg)
                id_leg += 1
                line.get_point_start().modify(translation, rotX, rotY)
                line.get_point_end().modify(translation, rotX, rotY)
                if line.get_longueur() > 1 :
                    mapping["Legs"].append(line)

            elif type(i) == Arc:
                #pour sélectionner les arc de cercle de la piste
                centre = i.center
                rayon = i.radius
                angle_s = (i.start_angle + rotation) % 360
                angle_e = (i.end_angle + rotation) % 360

                arc = Arc_t(centre, rayon, angle_s, angle_e,id_leg)
                id_leg += 1
                arc.get_centre().modify(translation, rotX, rotY)
                arc.create_start_end()
                if arc.get_longueur() > 1:
                    mapping["Legs"].append(arc)

        if i.layer == "Twirtee-loc" and type(i) is Insert and i.name == "Beacon":
            # pour selectionner les balises radio
            pos = i.insert
            id = i.attribs[0].text
            balise = Balise(pos, id)
            balise.modify(translation, rotX, rotY)
            mapping["Beacons"].append(balise)
            
    return mapping

def construct_leg(mapping):
    """Permet d'ajouter des informations aux legs, notamment connaître les legs voisins pour permettre les choix de trajectoires."""
    size = len(mapping["Legs"])
    for i in range(0,size):
        leg = mapping["Legs"][i]
        for j in range(0,size):
            if j != i:
                leg2 = mapping["Legs"][j]
                if leg.is_at_start(leg2):
                    leg.get_leg_start().append(leg2)
                elif leg.is_at_end(leg2):
                    leg.get_leg_end().append(leg2)

def refine_leg(mapping):
    """Permet de supprimer la connections de legs ayant des angles aigus entre eux"""
    legs = mapping["Legs"]
    iteration = True
    while iteration:
        print("Iteration de désassemblage des legs")
        iteration = False
        for leg in legs:
            vector_leg_s_e = vecteur(leg.get_point_start(),leg.get_point_end())
            vector_leg_e_s = vecteur(leg.get_point_end(),leg.get_point_start())
            for leg_voisin in leg.get_leg_start():
                if leg in leg_voisin.get_leg_start():
                    vector_leg_voisin_s_e = vecteur(leg_voisin.get_point_start(),leg_voisin.get_point_end())
                    if produit_scalaire_norm(vector_leg_e_s, vector_leg_voisin_s_e) < 0:
                        leg.get_leg_start().remove(leg_voisin)
                        leg_voisin.get_leg_start().remove(leg)
                        #print("Legs disconnected")
                        iteration = True
                elif leg in leg_voisin.get_leg_end():
                    vector_leg_voisin_e_s = vecteur(leg_voisin.get_point_end(), leg_voisin.get_point_start())
                    if produit_scalaire_norm(vector_leg_e_s, vector_leg_voisin_e_s) < 0:
                        leg.get_leg_start().remove(leg_voisin)
                        leg_voisin.get_leg_end().remove(leg)
                        #print("Legs disconnected")
                        iteration = True

            for leg_voisin in leg.get_leg_end():
                if leg in leg_voisin.get_leg_start():
                    vector_leg_voisin_s_e = vecteur(leg_voisin.get_point_start(),leg_voisin.get_point_end())
                    if produit_scalaire_norm(vector_leg_s_e, vector_leg_voisin_s_e) < 0:
                        leg.get_leg_end().remove(leg_voisin)
                        leg_voisin.get_leg_start().remove(leg)
                        #print("Legs disconnected")
                        iteration = True
                elif leg in leg_voisin.get_leg_end():
                    vector_leg_voisin_e_s = vecteur(leg_voisin.get_point_end(), leg_voisin.get_point_start())
                    if produit_scalaire_norm(vector_leg_s_e, vector_leg_voisin_e_s) < 0:
                        leg.get_leg_end().remove(leg_voisin)
                        leg_voisin.get_leg_end().remove(leg)
                        #print("Legs disconnected")
                        iteration = True
"""
===============================================================================================================
#
#   
#
#
#
#
#
#
===============================================================================================================
"""
# todo : 
#   
def isnewnode(x,y,base):
    for obj in base:
        if (round(obj[0],4)==round(x,4) and round(obj[1],4)==round(y,4)):
            return obj[2]
        
    return -1


def createBase(legs):
    base=[]
    noderank=0
    for leg in legs:
        if(isnewnode(leg._point_start._x,leg._point_start._y,base)==-1):
            base.append([leg._point_start._x,leg._point_start._y,noderank])
            noderank+=1
        if(isnewnode(leg._point_end._x,leg._point_end._y,base)==-1):
            base.append([leg._point_end._x,leg._point_end._y,noderank])
            noderank+=1
    #print(str(base[0])+","+str(base[1]))
    return base

def getAllNodes(legs):
    nodes=[]
    for leg in legs:
        nodes.append(leg.get_point_start)
    return nodes

def constructArcsNodes2(mapping):
    nodes=[]
    legs=mapping["Legs"]
    for lega in legs:
        allnodes=getAllNodes(legs)
    return nodes

        
def constructArcsNodes(mapping):
    legs = mapping["Legs"]
    nodes=[]
    base=createBase(legs)
    for obj in base:
        n=nodevar(obj[2])
        n._x=round(obj[0],3)
        n._y=round(obj[1],3)
        nodes.append(n)
    for leg in legs:
        id1=isnewnode(leg._point_start._x,leg._point_start._y,base)
        
        id2=isnewnode(leg._point_end._x,leg._point_end._y,base)
        if(id1!=-1 and id2!=-1):
            for node in nodes:

                #finish=0
                if (node._id==id1):
                    node.addId(id2)
                    #finish+=1
                if (node._id==id2):
                    node.addId(id1)
                    #finish+=1
                #if(finish==2):
                    #break
    return nodes
    """
    nb=0
    nc=0
    tt=0
    for node in nodes:
        nb+=1
            
        if(len(node.arcs)!=0):
            for obj in node.arcs:
                val=0
                for objv in node.arcs:
                    if(obj==objv):
                        val+=1
            if val>1:
                print("error, duplicated arc")
            tt+=node._nb_a
            print(node.arcs)
            nc+=1
        #else:
            #print("NULL")
    print(str(tt)+"\t"+str(nc)+"\t"+str(len(nodes)))
    """





        

"""========================================================================================="""


def vecteur(point_a,point_b):
    X = point_b.get_x() - point_a.get_x()
    Y = point_b.get_y() - point_a.get_y()
    return [X,Y]

def construct_waypoints(mapping):
    """Permet de déterminer à quel(s) leg(s) appartient les différents waypoint et supprimer ceux qui ne sont sur aucuns legs"""
    wpt_hors_scope = []
    for wpt in mapping["Waypoints"]:
        is_on_a_leg = False
        for leg in mapping["Legs"]:
            if wpt.distance(leg.get_point_start())<1:
                #pour les waypoints en début de segment
                leg.add_waypoints(wpt,0.0)
                wpt.add_on_leg(leg, 0.0)
                is_on_a_leg = True
            elif wpt.distance(leg.get_point_end())<1:
                # pour les waypoints en fin de segment
                wpt.add_on_leg(leg,leg.get_longueur())
                leg.add_waypoints(wpt, leg.get_longueur())
                is_on_a_leg = True
            else : #il faut trouver s'ils sont sur un leg
                if type(leg) is Line_t: #cas d'une ligne
                    if wpt.is_on_line(leg):
                        distance = wpt.distance(leg.get_point_start())
                        wpt.add_on_leg(leg, distance)
                        leg.add_waypoints(wpt, distance)
                        is_on_a_leg = True

                elif type(leg) is Arc_t:#cas d'un arc
                    reponse = wpt.is_on_arc(leg)
                    if reponse[0]:
                        distance = reponse[1]*leg.get_rayon() #l'angle retournée est en radian
                        wpt.add_on_leg(leg, distance)
                        leg.add_waypoints(wpt, distance)
                        is_on_a_leg = True

        if is_on_a_leg == False:
            wpt_hors_scope.append(wpt)
            print("erreur : ce waypoint n'appartient à aucun leg, il est retiré")

    for wpt in wpt_hors_scope:
        mapping["Waypoints"].remove(wpt)

def construct_contrainte(mapping):
    """Permet de déterminer à quel(s) leg(s) appartient les différentes contraintes et supprimer ceux qui ne sont sur aucuns legs"""
    ctr_hors_scope = []
    for ctr in mapping["Constraints"]:
        is_on_a_leg = False
        for leg in mapping["Legs"]:
            # il faut trouver s'ils sont sur un leg
            if type(leg) is Line_t:  # cas d'une ligne
                if ctr.is_on_line(leg):
                    leg.set_contrainte(ctr)
                    is_on_a_leg = True

            elif type(leg) is Arc_t:  # cas d'un arc
                if ctr.is_on_arc(leg)[0]:
                    leg.set_contrainte(ctr)
                    is_on_a_leg = True

        if is_on_a_leg == False:
            ctr_hors_scope.append(ctr)
            print("erreur : ce waypoint n'appartient à aucun leg, il est retiré")

    for ctr in ctr_hors_scope:
        mapping["Constraints"].remove(ctr)

from Fonctions_extraction import *
import numpy as np

import sys, getopt
import pdb


def getWptId(x,y,Waypoints):
    for obj in Waypoints:
        if(np.absolute(obj._x-x)<0.1 and np.absolute(obj._y-y)<0.1):
            return obj._id
   

def serialiseMapping(mapp):
    data={}
    # ----------------------------------------------------------------------
    data["Constants"]=mapp["Constants"]
    
    # ----------------------------------------------------------------------
    beacons = mapp["Beacons"]
    data["Beacons"] ={}
    table=[]
    for beacon in beacons:
        temp={}
        temp["Beaconid"]=beacon.get_id()
        temp["Beaconx"]=beacon._x
        temp["Beacony"]=beacon._y
        temp["Beaconz"]=beacon._z
        table.append(temp)
    data["Beacons"]=table
    # ----------------------------------------------------------------------   
    nodes = constructArcsNodes(mapp)
    pdb.set_trace()
    data["nodes"] ={}
    table=[]
    for node in nodes:
        temp={}
        temp["nodId"]=node._id
        temp["nodx"]=node._x
        temp["nody"]=node._y
        temp["nodeNbA"]=node._nb_a
        temp["nodeArc1"]=node.arc1
        temp["nodeArc2"]=node.arc2
        temp["nodeArc3"]=node.arc3
        temp["nodeArc4"]=node.arc4
        
        table.append(temp)
    data["nodes"]=table
    # ----------------------------------------------------------------------
    
    Constraints = mapp["Constraints"]
    data["Constraints"] ={}
    table=[]
    cid=0
    for Constraint in Constraints:
        temp={}
        
        
        temp["Cid"]=cid
        temp["Cvalue"]=Constraint.get_value()
        temp["Cname"]=Constraint.get_nom()
        temp["Ctype"]=Constraint.get_type()
        temp["Cx"]=Constraint._x
        temp["Cy"]=Constraint._y
        temp["Cz"]=Constraint._z
        cid+=1

        table.append(temp)
    data["Constraints"]=table
    # ----------------------------------------------------------------------
    Waypoints = mapp["Waypoints"]
    data["Waypoints"] ={}
    table=[]
    
    wpid=1
    for Waypoint in Waypoints:
        temp={}
        temp["wpid"]=wpid
        temp["wptype"]=Waypoint._waypoint
        temp["wpx"]=Waypoint._x
        temp["wpy"]=Waypoint._y
        temp["wpz"]=Waypoint._z
        #print(str(Waypoint._x)+", "+str(Waypoint._y))
        wpid+=1
        table.append(temp)
    data["Waypoints"]=table
    # ----------------------------------------------------------------------
   
    Legs = mapp["Legs"]
    data["Legs"] ={}
    table=[]
    
    legid=0
    for Leg in Legs:
        temp={}
        temp["id"]=legid
        temp["sID"] = getWptId(Leg._point_start._x,Leg._point_start._y,Waypoints)
        temp["eID"] = getWptId(Leg._point_end._x,Leg._point_end._y,Waypoints)
        temp["legLength"]=Leg._longueur
        
               
        legid+=1
        table.append(temp)
    data["Legs"]=table

    
    # ----------------------------------------------------------------------
    # Debugger
    # pdb.set_trace()

    return data

def main(argv):
    #variables et vérification des arguments en ligne de commande
    nom_du_fichier_dxf = '' #input
    nom_du_fichier_json = '' #output
    try:
        opts, args = getopt.getopt(argv, "hi:o:", ["ifile=", "ofile="])
    except getopt.GetoptError:
        print('main_extraction.py -i <inputfile.dxf> -o <outputfile.json>')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print('main_extraction.py -i <inputfile.dxf> -o <outputfile.json>')
            print('OR')
            print('main_extraction.py --ifile=<inputfile.dxf> --ofile=<outputfile.json>')
            sys.exit()
        elif opt in ("-i", "--ifile"):
            nom_du_fichier_dxf = arg
        elif opt in ("-o", "--ofile"):
            nom_du_fichier_json = arg

    if nom_du_fichier_dxf == '' or nom_du_fichier_json == '':
        print('main_extraction.py -i <inputfile.dxf> -o <outputfile.npz>')
        print('OR')
        print('main_extraction.py --ifile=<inputfile.dxf> --ofile=<outputfile.npz>')
        sys.exit()

    print("Fichier d'entrée : %s", nom_du_fichier_dxf)
    print("Fichier de sortie : %s", nom_du_fichier_json)

    #traitement
    print("Chargement du fichier .dxf")

    mapping = load_dxf_file(nom_du_fichier_dxf)

    print("Contruction des legs")
    construct_leg(mapping)

    print("Raffinage des legs")
    refine_leg(mapping)

    print("Construction des Waypoints")
    construct_waypoints(mapping)

    print("Construction des Contraintes")
    construct_contrainte(mapping)

    print("Fin d'extraction")
    """ 
    print("Sauvegarde de l'architecture globale")
    np.savez(nom_du_fichier_json, mapping)
     """

   
    """---------------------------------------------------------------"""
    
    #constructArcsNodes(mapping)

    
    """---------------------------------------------------------------"""
    print("Dumpping map to jSon file")
    data = serialiseMapping(mapping)
    jsonfile = open(nom_du_fichier_json,"w")
    json.dump(data,jsonfile,indent=4)
    jsonfile.close()

if __name__ == "__main__":
    main(sys.argv[1:])

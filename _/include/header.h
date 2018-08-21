#ifndef _head__
#define _head__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>


#include "jsmn.h"

#define CARTO_NODE_NBR	653
#define MAX_NODE_ARCS 4

#define stBeacons	        "Beacons"
    #define stBeaconid      "Beaconid"
    #define stBeaconx		"Beaconx"
    #define stBeacony	    "Beacony"
    #define stBeaconz	    "Beaconz"

#define stnodes	            "nodes"
    #define stNodeId        "nodId"
    #define stnodx		    "nodx"
    #define stnody	        "nody"
    #define stnodeNbA	    "nodeNbA"
    #define stnodeArc1	    "nodeArc1"
    #define stnodeArc2	    "nodeArc2"
    #define stnodeArc3	    "nodeArc3"
    #define stnodeArc4	    "nodeArc4"
    

    
    
#define stConstants	        "Constants"
    #define stvdef          "VDEF"
    #define stadef          "ADEF"
    #define stv1            "V1"
    #define stv2            "V2"

#define stConstraints       "Constraints"
    #define stCid           "Cid"
    #define stCvalue        "Cvalue"
    #define stCname         "Cname"
    #define stCtype         "Ctype"
    #define stCx            "Cx"
    #define stCy            "Cy"
    #define stCz            "Cz"

#define stWaypoints	        "Waypoints"
    #define stwpid          "wpid"
    #define stwptype        "wptype"
    #define stwpx           "wpx"
    #define stwpy           "wpy"
    #define stwpz           "wpz"

#define stLegs		        "Legs"
    #define stlegId         "legId"
    #define stlegLength     "legLength"
    #define stlegStartx     "legStartx"
    #define stlegStarty     "legStarty"
    #define stlegStartz     "legStartz"
    #define stlegEndx       "legEndx"
    #define stlegEndy       "legEndy"
    #define stlegEndz       "legEndz"


/****{ start(from twirtee.h)}****************************************************/
#define false			0
#define true			1
#define INVALID_DATA	0
#define VALID_DATA		1

#define GPS_NOISE_SCALE			0.0	///< default scale is 1.0
#define COMPASS_NOISE_SCALE		0.0	///< default scale is 1.0
#define ODOMETRY_NOISE_SCALE	0.0	///< default scale is 1.0

#define MAX_CARTO_NODES	1000
#define MAX_NODE_ARCS	4

#define PI 3.1415927f
#ifndef M_PI
#define M_PI 3.14159265358979323846264338328
#endif
#define POINTDIST(A,B)  (float)(sqrt((((A).x-(B).x)*((A).x-(B).x)) + (((A).y-(B).y)*((A).y-(B).y))))
#define ABS(A)			((A) >= 0 ? (A) : -(A))
#define SQR(x)			((x)*(x))
#define _MAT_ (double (*)[])
/*
typedef sint8			Bool8;
typedef sint32			Bool32;
typedef uint8			UInt8;
typedef uint16			UInt16;
typedef uint32	 		UInt32;
typedef sint16			Int16;
typedef sint32			Int32;
//typedef sint64			Int64;
typedef float32			Float32;
typedef float64			Double64;
*/


/****{ end(from twirtee.h) }****************************************************/


typedef int8_t			Bool8;
typedef int32_t			Bool32;
typedef uint8_t			UInt8;
typedef unsigned short	UInt16;
typedef uint32_t 		UInt32;
typedef signed short	Int16;
typedef int32_t			Int32;
typedef signed long		Int64;
typedef float 			Float32;
typedef double 			Double64;



typedef enum Node_type {
	DEFAULT = 0,///< standart point
	TARGET,		///< waypoint
	STATION,	///< rest point
	AUTO		///< generated point
} Node_type;

typedef struct legtoarc{
    float x,y;
    int arcsId[MAX_NODE_ARCS];
} legtoarc;
typedef struct Node Node, *Node_p;

typedef struct Arc {
	Node	*dest;
	Float32 max_speed;		///< Max linear speed,		-1.f if unset
	Float32 max_speed_up;	///< Max linear speed-up,	-1.f if unset
}Arc;

typedef struct Node {
    int id;
	Float32 x, y;
	Node_type nt;
	UInt8 nb_a;
	Arc arcs[MAX_NODE_ARCS];
    int ids[MAX_NODE_ARCS];
}Node;

typedef struct Cartography{
	Float32 def_max_speed;
	Float32 def_max_speed_up;
	Int16 nb_arcs;
	Int16 nb_nodes;
	Node nodes[MAX_CARTO_NODES];
} Cartography;

typedef struct Path {
	Float32 def_max_speed;
	Float32 def_max_speed_up;
	UInt16 size;
	Node *dest[MAX_CARTO_NODES];
} Path;

typedef struct Mission {
	int ind[CARTO_NODE_NBR];
	UInt16 size;
} Mission;

typedef struct initParser {
    int r;
	jsmn_parser p;
	jsmntok_t * t;
} initParser;

typedef struct Beacons {
    int     id;
    float    x;
    float    y;
    float    z; 
}Beacons;

typedef struct Constants
{
    float vdef;
    float adef;
    float v1;
    float v2;
} Constants;

typedef struct Constraints
{
    int id;
    float value;
    char *name;
    char *type;
    float    x;
    float    y;
    float    z; 
}Constraints ;

typedef struct Waypoints
{
    int id;
    char *type;
    float    x;
    float    y;
    float    z; 
}Waypoints ;


typedef struct Legs
{
    int       id;
    double    length;
    double    startx;
    double    starty;
    double    startz;    
    double    endx;
    double    endy;
    double    endz;
}Legs ;


struct Beacons beaconsExt(char *_JSON_STRING,initParser _IP,int _i,int objRank);
struct Node nodesExt(char *_JSON_STRING,initParser _IP,int _i,int objRank);
struct Waypoints waypointsExt(char *_JSON_STRING,initParser _IP,int _i,int objRank);
struct Legs legsExt(char *_JSON_STRING,initParser _IP,int _i,int objRank);
struct Constraints ConstrExt(char *_JSON_STRING,initParser _IP,int _i,int objRank);
struct Constants CstExt(char *_JSON_STRING,initParser _IP,int _i);
//struct Node * convertLegs(Legs legs[],int occurL);
struct initParser getJsonToken(int expectNvalues,char * JSON_STRING);


int jsoneq(const char *json, jsmntok_t *tok, const char *s);
int countNodes(Legs * l,int nlegs);
int objectOccurance(char * objectName,char * JSON_STRING, initParser IP);
int objectRank(char * objectName,char * JSON_STRING, initParser IP);
int nextObjectTab(char * objectName,char * JSON_STRING, initParser IP);





#endif
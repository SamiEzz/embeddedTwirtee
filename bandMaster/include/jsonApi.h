#ifndef __JSMN_H_
#define __JSMN_H_

#include "twirtee.h"
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CARTO_NODE_NBR 653
#define MAX_NODE_ARCS 4 // obsolete
#define EPSILONodes 0.01


/****{ start(from twirtee.h)}****************************************************/

#ifndef  false
#define false			0
#endif 
#ifndef  true
#define true			1
#endif 
#ifndef  FALSE
#define FALSE			0
#endif 
#ifndef  TRUE
#define TRUE			1
#endif 
#ifndef  INVALID_DATA
#define INVALID_DATA	0
#endif 
#ifndef  VALID_DATA
#define VALID_DATA		1
#endif 
#ifndef  GPS_NOISE_SCALE
#define GPS_NOISE_SCALE			0.0	///< default scale is 1.0
#endif 
#ifndef  COMPASS_NOISE_SCALE
#define COMPASS_NOISE_SCALE		0.0	///< default scale is 1.0
#endif 
#ifndef  ODOMETRY_NOISE_SCALE
#define ODOMETRY_NOISE_SCALE	0.0	///< default scale is 1.0
#endif 
#ifndef  MAX_CARTO_NODES
#define MAX_CARTO_NODES	1000
#endif 
#ifndef  MAX_NODE_ARCS
#define MAX_NODE_ARCS	4
#endif 
#ifndef  PI
#define PI 3.1415927f
#endif 
#ifndef  M_PI
#define M_PI 3.14159265358979323846264338328
#endif
#ifndef  POINTDIST
#define POINTDIST(A,B)  (float)(sqrt((((A).x-(B).x)*((A).x-(B).x)) + (((A).y-(B).y)*((A).y-(B).y))))
#endif 
#ifndef  ABS
#define ABS(A)			((A) >= 0 ? (A) : -(A))
#endif 
#ifndef  SQR
#define SQR(x)			((x)*(x))
#endif 
#ifndef  _MAT_
#define _MAT_ (double (*)[])
#endif 

/****{ end(from twirtee.h) }****************************************************/

// typedef int8_t			Bool8;
// typedef int32_t			Bool32;
// typedef uint8_t			UInt8;
// typedef unsigned short	UInt16;
// typedef uint32_t 		UInt32;
// typedef signed short	Int16;
// typedef int32_t			Int32;
// typedef signed long		Int64;
// typedef float 			Float32;
// typedef double 			Double64;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * JSON type identifier. Basic types are:
 * 	o Object
 * 	o Array
 * 	o String
 * 	o Other primitive: number, boolean (true/false) or null
 */
typedef enum {
    JSMN_UNDEFINED = 0,
    JSMN_OBJECT = 1,
    JSMN_ARRAY = 2,
    JSMN_STRING = 3,
    JSMN_PRIMITIVE = 4
} jsmntype_t;

enum jsmnerr {
    /* Not enough tokens were provided */
    JSMN_ERROR_NOMEM = -1,
    /* Invalid character inside JSON string */
    JSMN_ERROR_INVAL = -2,
    /* The string is not a full JSON packet, more bytes expected */
    JSMN_ERROR_PART = -3
};

/**
 * JSON token description.
 * type		type (object, array, string etc.)
 * start	start position in JSON data string
 * end		end position in JSON data string
 */
typedef struct {
    jsmntype_t type;
    int start;
    int end;
    int size;
#ifdef JSMN_PARENT_LINKS
    int parent;
#endif
} jsmntok_t;

/**
 * JSON parser. Contains an array of token blocks available. Also stores
 * the string being parsed now and current position in that string
 */
typedef struct {
    unsigned int pos;     /* offset in the JSON string */
    unsigned int toknext; /* next token to allocate */
    int toksuper;         /* superior token node, e.g parent object or array */
} jsmn_parser;

/**
 * Create JSON parser over an array of tokens
 */
void jsmn_init(jsmn_parser* parser);

/**
 * Run JSON parser. It parses a JSON data string into and array of tokens, each describing
 * a single JSON object.
 */
int jsmn_parse(jsmn_parser* parser, const char* js, size_t len, jsmntok_t* tokens,
               unsigned int num_tokens);

#ifdef __cplusplus
}
#endif

//---------------------------------------------------

// typedef enum Node_type {
// 	DEFAULT = 0,///< standart point
// 	TARGET,		///< waypoint
// 	STATION,	///< rest point
// 	AUTO		///< generated point
// } Node_type;

typedef struct legtoarc {
    float x, y;
    int arcsId[MAX_NODE_ARCS];
} legtoarc;
typedef struct Node Node, *Node_p;

// typedef struct Arc {
// 	Node	*dest;
// 	Float32 max_speed;		///< Max linear speed,		-1.f if unset
// 	Float32 max_speed_up;	///< Max linear speed-up,	-1.f if unset
// }Arc;

// typedef struct Node {
//     int id;
// 	Float32 x, y;
// 	Node_type nt;
// 	UInt8 nb_a;
// 	Arc arcs[MAX_NODE_ARCS];
//     int ids[MAX_NODE_ARCS];
// }Node;

typedef struct initParser {
    int r;
    jsmn_parser p;
    jsmntok_t* t;
} initParser;

typedef struct Beacons {
    int id;
    float x;
    float y;
    float z;
} Beacons;

typedef struct Constants {
    float vdef;
    float adef;
    float v1;
    float v2;
} Constants;

typedef struct Constraints {
    int id;
    float value;
    char* name;
    char* type;
    float x;
    float y;
    float z;
} Constraints;

typedef struct Waypoints {
    int id;
    char* type;
    float x;
    float y;
    float z;
} Waypoints;

typedef struct Legs {
    int id;
    double length;
    double startx;
    double starty;
    double startz;
    double endx;
    double endy;
    double endz;
} Legs;

typedef struct Base {
    Node* _nd;
    Beacons* _b;
    Waypoints* _wpt;
    Legs* _lg;
    Constraints* _ct;
    Constants _c;
} Base;

typedef struct occur {
    int nodes;
    int beacons;
    int waypoints;
    int legs;
    int Constraints;

} occur;

typedef struct jdata {
    struct Base* base;
    struct occur* occur;
} jdata;
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

void countJson(int* a);
struct Beacons beaconsExt(char* _JSON_STRING, initParser _IP, int _i, int objRank);
struct Node nodesExt(char* _JSON_STRING, initParser _IP, int _i, int objRank);
struct Waypoints waypointsExt(char* _JSON_STRING, initParser _IP, int _i, int objRank);
struct Legs legsExt(char* _JSON_STRING, initParser _IP, int _i, int objRank);
struct Constraints ConstrExt(char* _JSON_STRING, initParser _IP, int _i, int objRank);
struct Constants CstExt(char* _JSON_STRING, initParser _IP, int _i);
// struct Node * convertLegs(Legs legs[],int occurL);
struct initParser getJsonToken(int expectNvalues, char* JSON_STRING);
void jsonMainExtract(Base _base, char* jsonFile);
struct Base initBase();
void importData(jdata* data);

Node getnodebyid(jdata* data, int id);
void printnode(Node* n);
void traject_to_file(Node* n, int rank, char* outputFile);
void countJson(int* a);
int jsoneq(const char* json, jsmntok_t* tok, const char* s);
int countNodes(Legs* l, int nlegs);
int objectOccurance(char* objectName, char* JSON_STRING, initParser IP);
int objectRank(char* objectName, char* JSON_STRING, initParser IP);
int nextObjectTab(char* objectName, char* JSON_STRING, initParser IP);
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

#endif /* __JSMN_H_ */

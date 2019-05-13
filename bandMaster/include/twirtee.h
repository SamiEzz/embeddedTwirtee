
/*
 * twirtee.h
 *
 *  Created on: 11 janv. 2018
 *      Author: twirteeop
 */

#ifndef TWIRTEE_H_
#define TWIRTEE_H_

#include <stdint.h>
#include <pthread.h>

//#include "platform_types.h"


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

/*
 * AUTOSAR integer data types
 */
typedef signed char    sint8;               /*        -128 .. +127            */
typedef unsigned char  uint8;               /*           0 .. 255             */
typedef signed short   sint16;              /*      -32768 .. +32767          */
typedef unsigned short uint16;              /*           0 .. 65535           */
typedef signed long    sint32;              /* -2147483648 .. +2147483647     */
typedef unsigned long  uint32;              /*           0 .. 4294967295      */
typedef float          float32;
typedef double         float64;

typedef unsigned long  uint8_least;         /* At least 8 bit                 */
typedef unsigned long  uint16_least;        /* At least 16 bit                */
typedef unsigned long  uint32_least;        /* At least 32 bit                */
typedef signed long    sint8_least;         /* At least 7 bit + 1 bit sign    */
typedef signed long    sint16_least;        /* At least 15 bit + 1 bit sign   */
typedef signed long    sint32_least;        /* At least 31 bit + 1 bit sign   */

typedef unsigned char  boolean;   
//-----------------------

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

/****{ Included in model }****************************************************/

typedef struct PWM {
	Int16 regulation_right;
	Int16 regulation_left;
} PWM;

typedef struct Position {
	Float32 x;
	Float32 y;
	Float32 theta;
} Position;

typedef struct T_loc {
	Double64 x;
	Double64 y;
	Bool8 val;
} T_loc;

// Compass struct read by POW
typedef struct Magnetometer {
	Float32 x;
	Float32 y;
} Magnetometer;

// Compass struct sent by POW to COM
typedef struct T_head {
	Double64 theta;
	Bool8 val;
} T_head;

typedef struct T_odo {
	double l_speed;
	double r_speed;
	Float32 quad_delta_time_left;
	Float32 quad_delta_time_right;
	Int32 quad_delta_count_left;
	Int32 quad_delta_count_right;
	Bool8 val;
} T_odo;

typedef struct Modes_Req_Struct {
	Bool8 Ctrl_Mode;
	Bool8 Rem_Arm_Mode;
	Bool8 Auto_Arm_Mode;
} Modes_Req_Struct;

typedef struct Speed {
	Float32 v; ///< linear speed
	Float32 w; ///< angular speed (omega)
	Bool8 val;
} Speed;

/****{ Not in AADL model }****************************************************/

typedef struct T_mat {
	Int32 l;		///< Lines   number
	Int32 c;		///< Columns number
	Double64 (*m)[];
} T_mat;

typedef enum Node_type {
	DEFAULT = 0,///< standart point
	TARGET,		///< waypoint
	STATION,	///< rest point
	AUTO		///< generated point
} Node_type;

typedef struct Arc Arc;


typedef struct Node Node, *Node_p;
struct Arc {
	Node	*dest;
	Float32 max_speed;		///< Max linear speed,		-1.f if unset
	Float32 max_speed_up;	///< Max linear speed-up,	-1.f if unset
};

typedef struct Node {
    int id;
	Float32 x, y;
	Node_type nt;
	UInt8 nb_a;
	Arc arcs[MAX_NODE_ARCS];
    int ids[MAX_NODE_ARCS];
}Node;
/*
struct Node {
	Float32 x, y;
	Node_type nt;
	UInt8 nb_a;
	Arc arcs[MAX_NODE_ARCS];
};*/

typedef struct Vector { ///< used in COM/Tracking/Mission_mgt
	Float32 x, y;
} Vector;

typedef struct Point { ///< used in COM/Tracking/Segment
	Float32 x, y;
} Point;

typedef struct Segment { ///< used in COM/Tracking/Segment
	Point p1, p2;
} Segment;
typedef struct Path
{
	Float32 def_max_speed;
	Float32 def_max_speed_up;
	UInt16 size;
	Node *dest[MAX_CARTO_NODES];
} Path;

typedef struct Mission {
	UInt16 size;
	int ind[MAX_CARTO_NODES];
} Mission;
typedef struct Cartography{
	Float32 def_max_speed;
	Float32 def_max_speed_up;
	Int16 nb_arcs;
	Int16 nb_nodes;
	Node *nodes;
} Cartography;


typedef struct spf_mission{
    int start;
    int end;
    Path * path;
    pthread_mutex_t mut;
}spf_mission;

typedef struct {
    pthread_mutex_t mut;
    Path mission_var;
} mission_mtx;
typedef struct can_shared{
    char* can_name;
    unsigned int id[100];
    unsigned char data[100][256];
    int available;
    pthread_mutex_t mutex;
}can_shared;

#endif /* TWIRTEE_H_ */
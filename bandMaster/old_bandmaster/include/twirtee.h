/*
 * twirtee.h
 *
 *  Created on: 11 janv. 2018
 *      Author: twirteeop
 */

#ifndef TWIRTEE_H_
#define TWIRTEE_H_

#include <stdint.h>
#include "platform_types.h"


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


#endif /* TWIRTEE_H_ */

/**
******************************************************************************
* @file		Kanayama.c
* @coauthor	[IRT] RIPOLL Guillaume
* @version	1.0
* @brief	Implementation of Kanayama
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "com.h"
#include "kanayama.h"
#include <math.h>

/* Private function prototypes -----------------------------------------------*/
static void change_coordinate_system(Position* vout, Position vin, float matrix[3][3]);

/* Private functions ---------------------------------------------------------*/
void get_error_vector(Position pos, Position setpoint, Position* error) {
	Position errorW;
	float matrix[3][3] = {{0.0f}};
	matrix[0][0] = (float)cos(pos.theta);
	matrix[0][1] = (float)sin(pos.theta);
	matrix[1][0] = (float)-sin(pos.theta);
	matrix[1][1] = (float)cos(pos.theta);
	matrix[2][2] = 1.0f;

	/* Compute error in the absolute coordinate system */
	errorW.x = setpoint.x - pos.x;
	errorW.y = setpoint.y - pos.y;
	errorW.theta = setpoint.theta - pos.theta;

	/* Get error in the robot coordinate system */
	change_coordinate_system(error, errorW, matrix);
}

void get_ref_speed(Position setpoint, Position prev_setpoint, float* vr, float* wr) {
	Position speed_R, speed_W;
	float matrix[3][3] = {{0.0f}};
	matrix[0][0] = (float)cos(prev_setpoint.theta);
	matrix[0][1] = (float)sin(prev_setpoint.theta);
	matrix[1][0] = (float)-sin(prev_setpoint.theta);
	matrix[1][1] = (float)cos(prev_setpoint.theta);
	matrix[2][2] = 1.0f;

	speed_W.x = (float)((setpoint.x - prev_setpoint.x)*CALL_PER_SEC);
	speed_W.y = (float)((setpoint.y - prev_setpoint.y)*CALL_PER_SEC);
	speed_W.theta = (float)((setpoint.theta - prev_setpoint.theta)*CALL_PER_SEC);

	change_coordinate_system(&speed_R, speed_W, matrix);

	*vr = (float)sqrt((speed_R.x*speed_R.x) + (speed_R.y*speed_R.y));
	*wr = speed_W.theta;
}

void kanayama(float vr, float wr, Position error, float* v, float* w) {
	const float KX = 1.0f;
	const float KY = 0.4f;
	const float KTHETA = 0.075f;

	/* Apply control law */
	*v = vr * cos(error.theta) + KX*error.x;
	*w = wr + vr * (KY*error.y + KTHETA*sin(error.theta));
}

static void change_coordinate_system(Position* vout, Position vin, float matrix[3][3]) {
	/** Computations */
	vout->x		= matrix[0][0]*vin.x + matrix[0][1]*vin.y + matrix[0][2]*vin.theta;
	vout->y		= matrix[1][0]*vin.x + matrix[1][1]*vin.y + matrix[1][2]*vin.theta;
	vout->theta	= matrix[2][0]*vin.x + matrix[2][1]*vin.y + matrix[2][2]*vin.theta;
}

/*
 * twirtee.c
 *
 *  Created on: 26 janv. 2018
 *      Author: GPH
 */
#include "simulator.h"
#include <math.h>
#include "parametres.h"
#include "measurement_randomization.h"
#include "guidance_mgr.h"

double	v = 0.0,
		w = 0.0;

/**
	@brief	Simulating function : Update and trace positions
			aiming to let tracking work without need of any other process
	@param	IN  end_request : true only the last call, so it can proper close the GNUPLOT_FILE
	@param	IN  speed_gui   : used to simulate new position
	@param	OUT gps : simulates new position at device  level
	@param	OUT p	: simulates new position at process level
 */
void update_simulation(Bool8 end_request, Speed speed_gui, Position pos_sp, Position kalm_res, Position *p, T_head *compass, T_loc *gps, T_odo *odometry){
//	static int counter = 0;

	if(! end_request)
	{
		/* the simulation aims to test the guidance_manager results, v & w are also the speed results of it */
		v = speed_gui.v >= 0 ? speed_gui.v : 0;
		w = fmod(speed_gui.w, (2*M_PI)*(1/DELTA_TIME));
		/* update current position considering v, w & DELTA_TIME */
		p->x = p->x + (v*DELTA_TIME*cos(p->theta + w*DELTA_TIME));
		p->y = p->y + (v*DELTA_TIME*sin(p->theta + w*DELTA_TIME));
		p->theta = p->theta + w*DELTA_TIME;

		compass->theta = p->theta;
		compass->val = VALID_DATA;
		gps->x = p->x;
		gps->y = p->y;
		gps->val = VALID_DATA;
		odometry->l_speed = (-v + K_rp.width / 2.0 * w) * 2.0 / K_rp.rwd;
		odometry->r_speed = (-v - K_rp.width / 2.0 * w) * 2.0 / K_rp.lwd;

		/* some "noise" is added to the sensor values */
		sensor_data_randomize(gps, compass, odometry, TRUE);
//		if (counter % 4 == 0)
//		{
//			compass->val = VALID_DATA;
//		}
//		else
//		{
//			compass->val = INVALID_DATA;
//		}
//		if (counter % 10 == 0)
//		{
//			gps->val = VALID_DATA;
//		}
//		else
//		{
//			gps->val = INVALID_DATA;
//		}
//
//		++counter;
	}
}

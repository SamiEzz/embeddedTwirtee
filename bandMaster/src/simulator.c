/*
 * twirtee.c
 *
 *  Created on: 26 janv. 2018
 *      Author: GPH
 */
#include "../include/simulator.h"
#include "../include/guidance_mgr.h"
#include "../include/measurement_randomization.h"
#include "../include/parametres.h"
#include <math.h>

double v = 0.0, w = 0.0;

/**
        @brief	Simulating function : Update and trace positions
                        aiming to let tracking work without need of any other process
        @param	IN  simu->end_request : true only the last call, so it can proper close the
   GNUPLOT_FILE
        @param	IN  simu->speed_gui   : used to simulate new position
        @param	OUT simu->gps : simulates new position at device  level
        @param	OUT p	: simulates new position at process level
 */
void* robot(void* _state) {
    simu_param* state = _state;
    //	static int counter = 0;

    if (!state->end_request) {
        /* the statelation aims to test the guidance_manager results, v & w are also the speed
         * results of it */

        v = state->speed_gui.v >= 0 ? state->speed_gui.v : 0;
        w = fmod(state->speed_gui.w, (2 * M_PI) * (1 / DELTA_TIME));
        /* update current position considering v, w & DELTA_TIME */
        state->p->x = state->p->x + (v * DELTA_TIME * cos(state->p->theta + w * DELTA_TIME));
        state->p->y = state->p->y + (v * DELTA_TIME * sin(state->p->theta + w * DELTA_TIME));
        state->p->theta = state->p->theta + w * DELTA_TIME;

        state->compass->theta = state->p->theta;
        state->compass->val = VALID_DATA;
        state->gps->x = state->p->x;
        state->gps->y = state->p->y;
        state->gps->val = VALID_DATA;
        state->odometry->l_speed = (-v + K_rp.width / 2.0 * w) * 2.0 / K_rp.rwd;
        state->odometry->r_speed = (-v - K_rp.width / 2.0 * w) * 2.0 / K_rp.lwd;
        //printf()
        /* some "noise" is added to the sensor values */
        sensor_data_randomize(state->gps, state->compass, state->odometry, TRUE);
        //		if (counter % 4 == 0)
        //		{
        //			simu->compass->val = VALID_DATA;
        //		}
        //		else
        //		{
        //			simu->compass->val = INVALID_DATA;
        //		}
        //		if (counter % 10 == 0)
        //		{
        //			simu->gps->val = VALID_DATA;
        //		}
        //		else
        //		{
        //			simu->gps->val = INVALID_DATA;
        //		}
        //
        //		++counter;
    }
}
void* update_simulation(void* __simu) {
    simu_param* simu = __simu;
    //	static int counter = 0;

    if (!simu->end_request) {
        /* the simulation aims to test the guidance_manager results, v & w are also the speed
         * results of it */
        v = simu->speed_gui.v >= 0 ? simu->speed_gui.v : 0;
        w = fmod(simu->speed_gui.w, (2 * M_PI) * (1 / DELTA_TIME));
        /* update current position considering v, w & DELTA_TIME */
        simu->p->x = simu->p->x + (v * DELTA_TIME * cos(simu->p->theta + w * DELTA_TIME));
        simu->p->y = simu->p->y + (v * DELTA_TIME * sin(simu->p->theta + w * DELTA_TIME));
        simu->p->theta = simu->p->theta + w * DELTA_TIME;

        simu->compass->theta = simu->p->theta;
        simu->compass->val = VALID_DATA;
        simu->gps->x = simu->p->x;
        simu->gps->y = simu->p->y;
        simu->gps->val = VALID_DATA;
        simu->odometry->l_speed = (-v + K_rp.width / 2.0 * w) * 2.0 / K_rp.rwd;
        simu->odometry->r_speed = (-v - K_rp.width / 2.0 * w) * 2.0 / K_rp.lwd;

        /* some "noise" is added to the sensor values */
        sensor_data_randomize(simu->gps, simu->compass, simu->odometry, TRUE);
        //		if (counter % 4 == 0)
        //		{
        //			simu->compass->val = VALID_DATA;
        //		}
        //		else
        //		{
        //			simu->compass->val = INVALID_DATA;
        //		}
        //		if (counter % 10 == 0)
        //		{
        //			simu->gps->val = VALID_DATA;
        //		}
        //		else
        //		{
        //			simu->gps->val = INVALID_DATA;
        //		}
        //
        //		++counter;
    }
}

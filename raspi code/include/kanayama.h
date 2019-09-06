/*
 * Kanayama.h
 *
 *  Created on: Sep 26, 2016
 *	  Author: mf.sassi
 */
#ifndef KANAYAMA_H_
#define KANAYAMA_H_

#define TRACKING_PERIOD 100.0f /* in ms */
#define CALL_PER_SEC 1000.0f / TRACKING_PERIOD

void get_error_vector(Position pos, Position setpoint, Position* error);
void get_ref_speed(Position setpoint, Position prev_setpoint, float* vr, float* wr);
void kanayama(float vr, float wr, Position error, float* v, float* w);

#endif /* KANAYAMA_H_ */

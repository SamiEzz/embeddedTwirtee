#ifndef __MEASUREMENT_RANDOMIZATION_H_
#define __MEASUREMENT_RANDOMIZATION_H_

#include "twirtee.h"

#define FREQ_PRECISION 10

typedef struct {
    float invalid_gps_data;
    float noised_gps_data;
    float gps_noise_amplitude;
    float invalid_compass_data;
    float noised_compass_data;
    float compass_noise_amplitude;
    float invalid_odometry_data;
    float noised_odometry_data;
    float odometry_noise_amplitude;
} Randomization_Parameters;

void init_random(void);

/* GPS RANDOMIZATION */
/**
INPUTS:
        * gps -> pointer to current GPS data
        * freq_val -> frequency of occurrence of INVALID GPS data
        * noise_amplitude -> maximal distance between original and NOISED GPS coordinates
        * freq_noise -> frequency of occurrence of NOISED GPS data
FUNCTION RESULTS in potentially modified validity and coordinates for gps
*/
void random_gps(T_loc* gps, float freq_val, float noise_amplitude, float freq_noise);

/* COMPASS RANDOMIZATION */
/**
INPUTS:
        * compass -> pointer to current COMPASS data
        * freq_val -> frequency of occurrence of INVALID COMPASS data
        * noise_amplitude_deg -> maximal angle (in deg.) between original and NOISED COMPASS angle
        * freq_noise -> frequency of occurrence of NOISED COMPASS data
FUNCTION RESULTS in potentially modified validity and coordinates for compass
*/
void random_compass(T_head* compass, float freq_val, float noise_amplitude_deg, float freq_noise);

/* ODOMETER RANDOMIZATION */
/**
INPUTS:
        * odometry -> pointer to current ODOMETRY data
        * freq_val -> frequency of occurrence of INVALID ODOMETRY data
        * noise_amplitude_deg -> maximal angle (in deg.) between original and NOISED ODOMETRY angle
speeds
        * freq_noise -> frequency of occurrence of NOISED COMPASS data
FUNCTION RESULTS in potentially modified validity and coordinates for odometry
*/
void random_odometry(T_odo* odometry, float freq_val, float noise_amplitude_deg, float freq_noise);

void sensor_data_randomize(T_loc* gps, T_head* compass, T_odo* odometry, Bool8 randomize);

#endif /* __MEASUREMENT_RANDOMIZATION_H_ */

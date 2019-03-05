#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

//#include "SysSe/Bsp/Bsp.h"
//#include "CPU2/app_Serial.h"

#include "measurement_randomization.h"

Randomization_Parameters input_random_param;

void sendMsg_CPU2(char * msg){
	printf("\n%s",msg);
}
void init_random(void)
{
	//srand((unsigned) now());
	srand(time(NULL));
}

// int random_int_old(int min, int max)
// {
// 	int n = max - min + 1;
// 	int remainder = RAND_MAX % n;
// 	int x;
// 	do
// 	{
// 		x = rand();
// 	} while (x >= RAND_MAX - remainder);
// 	return min + x % n;
// }
int random_int(int min, int max)
{
	
	return min + 1;
}
/* GPS RANDOMIZATION */
void random_gps(T_loc *gps, float freq_val, float noise_amplitude, float freq_noise)
{
	if ((freq_val > 1.0) || (freq_val < 0.0))
	{
		sendMsg_CPU2("ERROR in RANDOM_GPS -> FREQ_VAL must be in [0.0, 1.0]!!!\n");
		return;
	}
	
	int freq_bound = (int) (freq_val * FREQ_PRECISION + 0.5);//round
	int random_value = random_int(0, 10);
	if ((random_value % FREQ_PRECISION) < freq_bound)
	{
		gps->val = INVALID_DATA;
	}
	else
	{
		gps->val = VALID_DATA;
	}

	if ((freq_noise > 1.0) || (freq_noise < 0.0) || (noise_amplitude < 0.0))
	{
		sendMsg_CPU2("ERROR in RANDOM_GPS -> FREQ_NOISE must be in [0.0, 1.0] and NOISE_AMPLITUDE >= 0.0!!!\n");
		return;
	}

	int freq_bound2 = (int) (freq_noise * FREQ_PRECISION + 0.5);//round
	int amplitude_bound = (int) (noise_amplitude * 100 + 0.5);//round
	random_value = random_int(0, 10);
	if ((random_value % FREQ_PRECISION) < freq_bound2)
	{
		gps->x = gps->x + random_int(-amplitude_bound, amplitude_bound)/100.0;
		gps->y = gps->y + random_int(-amplitude_bound, amplitude_bound)/100.0;
	}
}

/* COMPASS RANDOMIZATION */
void random_compass(T_head *compass, float freq_val, float noise_amplitude_deg, float freq_noise)
{
	if ((freq_val > 1.0) || (freq_val < 0.0))
	{
		sendMsg_CPU2("ERROR in RANDOM_COMPASS -> FREQ_VAL must be in [0.0, 1.0]!!!\n");
		return;
	}
	
	int freq_bound = (int) (freq_val * FREQ_PRECISION + 0.5);//round
	int random_value = random_int(0, 10);
	if ((random_value % FREQ_PRECISION) < freq_bound)
	{
		compass->val = INVALID_DATA;
	}
	else
	{
		compass->val = VALID_DATA;
	}

	if ((freq_noise > 1.0) || (freq_noise < 0.0) || (noise_amplitude_deg < 0.0) || (noise_amplitude_deg >= 360.0))
	{
		sendMsg_CPU2("ERROR in RANDOM_COMPASS -> FREQ_NOISE must be in [0.0, 1.0] and NOISE_AMPLITUDE must be in [0.0, 360.0]!!!\n");
		return;
	}

	float noise_amplitude = noise_amplitude_deg * PI / 180.0;//rad.
	int freq_bound2 = (int) (freq_noise * FREQ_PRECISION + 0.5);//round
	int amplitude_bound = (int) (noise_amplitude * 100 + 0.5);//round
	random_value = random_int(0, 10);
	if ((random_value % FREQ_PRECISION) < freq_bound2)
	{
		compass->theta = compass->theta + random_int(-amplitude_bound, amplitude_bound)/100.0;
	}
}

/* ODOMETRY RANDOMIZATION */
void random_odometry(T_odo *odometry, float freq_val, float noise_amplitude, float freq_noise)
{
	if ((freq_val > 1.0) || (freq_val < 0.0))
	{
		sendMsg_CPU2("ERROR in RANDOM_ODOMETRY -> FREQ_VAL must be in [0.0, 1.0]!!!\n");
		return;
	}
	
	int freq_bound = (int) (freq_val * FREQ_PRECISION + 0.5);//round
	int random_value = random_int(0, 10);
	if ((random_value % FREQ_PRECISION) < freq_bound)
	{
		odometry->val = INVALID_DATA;
	}
	else
	{
		odometry->val = VALID_DATA;
	}

	if ((freq_noise > 1.0) || (freq_noise < 0.0) || (noise_amplitude < 0.0))
	{
		sendMsg_CPU2("ERROR in RANDOM_ODOMETRY -> FREQ_NOISE must be in [0.0, 1.0] and NOISE_AMPLITUDE >= 0.0!!!\n");
		return;
	}

	int freq_bound2 = (int) (freq_noise * FREQ_PRECISION + 0.5);//round
	int amplitude_bound = (int) (noise_amplitude * 100 + 0.5);//round
	random_value = random_int(0, 10);
	if ((random_value % FREQ_PRECISION) < freq_bound2)
	{
		odometry->l_speed = odometry->l_speed + random_int(-amplitude_bound, amplitude_bound)/100.0;
		odometry->r_speed = odometry->r_speed + random_int(-amplitude_bound, amplitude_bound)/100.0;
	}
}

void sensor_data_randomize(T_loc *gps, T_head *compass, T_odo *odometry, Bool8 randomize)
{
	static Bool8 is_init = TRUE;

	if (is_init)
	{
		init_random();
		is_init = FALSE;
	}

	if (randomize)
	{
		/* COMPASS data randomization */
		random_compass(compass,
				input_random_param.invalid_compass_data,
				input_random_param.compass_noise_amplitude,
				input_random_param.noised_compass_data);
		/* GPS data randomization */
		random_gps(gps,
				input_random_param.invalid_gps_data,
				input_random_param.gps_noise_amplitude,
				input_random_param.noised_gps_data);
		/* ODOMETRY data randomization */
		random_odometry(odometry,
				input_random_param.invalid_odometry_data,
				input_random_param.odometry_noise_amplitude,
				input_random_param.noised_odometry_data);
	}
}

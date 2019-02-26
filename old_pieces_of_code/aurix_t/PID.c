#include "CPU2/Misc.h"
#include "CPU2/PID.h"
#include "CPU2/Can.h"
#include <Cpu/Std/Platform_Types.h>

const int MAX_ITG = (100.0 /(KI*DELTA_TIME));

int PID(int speed_setpoint, SIDE side) { ///< speed in rotations per second
	static float	itg[2] = {0.0f},
					der[2] = {0.0f};
	static unsigned int odometry[2] = {0};
	static unsigned int nb_ite = 0;
	float	err = 0,
			res;
	int count = get_odometry(side, TRUE);

	odometry[side] += count;
	err = 100*(speed_setpoint - count)/MOTOR_RPS;
	itg[side] += err;
	itg[side] = itg[side] <= MAX_ITG ? itg[side] : MAX_ITG;
	der[side] = err - der[side];
	res = KP*err + (KI*itg[side]*DELTA_TIME) + (KD*der[side] /DELTA_TIME);
	der[side] = err;

	if((nb_ite % FREQ_PRINT) == 0) {
		if(side == LEFT){ // Serial trace
			char l_print_speed[MAX_LINE_SIZE] = "Speed[LEFT] = 00000000000\r\n";
			const int L_IND = 14;
			int_to_str(count, L_IND, l_print_speed);
			sendMsg_CPU2(l_print_speed);
		} else {
			char r_print_speed[MAX_LINE_SIZE] = "Speed[RIGHT] = 00000000000\r\n";
			const int R_IND = 15;
			int_to_str(count, R_IND, r_print_speed);
			sendMsg_CPU2(r_print_speed);
		}
	}
	if((nb_ite % FREQ_SEND) == 0) {
		/* Give both counters values at same time requires a nice precision,
		 * so even if for 1 side get_odometry was called at start of the function,
		 * having the 2 lines close ensure a better precision.
		 **/
		odometry[LEFT]	+= get_odometry(LEFT, FALSE);
		odometry[RIGHT]	+= get_odometry(RIGHT, FALSE);
		CAN_init_msg(CAN_NODE_1, SEND, O_ODOMETRY);
		CAN_send_odometry(CAN_NODE_1, odometry[LEFT], odometry[RIGHT]);
		CAN_init_msg(CAN_NODE_1, RECEIVE, I_SPEED_SP);
	}
	++nb_ite;
	return (int) res;
}

/**
 * @brief return 0 at first call, then
 * 		  the motor rotations per second from the given side
 * @param side is used to purpose the same behavior for each side
 *
 * FIXME: the return value should be changed to angel or position or what-so-ever in the future
*/
int get_odometry(SIDE side, boolean new_period) { ///< rotations per second
	int count;
	if(side == LEFT)
		count = 0xCAFEBABE; // get_T2_quad_count(new_period);
	else
		count = 0xCAFEBABE; // get_T3_quad_count(new_period);
	return FREQ*count;
}


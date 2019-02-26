#include "CPU2/Misc.h"
#include <Cpu/Std/Platform_Types.h>

#define MOTOR_RPS 1613 ///< 146rpm @12V | Gear ratio 51:1 | 13PPR odometry | 146*51*13/60
#define KP 1.0 ///< Proportional constant
#define KI 0.5 ///< Integral constant
#define KD 0.05 ///< Derivative constant
#define DELTA_TIME 1/FREQ

int get_odometry(SIDE side, boolean new_period);

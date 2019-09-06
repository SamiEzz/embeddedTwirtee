#include <stdlib.h>
#include <stdio.h>



float odo_right;
float odo_left;
float left_wheel_speed;
float right_wheel_speed;
float vitesse;
float theta;
float accel_x;
float gyro_x;
float accel_y;
float gyro_y;
float accel_z;
float gyro_z;
float lidar_angle;
float lidar_dist;
float bat_tension;
float bat_current;
float bat_temperature;
float buck_out_tension;
float buck_in_tension;
float buck_out_current;
char buck_state;
float buck_kp;
float buck_ki;
float buck_kd;



const int can_tetha_droite = 1;
const int can_tetha_gauche = 2;


typedef struct can_table{
    unsigned char data[8];
    int priority;
    int header;
    int payload;
}can_msg;

void *send_via_can(void* can_struct){
    can_msg* can_m=can_struct;

//    can_m->
    
}
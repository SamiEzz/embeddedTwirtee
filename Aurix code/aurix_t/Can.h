#ifndef CAN_H
#define CAN_H

#include <Multican/Can/IfxMultican_Can.h>

typedef enum {
	CAN_NODE_2 = 0,
	CAN_NODE_1
} CAN_NODE;

typedef enum {
	RECEIVE = 0,
	SEND
} SEND_RECEIVE;

typedef enum {
	I_SPEED_SP = 33,
	O_ODOMETRY = 34
} MSG_DATA_ID;

extern void CAN_init(void);
extern IfxMultican_Status CAN_init_nodes(IfxMultican_Status* status1, IfxMultican_Status* status2);
extern void CAN_init_msg(CAN_NODE node1, SEND_RECEIVE to_send, MSG_DATA_ID m_id);
extern void CAN_send_odometry(CAN_NODE node1, unsigned int L_odometry, unsigned int R_odometry);
extern uint32 CAN_receive(CAN_NODE node1, unsigned int *L_speed_sp, unsigned int *R_speed_sp);

#endif ///< CAN_H

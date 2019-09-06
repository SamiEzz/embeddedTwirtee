#include "Can.h"
#include <Multican/Can/IfxMultican_Can.h>
#include <_PinMap/IfxMultican_PinMap.h>

// CAN handle
IfxMultican_Can can;
// Two Nodes (for external loopback demo)
IfxMultican_Can_Node canNode1;
IfxMultican_Can_Node canNode2;
// Two Message objects (for external loopback demo)
IfxMultican_Can_MsgObj canMsgObj1;
IfxMultican_Can_MsgObj canMsgObj2;

void CAN_init(void) {
	// create configuration
	IfxMultican_Can_Config canConfig;
	IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);
	// initialize module
	IfxMultican_Can_initModule(&can, &canConfig);
}

IfxMultican_Status CAN_init_nodes(IfxMultican_Status* status1, IfxMultican_Status* status2) {
	IfxMultican_Status status = IfxMultican_Status_noError;
	// create CAN node config
	IfxMultican_Can_NodeConfig canNodeConfig;
	IfxMultican_Can_Node_initConfig(&canNodeConfig, &can);
	canNodeConfig.baudrate = 1000000; // 1 MBaud

	/// Node 1 ///
	canNodeConfig.nodeId = IfxMultican_NodeId_0;
	canNodeConfig.rxPin = &IfxMultican_RXD0A_P02_1_IN; // ShieldBuddy D3
	canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
	canNodeConfig.txPin = &IfxMultican_TXD0_P02_0_OUT; // ShieldBuddy D2
	canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;
	// initialise the node
	*status1 = IfxMultican_Can_Node_init(&canNode1, &canNodeConfig);

	/// Node 2 ///
	canNodeConfig.nodeId = IfxMultican_NodeId_1;
	canNodeConfig.rxPin = &IfxMultican_RXD1B_P14_1_IN;
	canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
	canNodeConfig.txPin = &IfxMultican_TXD1_P14_0_OUT;
	canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;
	// initialise the node
	*status2 = IfxMultican_Can_Node_init(&canNode2, &canNodeConfig);
	return status;
}

void CAN_init_msg(CAN_NODE node1, SEND_RECEIVE to_send, MSG_DATA_ID m_id) {
		// create message object config
		IfxMultican_Can_MsgObjConfig canMsgObjConfig;
		IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, node1 ? &canNode1 : &canNode2);
		// assigned message object:
		canMsgObjConfig.msgObjId = to_send ? 0 : 1;
		canMsgObjConfig.messageId = m_id;
		canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
		canMsgObjConfig.frame = to_send ? IfxMultican_Frame_transmit : IfxMultican_Frame_receive;
		canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
		canMsgObjConfig.control.extendedFrame = FALSE;
		canMsgObjConfig.control.matchingId = TRUE;
		// initialize message object
		IfxMultican_Can_MsgObj_init(node1 ? &canMsgObj1 : &canMsgObj2, &canMsgObjConfig);
}

void CAN_send_odometry(CAN_NODE node1, unsigned int L_odometry, unsigned int R_odometry) {
	// Initialise the message strcture
	IfxMultican_Message msg;
	IfxMultican_Message_init(&msg, O_ODOMETRY, L_odometry, R_odometry, IfxMultican_DataLengthCode_8);
	// Transmit Data
	//while( IfxMultican_Can_MsgObj_sendMessage(node1 ? &canMsgObj1 : &canMsgObj2, &msg) == IfxMultican_Status_notSentBusy );
	while( IfxMultican_Can_MsgObj_sendMessage(node1 ? &canMsgObj1 : &canMsgObj2, &msg) != IfxMultican_Status_noError );
}

uint32 CAN_receive(CAN_NODE node1, unsigned int *L_speed_sp, unsigned int *R_speed_sp) {
	// Receiving Data
	// Initialise the message structure with dummy values, will be replaced by the received values
	IfxMultican_Message msg;
	IfxMultican_Can_MsgObj *canMsgObjPtr = node1 ? &canMsgObj1 : &canMsgObj2;
	IfxMultican_Message_init(&msg, 0xdead, 0xdeadbeef, 0xdeadbeef, IfxMultican_DataLengthCode_8); // start with invalid values
	// check if Multican receives a new message

	IfxMultican_Status readStatus = 0xFFFF;

	//while(!(IfxMultican_Can_MsgObj_isRxPending(canMsgObjPtr) || IfxMultican_Can_MsgObj_isTxPending(canMsgObjPtr)));

	while (!IfxMultican_Can_MsgObj_isRxPending(canMsgObjPtr)){}
	//if( IfxMultican_Can_MsgObj_isRxPending(canMsgObjPtr) || IfxMultican_Can_MsgObj_isTxPending(canMsgObjPtr) )
	{
		// read message
		readStatus = IfxMultican_Can_MsgObj_readMessage(canMsgObjPtr, &msg);
		if( readStatus == IfxMultican_Status_noError ) {
//			switch(msg.id) {
//				case I_SPEED_SP:
					*L_speed_sp = msg.data[0];
					*R_speed_sp = msg.data[1];
//					break;
		}
		return readStatus;
	}
	return readStatus;
}

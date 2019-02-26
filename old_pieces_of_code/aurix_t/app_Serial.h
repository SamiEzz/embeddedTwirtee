/*
 * app_Serial.h
 *
 *  Created on: 2 oct. 2018
 *      Author: twirteeop
 */

#ifndef APP_SERIAL_H_
#define APP_SERIAL_H_


extern void serial_init_CPU2(void);
extern void sendMsg_CPU2(char *msg);
extern void getMsg_CPU2(char *msg);


#endif /* APP_SERIAL_H_ */

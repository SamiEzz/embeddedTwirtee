/**
******************************************************************************
* @file		io_com_service.h
* @author   [IRT] EZZEROUALI Sami
* @brief	Input_Output communication service 
*           This service provide a high access to all communication medium 
*           To configure this service, please check the io_service_config.json file
******************************************************************************
*/

#ifndef io_com_service
#define io_com_service

#include "../twirtee.h"

#define MAX_VAR_TO_COM 99
#define MAX_JSON_TOKENS 500

// _____________________________________
// json to struct char*
// char* j_speed_left 	= "speed_lef";
// char* j_speed_right	= "speed_right";
// char* j_theta		= "theta";

// char* j_battery		= "batterie"; 
const uint8 j_speed_left		= 0;
const uint8 j_speed_right		= 1;
const uint8 j_tetha				= 2;
const uint8 position_x			= 3;
const uint8 position_y			= 4;
const uint8 position_z			= 5;
const uint8 position_qf			= 6;
const uint8 batterie_state		= 7;
const uint8 odometrie 			= 8;







typedef struct io_data_base {
	uint16 var_id;
	char* data;
	clock_t validity_time;
	clock_t edition_time;
	uint32 periode;
	uint16 size;
	pthread_mutex_t mutex;
	uint8 remote_request;	// 0: no remote, 0!= : oui  
							// et remote_request = taille de la valeur retourné 
	uint8 trigger_type;		// 0: Time, 1: User, 2: Event 
	uint8 validity[2];		// [0]: temporelle, [1]: fonctionnelle
	uint8 medium;			// 0: CAN, 1: UART, 2: Wifi, 3: SPI….
	uint8 disabled;			// 1 : do not exchange variable
} io_data_base;

typedef struct can_tram_db{
	uint16 var_id[4];
	uint8 available;
	char can_id[3];
	char variable[12];
} can_tram_db;

typedef struct can_config{
	uint8 enabled;
	char* can_name;
	uint16 available;
	can_tram_db id_data_base[MAX_VAR_TO_COM];
} can_config;

typedef struct uart_id_db{
	uint16 var_id;
	char variable[12];
} uart_id_db;

typedef struct uart_config{
	uint16 available;
	uint8 enabled;
	uart_id_db uart_id_database[MAX_VAR_TO_COM];
	char COM[12];
	long int speed;
} uart_config;

typedef struct wifi_config{
	uint8 enabled;
	char* SSID;
	char* password;
	char* server;
	uint16 port;
} wifi_config;

typedef struct COM_CONFIG{
	io_data_base data_base[MAX_VAR_TO_COM];
	uint16 available;
	can_config can;
	uart_config uart;
	wifi_config wifi;
} COM_CONFIG;




#endif /* io_com_service */

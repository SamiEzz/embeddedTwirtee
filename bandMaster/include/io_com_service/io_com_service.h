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
#include "../threads_mgr.h"
#include "./var_id.h"
#include "../jsonApi.h"
//#include "./jsmn/jsmn.h"

#define MAX_VAR_TO_COM 99
#define MAX_JSON_TOKENS 500

#define io_vitesse_V 			 12
#define io_omega_W        		 1
#define io_tetha				 2
#define io_position_x			 3
#define io_position_y			 4
#define io_position_z			 5
#define io_position_qf			 6
#define io_batterie_state		 7
#define io_odometrie_left		 8
#define io_odometrie_right		 9
#define io_speed_left			 10
#define io_speed_right			 11

// _____________________________________
// json to struct char*
// char* j_speed_left 	= "speed_lef";
// char* j_speed_right	= "speed_right";
// char* j_theta		= "theta";


typedef struct io_data_base {
	uint16 var_id;
	char data[13];
	uint32 xdata;
	clock_t validity_time;
	clock_t edition_time;
	uint32 periode;
	uint16 size;
	uint8 type; // 0: int, 1: float
	pthread_mutex_t mutex;
	uint8 remote_request;	// 0: no remote, 0!= : oui  
							// et remote_request = taille de la valeur retourné 
	// 0: Time, 1: User, 2: Event 
	uint8 trigger_type;		
	// [0]: temporelle, [1]: fonctionnelle
	uint8 validity[2];		
	// 0: CAN, 1: UART, 2: Wifi, 3: SPI….
	uint8 medium;			
	uint8 disabled;			// 1 : do not exchange variable
} io_data_base;

typedef struct can_tram_db{
	uint16 var_id[10];
	uint8  offsets[10]; 
	clock_t edition_time;
	clock_t period; 
	uint8 available;
	uint16 x_can_id;
	uint32 x_can_frame;
	char* can_id;
	char* variable;
} can_tram_db;

typedef struct can_config{
	uint8 enabled;
	char* can_name;
	uint16 available;
	uint32 speed;
	can_tram_db* id_data_base;
} can_config;

typedef struct uart_id_db{
	uint16 var_id;
	char* variable;
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


typedef struct can_shared{
    char* can_name;
    uint16 id[MAX_VAR_TO_COM];
	uint32 xdata[MAX_VAR_TO_COM];
    unsigned char data[MAX_VAR_TO_COM][13];
    int available;
    pthread_mutex_t mutex;
	COM_CONFIG* p_cfg;
}can_shared;

// typedef struct can_tram{
// 	char msg[13];
// 	//struct tram* next;
// }can_tram;

// typedef struct can_pipe{
// 	can_tram* tram;
// 	int available;
// }can_pipe;
void io_service_main();

void get_bit(uint32 f_in,uint8 offset,uint8* ret);
void set_bit_32(uint32* f_out,uint8 offset,uint8 value);
void set_8bits(uint32* f_out,uint8 offset,uint8 value);
void set_16bits(uint32* f_out,uint8 offset,uint16 value);
void set_bit_16(uint16* f_out,uint8 offset,uint8 value);
void set_bit_8(uint32* f_out,uint8 offset,uint8 value);
void get_uint8(uint32 f_in,uint8 offset,uint8* ret);
void get_uint16(uint32 f_in,uint8 offset,uint16* ret);
void print_bits(uint32 f_in,uint8 size);
void float2char(char* in_char,float f_in);
void uint32tochar(char* out_char,uint32 in_int);
uint32 float2uint32(float f_in);

void io_can_write_engine(COM_CONFIG* cfg,can_shared* pipeline);
void io_can_read_engine(COM_CONFIG* cfg, can_shared* pipeline);
void set_edition_time(clock_t* edition_time);
void io_write(uint8 var_id,uint32 data,COM_CONFIG* cfg);
void io_read(uint8 var_id,uint32* ret,COM_CONFIG* cfg);

int jsoncomp(const char* json, jsmntok_t* tok, const char* s) ;
void com_init_config(uint16* available,char* JSON_STRING,jsmntok_t* t,int max);
void can_init_config(can_config* can,char* JSON_STRING,jsmntok_t* t,int max);
void uart_init_config(uart_config* uart,char* JSON_STRING,jsmntok_t* t,int max);
void io_db_init(COM_CONFIG* cfg,char* JSON_STRING, jsmntok_t* t,int max);
void can_database_init(COM_CONFIG* cfg,char* JSON_STRING, jsmntok_t* t,int max);
sint8 init_io_service(COM_CONFIG* cfg,char* jsonConfigFileName);
void print_conf(COM_CONFIG cfg);
void print_db(COM_CONFIG cfg);



#endif /* io_com_service */

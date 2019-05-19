#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "io_com_service.h"
#include "jsmn/jsmn.h"

int jsoncomp(const char* json, jsmntok_t* tok, const char* s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}
void com_init_config(uint16* available,char* JSON_STRING,jsmntok_t* t,int max){
    //char* tempchar;
    //can->can_name=malloc(sizeof(char)*10);
    int error=1;
    for (int i = 1; i < max; i++) {
        
        if (jsoncomp(JSON_STRING, &t[i], "io_data_base") == 0) {
            *available=t[i+1].size;
            error--;
        } 
    }
    if(error!=0){
        printf("\nio_com_service.c : Configuration io_data_base indisponible dans le fichier json\n");
    }
    
}
void can_init_config(can_config* can,char* JSON_STRING,jsmntok_t* t,int max){
    char* tempchar;
    can->can_name=malloc(4);
    uint8 error=3; // nomrbe d'élements dans l'objet json
    for (int i = 1; i < max; i++) {
        //
        if (jsoncomp(JSON_STRING, &t[i], "can_enabled") == 0) {
            strncpy(tempchar, JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start);
            can->enabled = atoi(tempchar);
            error--;
            //free(&tempchar);
        }
        else if(jsoncomp(JSON_STRING,&t[i],"can_name")==0){
            strncpy(can->can_name,JSON_STRING+t[i+1].start, t[i+1].end-t[i+1].start+2);
            can->can_name[4]='\0';
            error--;
        }   
        else if (jsoncomp(JSON_STRING,&t[i],"can_tram_db")==0)
        {
            /* code */
            can->available=t[i+1].size;
            error--;
        }
         
    }
    if(error!=0){
        printf("\nio_com_service.c : Configuration can indisponible dans le fichier json\n");
    }
    
}

void uart_init_config(uart_config* uart,char* JSON_STRING,jsmntok_t* t,int max){
    char* tempchar;
    uint8 error=4;
    int i=1;
    for (i; i < max; i++) {
        //
        if (jsoncomp(JSON_STRING, &t[i], "uart_enabled") == 0) {
            strncpy(tempchar, JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start);
            uart->enabled = atoi(tempchar);
            error--;
            //free(&tempchar);
        }
        else if(jsoncomp(JSON_STRING,&t[i],"uart_COM")==0){
            strncpy(uart->COM,JSON_STRING+t[i+1].start, t[i+1].end-t[i+1].start+1);
            uart->COM[sizeof(uart->COM)]='\n';
            error--;
        }    
            
        else if(jsoncomp(JSON_STRING,&t[i],"uart_speed")==0){
            strncpy(tempchar,JSON_STRING+t[i+1].start, t[i+1].end-t[i+1].start);
            uart->speed= atol(tempchar);;
            //sprintf(uart->speed,"%ld",tempchar);
            error--;
        } 
        else if (jsoncomp(JSON_STRING,&t[i],"uart_id_db")==0)
        {
            /* code */
            uart->available=t[i+1].size;
            error--;
        }   
    }
    if(error!=0){
        printf("\nio_com_service.c : Configuration uart indisponible dans le fichier json\n");
    }
    
}
void can_database_init(COM_CONFIG* cfg,char* JSON_STRING, jsmntok_t* t,int max){
    
    can_config* pcan=&cfg->can;

    //pcan->id_data_base->variable=malloc(sizeof(char)*20);

    
    uint8 error=2*pcan->available;
    int i=0;
    for (i; i < max; i++) {
        //printf("\n i=%d\n",i);

        char* string="can_id_db";
        if (jsoncomp(JSON_STRING, &t[i], string) == 0) {
            char tempjson[t[i+1].end-t[i+1].start-2];
            strncpy(tempjson, JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start);
            
            int r;
            jsmn_parser p;
            jsmntok_t t[pcan->available*10];
            jsmn_init(&p);
            r = jsmn_parse(&p,tempjson,t[i+1].end-t[i+1].start,t,MAX_JSON_TOKENS);
            if(r<0){
                printf("can_database_init parse json failed r=%d\n",r);
            }
            char* tempchar;
            int index=0;
            for(int k=0;k<r;k++){
                //printf("\nk=%d\n",k);
                if (jsoncomp(tempjson, &t[k], "var_id") == 0){
            
                    // char tempjson_2[t[k+1].end-t[k+1].start-2];
                    // int r_2;
                    // jsmn_parser p_2;
                    // jsmntok_t t_2[pcan->available*10];
                    // jsmn_init(&p_2);
                    // r_2 = jsmn_parse(&p_2,tempjson_2,t_2[i+1].end-t_2[i+1].start,t_2,MAX_JSON_TOKENS);
                    // if(r_2<0){
                    //     printf("can_database_init parse json failed r=%d\n",r_2);
                    // }
                    char* tempchar_2;
                    int index_2=0;

                    for(int j=0;j<pcan->id_data_base[index_2].available;j++){
                        strncpy(tempchar_2, tempjson + t[k+j].start, t[k+j].end-t[k+j].start);
                        pcan->id_data_base[index_2].var_id[j]=atoi(tempchar_2);
                        
                    }
                    // strncpy(tempchar, tempjson + t[k+1].start, t[k+1].end-t[k+1].start);
                    // pcan->id_data_base[index].var_id=atoi(tempchar);
                    // error--;
                }
                if (jsoncomp(tempjson, &t[k], "can_id") == 0){
                    strncpy(pcan->id_data_base[index].can_id, tempjson + t[k+1].start, t[k+1].end-t[k+1].start);
                    //printf("\ncan_id=%s\n",pcan->id_data_base[index].can_id);
                    index++;
                    error--;
                }
                

            }
            //free(&tempchar);
        }
    }
    if(error!=0){
        printf("\n%d : io_com_service.c : Configuration can_id_db indisponible dans le fichier json\n",error);
    }
    
}
void init_io_service(COM_CONFIG* cfg,char* jsonConfigFileName){ 
	/***
	 * 
	 * 
	 * int_config[0]: CAN enabled ? 0 or 1
	 * int_config[1]: UART enabled ? 0 or 1
	 * int_config[2]: WIFI enabled ? 0 or 1
	 * 
	 * 
	 * */
    long length=0;
    char* JSON_STRING;

    FILE* f = fopen(jsonConfigFileName, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        //printf("length : %ld\n",length);
        fseek(f, 0, SEEK_SET);
        JSON_STRING = malloc(length+1);
        if (JSON_STRING) {
            fread(JSON_STRING, 1, length, f);
        }
        fclose(f);
        //printf("io.c : open json file : %s\n",JSON_STRING);

    } else {
        printf("io.c : Couldn't open json file. \n");
    }
    int r;
    //char* s = "{'sami':3,'rick':2,'morty':1}";
    jsmn_parser p;
    jsmntok_t t[MAX_JSON_TOKENS];
    jsmn_init(&p);
    r = jsmn_parse(&p,JSON_STRING,strlen(JSON_STRING),t,MAX_JSON_TOKENS);
    if(r<0){
        if(r==JSMN_ERROR_INVAL){
            printf("%d : bad token, JSON string is corrupted\n",r);
        }
        else if(r==JSMN_ERROR_NOMEM){
            printf("%d : not enough tokens, JSON string is too large\n",r);
        }
        else if(r==JSMN_ERROR_PART){
            printf("%d : JSON string is too short, expecting more JSON data\n",r);
        }        
    }
    else{
        printf("Json file parsed : %d tokens\n",r);
    }
	
    can_init_config(&cfg->can,JSON_STRING,t,r);
    uart_init_config(&cfg->uart,JSON_STRING,t,r);
    com_init_config(&cfg->available,JSON_STRING,t,r);
    
    can_database_init(cfg,JSON_STRING,t,r);
}

int main(){
    COM_CONFIG cfg;
    //char* jsonConfigFileName="io_service_config.json";
    char* jsonConfigFileName="/home/lasagne/Documents/git/embeddedTwirtee/bandMaster/include/io_com_service/io_service_config.json";
    init_io_service(&cfg,jsonConfigFileName);


    printf("\ncan_config.enabled : %d\n",cfg.can.enabled);
    printf("can name : %s\n",cfg.can.can_name);
    printf("can variables : %d\n",cfg.can.available);
    for(int i=0;i<cfg.can.available;i++){
        printf("can ids : %s\n",cfg.can.id_data_base[i].can_id);
    }
    
    printf("\nuart enabled : %d\n",cfg.uart.enabled);
    printf("uart COM : %s\n",cfg.uart.COM);
    printf("uart speed : %ld\n",cfg.uart.speed);
    printf("uart available : %d\n",cfg.uart.available);
    
    printf("\nCOM available variables : %d\n",cfg.available);
     
}   
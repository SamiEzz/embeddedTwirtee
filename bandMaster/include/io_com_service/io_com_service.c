#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

//#include "../twirtee.h"
#include "io_com_service.h"
#include "./linux-can-utils/write_can.h"
#include "./linux-can-utils/read_can.h"

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
    char tempchar[12];//="500000";
    can->can_name=malloc(4);
    uint8 error=4; // nomrbe d'élements dans l'objet json
    for (int i = 1; i < max; i++) {
        //
        if (jsoncomp(JSON_STRING, &t[i], "can_enabled") == 0) {
            strncpy(tempchar, JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start+1);
            can->enabled = atoi(tempchar);
            //printf("can_enabled : %d\n",can->enabled);
            error--;
            //free(&tempchar);
        }
        else if(jsoncomp(JSON_STRING,&t[i],"can_name")==0){
            strncpy(can->can_name,JSON_STRING+t[i+1].start, t[i+1].end-t[i+1].start+1);
            can->can_name[4]='\0';
            error--;
        }   

        else if(jsoncomp(JSON_STRING,&t[i],"can_speed")==0){
            strncpy(tempchar, JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start+1);
            can->speed = atoi(tempchar);
            error--;
        }   
        else if (jsoncomp(JSON_STRING,&t[i],"can_tram_id")==0)
        {
            /* code */
            can->available=t[i+1].size;
            error--;
        }
         
    }
    if(error!=0){
        printf("\n%d :io_com_service.c : Configuration can indisponible dans le fichier json\n",error);
    }
    
}

void uart_init_config(uart_config* uart,char* JSON_STRING,jsmntok_t* t,int max){
    char tempchar[]="500000";
    uint8 error=4;
    int i=1;
    for (i; i < max; i++) {
        //
        if (jsoncomp(JSON_STRING, &t[i], "uart_enabled") == 0) {
            char c[2];
            strncpy(c, JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start+1);
            uart->enabled = atoi(c);
            error--;
            //free(&tempchar);
        }
        else if(jsoncomp(JSON_STRING,&t[i],"uart_COM")==0){
            strncpy(uart->COM,JSON_STRING+t[i+1].start, t[i+1].end-t[i+1].start+1);
            uart->COM[sizeof(uart->COM)]='\0';
            error--;
        }    
            
        else if(jsoncomp(JSON_STRING,&t[i],"uart_speed")==0){
            strncpy(tempchar,JSON_STRING+t[i+1].start, t[i+1].end-t[i+1].start+1);
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
void io_db_init(COM_CONFIG* cfg,char* JSON_STRING, jsmntok_t* t,int max){
    // looking for json extrems
    uint16 start_tok=0,end_tok=0;
    for(int i=0;i<max;i++){
        if (jsoncomp(JSON_STRING, &t[i], "io_data_base") == 0){
            start_tok=i;
        }
        else if(jsoncomp(JSON_STRING, &t[i], "last_variable") == 0){
            end_tok=i+2;
        }
    }

    char tempchar[]="10";
    uint8 index=0;
    for(int k=start_tok;k<end_tok;k++){
        if (jsoncomp(JSON_STRING, &t[k], "var_id") == 0){
            strncpy(tempchar,JSON_STRING+t[k+1].start,t[k+1].end-t[k+1].start+1);
            cfg->data_base[index].var_id=atoi(tempchar);
            //printf("varid[%d] : %d\n",index,cfg->data_base[index].var_id);
            //index++;
        }
        else if (jsoncomp(JSON_STRING, &t[k], "validity_time") == 0){
            strncpy(tempchar,JSON_STRING+t[k+1].start,t[k+1].end-t[k+1].start);
            cfg->data_base[index].validity_time=atoi(tempchar);
            //printf("varid[%d] : %d\n",index,cfg->data_base[index].var_id);
            //index++;
        }
        else if (jsoncomp(JSON_STRING, &t[k], "period") == 0){
            strncpy(tempchar,JSON_STRING+t[k+1].start,t[k+1].end-t[k+1].start+1);
            cfg->data_base[index].periode=atoi(tempchar);
            //printf("varid[%d] : %d\n",index,cfg->data_base[index].var_id);
            //index++;
        }
        else if (jsoncomp(JSON_STRING, &t[k], "size") == 0){
            strncpy(tempchar,JSON_STRING+t[k+1].start,t[k+1].end-t[k+1].start+1);
            cfg->data_base[index].size=atoi(tempchar);
            //printf("varid[%d] : %d\n",index,cfg->data_base[index].var_id);
            //index++;
        }
        else if (jsoncomp(JSON_STRING, &t[k], "trigger_type") == 0){
            strncpy(tempchar,JSON_STRING+t[k+1].start,t[k+1].end-t[k+1].start+1);
            cfg->data_base[index].trigger_type=atoi(tempchar);
            //printf("varid[%d] : %d\n",index,cfg->data_base[index].var_id);
            //index++;
        }
        else if (jsoncomp(JSON_STRING, &t[k], "medium") == 0){
            strncpy(tempchar,JSON_STRING+t[k+1].start,t[k+1].end-t[k+1].start+1);
            cfg->data_base[index].medium=atoi(tempchar);
            //printf("varid[%d] : %d\n",index,cfg->data_base[index].var_id);
            //index++;
        }
        else if (jsoncomp(JSON_STRING, &t[k], "disabled") == 0){
            strncpy(tempchar,JSON_STRING+t[k+1].start,t[k+1].end-t[k+1].start);
            cfg->data_base[index].disabled=atoi(tempchar);
            //printf("varid[%d] : %d\n",index,cfg->data_base[index].var_id);
            index++;
        }

    }
    //free(tempchar);
}
void can_database_init(COM_CONFIG* cfg,char* JSON_STRING, jsmntok_t* t,int max){
    
    can_config* pcan=&cfg->can;
    
    //pcan->id_data_base->variable=malloc(sizeof(char)*20);

    uint8 error=0;
    int i=0;
    int index=0;
    for (i=0; i < max; i++) {
        //printf("\n i=%d\n",i);
        if (jsoncomp(JSON_STRING, &t[i], "can_tram_id") == 0) {
            pcan->available=t[i+1].size;
            int index_2=0;
            int index_3=0;
            
            char tempchar[12];
            int k=0;
            int K=pcan->available*30;
            //int* p_var_id=pcan->id_data_base[index_2];
            for(k=i;k<K;k++){
                char* tempchar_2=malloc(32);
                if (jsoncomp(JSON_STRING, &t[k], "var_id") == 0){
                    pcan->id_data_base[index_2].available=t[k+1].size;
                    for(int j=0;j<t[k+1].size;j++){
                        //printf("\nk : %d, j : %d\n",k,j);
                        strncpy(tempchar_2, JSON_STRING + t[k+j+2].start, 4);
                        pcan->id_data_base[index_2].var_id[j]=atoi(tempchar_2);
                        //tempchar_2[sizeof(tempchar)]='\0';
                        //printf("id[%d] : %d \n",j,pcan->id_data_base[index_2].var_id[j]);
                    }
                    index_2++;
                    // error--;
                }
                else if (jsoncomp(JSON_STRING, &t[k], "offsets") == 0){
                    pcan->id_data_base[index_3].available=t[k+1].size;
                    for(int j=0;j<t[k+1].size;j++){
                        strncpy(tempchar_2, JSON_STRING + t[k+j+2].start, 4);
                        pcan->id_data_base[index_3].offsets[j]=atoi(tempchar_2);
                        // printf("index_3[%d/%d] : %d \n",j,t[k+1].size,pcan->id_data_base[index_3].offsets[j]);
                    }
                    index_3++;
                }
                else if (jsoncomp(JSON_STRING, &t[k], "tram_period") == 0){
                    strncpy(tempchar, JSON_STRING + t[k+1].start, t[k+1].end-t[k+1].start);
                    pcan->id_data_base[index].period=atoi(tempchar);
                    //printf("\nperiod :%d\n",pcan->id_data_base[index].period);
                    //error--;
                }
                else if (jsoncomp(JSON_STRING, &t[k], "can_id") == 0){
                    strncpy(pcan->id_data_base[index].can_id, JSON_STRING + t[k+1].start, t[k+1].end-t[k+1].start);
                    //printf("\n%d : can_id=%s\n",index,pcan->id_data_base[index].can_id);
                    index++;
                    //error--;
                }
            }
            //free(&tempchar);
        }
    }
    if(error!=0){
        printf("\n%d : io_com_service.c : Configuration can_tram_id indisponible dans le fichier json\n",error);
    }
    
}
sint8 init_io_service(COM_CONFIG* cfg,char* jsonConfigFileName){ 
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
        return -1;
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
    //printf("io_com_service : com_init_config()\n");
    com_init_config(&cfg->available,JSON_STRING,t,r);
    //printf("io_com_service : can_init_config()\n");
    can_init_config(&cfg->can,JSON_STRING,t,r);
    //printf("io_com_service : uart_init_config()\n");
    uart_init_config(&cfg->uart,JSON_STRING,t,r);

    //printf("io_com_service : io_db_init()\n");
    io_db_init(cfg,JSON_STRING,t,r);
    //printf("io_com_service : can_database_init()\n");    
    can_database_init(cfg,JSON_STRING,t,r);


    return 0;
}

void print_conf(COM_CONFIG cfg){
    printf("\nuart enabled : %d\n",cfg.uart.enabled);
    printf("uart COM : %s\n",cfg.uart.COM);
    printf("uart speed : %ld\n",cfg.uart.speed);
    printf("uart available : %d\n",cfg.uart.available);
    printf("\ncan_config.enabled : %d\n",cfg.can.enabled);
    printf("can_config.name : %s\n",cfg.can.can_name);
    printf("can_config.speed : %ld\n",cfg.can.speed);
}
void print_db(COM_CONFIG cfg){
    printf("\ncan variables : %d\n",cfg.can.available);
    printf("can id");
    for(int i=0;i<cfg.can.available;i++){
        printf("\n%s \t\t",cfg.can.id_data_base[i].can_id/*,cfg.can.id_data_base[i].period*/);
        for(int j=0;j<cfg.can.id_data_base[i].available;j++){
            printf("%d\t",cfg.can.id_data_base[i].var_id[j]);
        }
        printf("\n----Offsets");
        for(int l=0;l<cfg.can.id_data_base[i].available;l++){
            printf("\t%d ",cfg.can.id_data_base[i].offsets[l]);
        }            
    }
    printf("\n\nCOM available variables : %d\n",cfg.available);
    for(int j=0;j<cfg.available;j++){
        printf("varid[%d] \t\t-> %d",j,cfg.data_base[j].var_id);
        printf("\n\tval_time \t: %ld",cfg.data_base[j].validity_time);
        printf("\n\tperiod \t\t: %ld",cfg.data_base[j].periode);
        printf("\n\tsize \t\t: %d bits",cfg.data_base[j].size);
        printf("\n\ttrigger_type \t: %s",(cfg.data_base[j].trigger_type==0)?"Time":"Event");
        printf("\n\tmedium \t\t: %s\n",(cfg.data_base[j].medium==0)?"CAN":"UART");
    }
}
sint16 get_element_byvarid(uint8 var_id,COM_CONFIG* cfg){
    for(sint16 i=0;i<cfg->available;i++){
        pthread_mutex_lock(&cfg->data_base[i].mutex);
        if(cfg->data_base[i].var_id==var_id){
            return i;
        }
        pthread_mutex_unlock(&cfg->data_base[i].mutex);
    }
    return -1;
}

uint8 check_time_val(clock_t period,clock_t edition_time){
    if(period+edition_time<clock()){
        return 1;
    }
    return 0;
}
void io_can_concatenate(can_tram_db* c_tram){

}
/**
 * @brief 
 *  
 * @param cfg 
 */
void io_can_write_engine(COM_CONFIG* cfg){
    can_shared* can_buffer=malloc(sizeof(can_shared));
    //sprintf(can_buffer->data[can_buffer->available],"%s",cfg);

    // cfg.can.available
    // cfg.can.id_data_base[1].available
    // cfg.can.id_data_base[1].edition_time

    for(int i=0;i<cfg->can.available;i++){
        if(check_time_val(cfg->can.id_data_base[i].period,cfg->can.id_data_base[i].edition_time)==1){
            io_can_concatenate(&cfg->can.id_data_base[i]);
        }
    }    
    uint8 sendable_var;


}


void set_edition_time(clock_t* edition_time){
    *edition_time=clock();
}
void check_availability(){

}
void float2char(char* in_char,float f_in){
    unsigned int payload;
    memcpy(&payload,&f_in,4);
    sprintf(in_char,"%08x",payload);
    //printf("CAN SEND : %s \t %f\n",test,f_in);
}

void io_can_write(uint8 var_id,char* data,COM_CONFIG* cfg){
    sint16 index=get_element_byvarid(var_id,cfg);
    if(index==-1){
        printf("\nio_com_service.c : var_id introuvable\n");
    }
    pthread_mutex_lock(&cfg->data_base[index].mutex);
    set_edition_time(&cfg->data_base[index].edition_time);
    sprintf(cfg->data_base[index].data,"%s",data);
    pthread_mutex_unlock(&cfg->data_base[index].mutex);

    //char* value[4];
    
}

// typedef struct can_shared{
//     char* can_name;
//     unsigned int id[100];
//     unsigned char data[100][256];
//     int available;
//     pthread_mutex_t mutex;
// }can_shared;
int main(){
    COM_CONFIG cfg;
    //char* jsonConfigFileName="./io_service_config.json";
    char jsonConfigFileName[]="/home/samie/Documents/git/embeddedTwirtee/bandMaster/include/io_com_service/io_service_config.json";
    printf("io service initiation \njsonConfigFileName : %s\n",jsonConfigFileName);
    if(init_io_service(&cfg,jsonConfigFileName)==0){        
        //print_db(cfg);
        //print_conf(cfg);
    };
    //char value;
    char value[16];
    float2char(value,0.1);
    printf("CAN SEND : %s \t %f\n",value,0.1);
    float2char(value,0.2);
    printf("CAN SEND : %s \t %f\n",value,0.2);
    float2char(value,3.14);
    printf("CAN SEND : %s \t %f\n",value,3.14);
    
    
 }   

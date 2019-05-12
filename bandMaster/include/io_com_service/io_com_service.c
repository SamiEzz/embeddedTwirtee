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
    char tempchar[12];
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
    char tempchar[12];
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
            uart->COM[sizeof(uart->COM)]='\0';
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

    uint8 error=0;
    int i=0;
    int index=0;
    for (i=0; i < max; i++) {
        //printf("\n i=%d\n",i);
        if (jsoncomp(JSON_STRING, &t[i], "can_tram_id") == 0) {
            pcan->available=t[i+1].size;
            //error=pcan->available;
            // int json_size = t[i+1].end-t[i+1].start;
            // char tempjson[json_size];
            // strncpy(tempjson, JSON_STRING + t[i+1].start, json_size);
            // tempjson[json_size]='\0';
            // printf("%s",tempjson);
            // int r;
            // jsmn_parser p;
            // jsmntok_t t[pcan->available*15];
            // jsmn_init(&p);
            // r = jsmn_parse(&p,tempjson,t[i+1].end-t[i+1].start,t,MAX_JSON_TOKENS);
            // if(r<0){
            //     printf("can_database_init parse json failed r=%d\n",r);
            // }
            char* tempchar;
            int k=0;
            int K=100;
            for(k=i;k<K;k++){
                if (jsoncomp(JSON_STRING, &t[k], "var_id") == 0){
                    char* tempchar_2=malloc(32);
                    int index_2=0;
                    pcan->id_data_base[index_2].available=t[k+1].size;

                    for(int j=0;j<t[k+1].size;j++){
                        //printf("\nk : %d, j : %d\n",k,j);
                        strncpy(tempchar_2, JSON_STRING + t[k+j+2].start, 4);
                        pcan->id_data_base[index_2].var_id[j]=atoi(tempchar_2);
                        //tempchar_2[sizeof(tempchar)]='\0';
                        printf("id[%d] : %d \n",j,pcan->id_data_base[index_2].var_id[j]);
                    
                        // strncpy(tempchar_2, tempjson + t[k+1+j].start, t[k+1+j].end-t[k+1+j].start);
                        // pcan->id_data_base[index_2].var_id[j]=atoi(tempchar_2);
                        //printf("\n%d id[%d]",pcan->id_data_base[index_2].var_id[j],index_2);
                    }
                    printf(",\n");
                    index_2++;
                    // strncpy(tempchar, tempjson + t[k+1].start, t[k+1].end-t[k+1].start);
                    // pcan->id_data_base[index].var_id=atoi(tempchar);
                    // error--;
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
    printf("io_com_service : com_init_config()\n");
    com_init_config(&cfg->available,JSON_STRING,t,r);
    printf("io_com_service : can_init_config()\n");
    can_init_config(&cfg->can,JSON_STRING,t,r);
    printf("io_com_service : uart_init_config()\n");
    uart_init_config(&cfg->uart,JSON_STRING,t,r);
    
    
    can_database_init(cfg,JSON_STRING,t,r);
    return 0;
}

int main(){
    COM_CONFIG cfg;
    char* jsonConfigFileName="./io_service_config.json";
    //char jsonConfigFileName[]="/home/samie/Documents/git/embeddedTwirtee/bandMaster/include/io_com_service/io_service_config.json";
    printf("io service initiation \njsonConfigFileName : %s\n",jsonConfigFileName);
    if(init_io_service(&cfg,jsonConfigFileName)==0){        
        printf("\ncan_config.enabled : %d\n",cfg.can.enabled);
        
        printf("can variables : %d\n",cfg.can.available);
        printf("can ids : {");
        for(int i=0;i<cfg.can.available;i++){
            printf("%s,\t",cfg.can.id_data_base[i].can_id);
        }
        printf("}\n");
        
        printf("\nuart enabled : %d\n",cfg.uart.enabled);
        printf("uart COM : %s\n",cfg.uart.COM);
        printf("uart speed : %ld\n",cfg.uart.speed);
        printf("uart available : %d\n",cfg.uart.available);
        
        printf("\nCOM available variables : %d\n",cfg.available);
    };


     
}   

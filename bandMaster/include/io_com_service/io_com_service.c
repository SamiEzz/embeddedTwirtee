#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "io_com_service.h"
#include "jsmn/jsmn.h"

#define MAX_JSON_TOKENS 200

int jsoncomp(const char* json, jsmntok_t* tok, const char* s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}
void can_init_config(can_config* can,char* JSON_STRING,jsmntok_t* t,int max){
    printf("%d : max \n",max);
    char* tempchar;
    for (int i = 1; i < max; i++) {
        //
        if (jsoncomp(JSON_STRING, &t[i], "can_enabled") == 0) {
            strncpy(tempchar, JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start);
            can->enabled = atoi(tempchar);
            //free(&tempchar);
        }
        else if(jsoncomp(JSON_STRING,&t[i],"can_name")==0){
            strncpy(tempchar,JSON_STRING+t[i+1].start, t[i+1].end-t[i+1].start);
            can->can_name=tempchar;
            sprintf(can->can_name,"%c%c%c%c",tempchar[0],tempchar[1],tempchar[2],tempchar[3]);
            //printf("can_name : %s\n",can->can_name);
        }

        
    }
}
void init_io_service(COM_CONFIG* cfg,char* jsonConfigFileName){ // 
	/***
	 * 
	 * 
	 * int_config[0]: CAN enabled ? 0 or 1
	 * int_config[1]: UART enabled ? 0 or 1
	 * int_config[2]: WIFI enabled ? 0 or 1
	 * 
	 * 
	 * */
    char* JSON_STRING = NULL;
    long length;

    FILE* f = fopen(jsonConfigFileName, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        printf("length : %ld\n",length);
        fseek(f, 0, SEEK_SET);
        JSON_STRING = malloc(length*sizeof(char));
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
        printf("%d : Json file parsed\n",r);
    }
	
    can_init_config(&cfg->can,JSON_STRING,t,MAX_JSON_TOKENS);
}

int main(){
    COM_CONFIG cfg;
    char* jsonConfigFileName="io_service_config.json";

    init_io_service(&cfg,jsonConfigFileName);
    printf("can name : %s\n",cfg.can.can_name);
    printf("can_config.enabled : %d\n",cfg.can.enabled);

}
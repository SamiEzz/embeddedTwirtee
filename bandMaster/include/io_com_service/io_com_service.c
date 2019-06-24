#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
//#include "../twirtee.h"
//#include "./jsmn/jsmn.h"
#include "../jsonApi.h"
#include "io_com_service.h"
#include "./linux-can-utils/write_can.h"
#include "./linux-can-utils/read_can.h"
#define SYSTEM_CMD_STOP 1

void io_service_thread(){
    COM_CONFIG vcfg;
    COM_CONFIG* cfg=&vcfg;
    //char* jsonConfigFileName="./io_service_config.json";
    //char jsonConfigFileName[]="/mnt/d/CODE/git/embeddedTwirtee/bandMaster/include/io_com_service/io_service_config.json";
    char jsonConfigFileName[]="/home/pi/Documents/git/embeddedTwirtee/bandMaster/include/io_com_service/io_service_config.json";
    
    printf("io service initiation \njsonConfigFileName : %s\n",jsonConfigFileName);
    if(init_io_service(cfg,jsonConfigFileName)==0){        
        //print_db(*cfg);
        print_conf(*cfg);
    };
    can_shared can_pipeline;
    can_pipeline.available=3;
    can_pipeline.id[0]=100;
    can_pipeline.id[1]=101;
    can_pipeline.id[2]=200;
    
    sprintf(can_pipeline.data[0],"F0000AA");
    sprintf(can_pipeline.data[1],"F000001");
    sprintf(can_pipeline.data[2],"06061995");
    uint32 balek;
    while(1){
        delay(1000);
        io_can_read_engine(cfg,&can_pipeline);
        io_read(0,&balek,cfg);
        io_read(1,&balek,cfg);
        io_read(8,&balek,cfg);
        io_read(9,&balek,cfg);
        can_pipeline.available=0;
        
    }

    /* CAN TEST WORKING
    for(int y=0;y<5;y++){
        io_simulation(cfg,y);
        can_shared pipeline_can;
        pipeline_can.available=0;
        pthread_mutex_init(&(pipeline_can.mutex),NULL);
        io_can_write_engine(cfg,&pipeline_can);
        delay(1000);
    }
    */
 }
/**
 * @brief Set the bit number "offset" to the value "value"
 * 
 * @param f_out 
 * @param offset 
 * @param value 
 */
#define BitVal(data,y) ( (data>>y) & 1)      /** Return Data.Y value   **/
#define SetBit(data,y)    data |= (1 << y)    /** Set Data.Y   to 1    **/
#define ClearBit(data,y)  data &= ~(1 << y)   /** Clear Data.Y to 0    **/


void get_bit(uint32 f_in,uint8 offset,uint8* ret){
    //*ret = f_in>>offset&1;
    *ret=BitVal(f_in,offset);
}

void set_bit_32(uint32* f_out,uint8 offset,uint8 value){
    
    // uint8 state;
    // *f_out=0;
    // get_bit(*f_out,offset,&state);
    if(BitVal(value,offset)==0 && value==1){
        *f_out=SetBit(value,offset);// 2^offset;
    }
    else if(BitVal(value,offset)==1 && value==0){
        *f_out=ClearBit(value,offset);
    }
}

void set_8bits(uint32* f_out,uint8 offset,uint8 value){
    *f_out=0;
    uint8 SIZE=8;
    uint8 bin_ret=0;
    if(offset<=24){
        for(int i=0;i<SIZE;i++){
            get_bit(value,i,&bin_ret);
            set_bit_32(f_out,offset+i,bin_ret);
        }
    }
    else{
        printf("\nio_com_service : set_8bits - offset:%d > 24\n",offset);
    }
}

void set_16bits(uint32* f_out,uint8 offset,uint16 value){
    uint8 SIZE=16;
    uint8 bin_ret=0;
    if(offset<=16){
        for(int i=0;i<SIZE;i++){
            get_bit(value,i,&bin_ret);
            set_bit_32(f_out,offset+i,bin_ret);
        }
    }
    else{
        printf("\nio_com_service : set_16bits - offset:%d > 16\n",offset);
    }
}

void set_bit_16(uint16* f_out,uint8 offset,uint8 value){
    uint8 state;
    get_bit(*f_out,offset,&state);
    if(state==0 && value==1){
        *f_out+=(uint32)pow(2,offset);// 2^offset;
    }
    else if(state==1 && value==0){
        *f_out-=(uint32)pow(2,offset);
    }
}
void set_bit_8(uint32* f_out,uint8 offset,uint8 value){
    uint8 state;
    get_bit(*f_out,offset,&state);
    if(state==0 && value==1){
        *f_out+=(uint32)pow(2,offset);// 2^offset;
    }
    else if(state==1 && value==0){
        *f_out-=(uint32)pow(2,offset);
    }
}
void get_uint8(uint32 f_in,uint8 offset,uint8* ret){
    //*ret = f_in>>offset&1;

    uint8 SIZE=8;
    uint8 bit=0;
    for(int i=0;i<SIZE;i++){
        get_bit(f_in,offset+i,&bit);
        set_bit_8(ret,i,bit);
    }
}

void get_uint16(uint32 f_in,uint8 offset,uint16* ret){
    //*ret = f_in>>offset&1;
    uint8 SIZE=16;
    uint8 bit;
    for(int i=0;i<SIZE;i++){
        get_bit(f_in,offset+i,&bit);
        set_bit_16(ret,i,bit);
    }
}

void print_bits(uint32 f_in,uint8 size){
    for(int _i=0;_i<size;_i++){
        uint8 ret;
        get_bit(f_in,_i,&ret);
        printf("%d,",ret);
    }
    printf("\n");
}

   




sint16 get_element_byvarid(uint16 var_id,COM_CONFIG* cfg){
    for(sint16 i=0;i<cfg->available;i++){
        //pthread_mutex_lock(&cfg->data_base[i].mutex);
        if(cfg->data_base[i].var_id==var_id){
            return i;
        }
        //pthread_mutex_unlock(&cfg->data_base[i].mutex);
    }
    return -1;
}

uint8 check_time_val(can_tram_db* db_element){
    if(db_element->period+db_element->edition_time<clock()){
        return 1;
    }
    return 0;
}
void io_can_concatenate(can_shared* c_tram,COM_CONFIG* cfg){
    // uint8 indexs[c_tram->available];
    // c_tram->variable="01234567890";
    // for(int i=0;i<c_tram->available;i++){
    //     indexs[i]=get_element_byvarid(c_tram->var_id[i],cfg);
    // }

}
/**
 * @brief 
 *  update can database
 * @param cfg 
 */


/**
 * @brief 
 * 
 * @param cfg 
 * @param pipeline 
 */

void io_can_write_engine(COM_CONFIG* cfg,can_shared* pipeline){

//    pipeline->available=0;
    for(int i=0;i<cfg->can.available;i++){
        //printf("\n can available : %d\n",cfg->can.available);
        uint8 size=(uint8)cfg->can.id_data_base[i].available;
        //uint8 size=2;
        
        uint16 indexs[size];
        //printf("i : %d\n",i);
        uint32 xcan_frame=0x0;
        for(int j=0;j<size;j++){
            //printf("j : %d\n",j);
            //printf("io_com_service.c : inside io_can_write_engine loop 2\n");
            io_data_base* p_base=cfg->data_base;
            
            indexs[j]=get_element_byvarid(*(&cfg->can.id_data_base[i].var_id[j]),cfg);
            if(cfg->data_base[indexs[j]].size==32&& cfg->data_base[indexs[j]].type==0){ // long int
                //printf("io_com_service.c : size 32 int\n");
                xcan_frame=(p_base+indexs[j]*sizeof(io_data_base))->xdata;
                set_edition_time(indexs[j],cfg);
            }
            else if(cfg->data_base[indexs[j]].size==32 && cfg->data_base[indexs[j]].type==1){ // float type
                //xcan_frame=(p_base+indexs[j])->xdata;
                memcpy(&xcan_frame,&((p_base+indexs[j])->xdata),4);
                set_edition_time(indexs[j],cfg);
                //printf("io_can_write_engine, xdata : %x \n",(p_base+indexs[j])->xdata);
            }
            else if(cfg->data_base[indexs[j]].size==16){
                
                //printf("io_com_service.c : size 16 int\n");
                uint16 payload=0x0;
                payload=(uint16)cfg->data_base[indexs[j]].xdata;
                set_16bits(&xcan_frame,cfg->can.id_data_base[i].offsets[j],payload);
                set_edition_time(indexs[j],cfg);
                //printf("xcan_frame : %x\n",xcan_frame);
                //sprint()
            }
            else if(cfg->data_base[indexs[j]].size==8){
                
                //printf("io_com_service.c : size 8 int\n");
                uint16 payload=0x0;
                payload=(uint8)cfg->data_base[indexs[j]].xdata;
                set_8bits(&xcan_frame,cfg->can.id_data_base[i].offsets[j],payload);
                set_edition_time(indexs[j],cfg);
                //printf("data : %x - xcan_frame : %x\n",cfg->data_base[indexs[j]].xdata,xcan_frame);
                //sprint()
            }
        }
        

        sprintf(pipeline->data[pipeline->available],"%s#%08x",cfg->can.id_data_base[i].can_id,xcan_frame);
        printf("add to pipeline: %s\n",pipeline->data[pipeline->available]);
        pipeline->available++;
        
        //io_can_pipeline_append(pipeline,add_to_pipe);
        //io_can_pipeline_pop(pipeline,&poped);
        //printf("added to pipeline: %s\n",poped.msg);
        //printf("\ntram.msg : %s \n",pipeline.data);
    }    
    write_can(pipeline);
}

int get_tram_by_canid(char* canid,COM_CONFIG* cfg){
    //int ret_id[10];
    int index=0;
    can_tram_db* db=cfg->can.id_data_base;
    for(int i=0;i<cfg->can.available;i++){
        char cur_id[4];
        strcpy(cur_id,(db+i*sizeof(can_tram_db))->can_id);
        if(strcmp(cur_id,canid)==0){
            return i;
        }
    }
}
/**
 * @brief 
 *  Cette fonction va lire la liste des trams reçu sur le CAN,
 *  + Repérer les variables de la tram par l'id de la tram
 *  + affecter la variable dans la base de données de COM grace a l'id de la variable
 *  + inscrire le temps de modification de la variable
 * 
 * @param cfg 
 * @param pipeline 
 */


void read_from_cantram(uint8 offset,uint8 SIZE,uint32 can_xdata,uint32* result){
    uint8 bit=0;
    *result=0;
    int i=offset;
    for(i;i<offset+SIZE;i++){
        //get_bit(can_xdata,i,&bit);
        //set_bit_8(result,i-offset,bit);
        bit = BitVal(can_xdata,i);
        if(bit==1){
            SetBit(*result,(i-offset));
        }
        else if(bit==0){
            ClearBit(*result,(i-offset));
        }
        
    }
}

void io_can_read_engine(COM_CONFIG* cfg,can_shared* pipeline){
    pthread_t t_io_read_can;
    start_thread(&t_io_read_can, NULL, read_can, pipeline);
    //read_can(pipeline);
    

    uint8 tram_ids[cfg->can.available];
    int var_id;
    int var_offsets[10];
    uint32 var_values[10];
    uint8 index=0;
    int size=0;
    for(int i=0;i<pipeline->available;i++){
        index=0;
        for(int j=0;j<cfg->can.available;j++){
            char hex_id[4];
            sprintf(hex_id,"%x",pipeline->id[i]);
            hex_id[3]='\0';
            uint32 x_hex_id=strtol(pipeline->id[i],NULL,16);
            uint32 new_x_hex_id=strtol(cfg->can.id_data_base[j].can_id,NULL,16);
            
            if(x_hex_id==new_x_hex_id){
                tram_ids[index]=j;
                index++;
            }
            else{
                printf("io_com_service.c : /!'\ id not found : %s \n",hex_id);
            }
        }
    }
    for(int k=0;k<pipeline->available;k++){
        if(cfg->can.id_data_base[tram_ids[k]].available==1){
            var_id=get_element_byvarid(cfg->can.id_data_base[tram_ids[k]].var_id[0],cfg);
            uint32 xcan_data=0;
            xcan_data=strtol(pipeline->data[k],NULL,16);
            
            read_from_cantram(cfg->can.id_data_base[tram_ids[k]].offsets[0],cfg->data_base[var_id].size,xcan_data,&(cfg->data_base[var_id].xdata));
            uint32 balek[1];
            io_read(var_id,balek,cfg);
        }
        else if(cfg->can.id_data_base[tram_ids[k]].available>1){
            for(int l=0;l<cfg->can.id_data_base[tram_ids[k]].available;l++){
                var_id=get_element_byvarid(cfg->can.id_data_base[tram_ids[k]].var_id[l],cfg);
                
                uint32 xcan_data=0;
                xcan_data=strtol(pipeline->data[k],NULL,16);
                // read data
                read_from_cantram(cfg->can.id_data_base[tram_ids[k]].offsets[l],cfg->data_base[var_id].size,xcan_data,&(cfg->data_base[var_id].xdata));
                // edition time
                set_edition_time(var_id,cfg);
                
                uint32 balek[1];
                io_read(var_id,balek,cfg);

            }
        }

    }
    pipeline->available=0;
    // /end_thread(t_io_read_can, NULL);
    // for(int b=0;b<index;b++){
    //     printf("var_changed : %d\n",tram_ids);
    // }

    // int index=0;


    // can_tram_db* db=cfg->can.id_data_base;

    // for(int i=0;i<pipeline->available;i++){
    //     canid=pipeline
    //     char cur_id[4];
    //     strcpy(cur_id,(db+i*sizeof(can_tram_db))->can_id);
        
    //     if(strcmp(cur_id,canid)==0){
            
    //     }
    // }
}


void set_edition_time(uint8 var_id, COM_CONFIG* cfg){
    int index = get_element_byvarid(var_id,cfg);
    cfg->data_base[index].validity[0]=1;
    cfg->data_base[index].validity[1]=1;
    
    cfg->data_base[index].edition_time=clock();
}

void check_availability(io_data_base var){

}
void float2char(char* in_char,float f_in){
    unsigned int payload;
    memcpy(&payload,&f_in,4);
    sprintf(in_char,"%08x",payload);
    //printf("CAN SEND : %s \t %f\n",test,f_in);
}
void uint32tochar(char* out_char,uint32 in_int){
    sprintf(out_char,"%x",in_int);
}
void hex2float(uint32 hex_in,float* ret){
    memcpy(ret,&hex_in,4);
}
uint32 float2uint32(float f_in){
    uint32 ret=0;
    memcpy(&ret,&f_in,4);
    //printf("floathex : %08x\n",ret);
    return ret;
}
void io_read(uint8 var_id,uint32* ret,COM_CONFIG* cfg){
    *ret=0;
    sint16 index=get_element_byvarid(var_id,cfg);
    memcpy(ret,&cfg->data_base[index].xdata,cfg->data_base[index].size/8);
    printf("io_read(%d) : %x \n",index,*ret);

}
void io_write(uint8 var_id,uint32 data,COM_CONFIG* cfg){
    sint16 index=get_element_byvarid(var_id,cfg);
    if(index==-1){
        printf("\nio_com_service.c : var_id introuvable\n");
    };

    
    //pthread_mutex_lock(&cfg->data_base[index].mutex);
    set_edition_time(var_id,cfg);
    memcpy(&(cfg->data_base[index].xdata),&data,sizeof(uint32)+1);
    //memcpy(&cfg->data_base[index].xdata,&data,cfg->data_base[index].size/8+1);
    sprintf(cfg->data_base[index].data,"%lx",data);
    //printf("data[%d] : %x\n",var_id,data);
    //printf("xdata[%d] : %x\n",var_id,cfg->data_base[index].xdata);
    //printf("&data[%d] : %s\n",var_id,cfg->data_base[index].data);
    
    
}

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
    can->id_data_base=malloc(sizeof(can_tram_db)*MAX_VAR_TO_COM);
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
    //sudo /sbin/ip link set can1 up type can bitrate 500000
    char command[]="sudo /sbin/ip link set can1 up type can bitrate 500000";

    sprintf(command,"sudo /sbin/ip link set %s up type can bitrate %ld",can->can_name,can->speed);
    if(SYSTEM_CMD_STOP!=1){
        system(command);
    }
    //printf("\n system(%s)\n",command);

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
        else if (jsoncomp(JSON_STRING, &t[k], "type") == 0){
            strncpy(tempchar,JSON_STRING+t[k+1].start,t[k+1].end-t[k+1].start+1);
            cfg->data_base[index].type=atoi(tempchar);
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
                        strncpy(tempchar_2, JSON_STRING + t[k+j+2].start, t[k+j+2].end-t[k+j+2].start+1);
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
                    pcan->id_data_base[index].can_id=malloc(sizeof(char)*3);
                    char tempchar_5[3];
                    strncpy(tempchar_5, JSON_STRING + t[k+1].start, t[k+1].end-t[k+1].start+1);
                    pcan->id_data_base[index].x_can_id=atoi(tempchar_5);
                    sprintf(pcan->id_data_base[index].can_id,"%d",pcan->id_data_base[index].x_can_id);
                    
                    index++;
                }
            }
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
    cfg->data_base=malloc(sizeof(io_data_base)*MAX_VAR_TO_COM);
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

    } 
    else {
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
    printf("\nuart enabled \t\t: %d\n",cfg.uart.enabled);
    printf("uart COM \t\t: %s\n",cfg.uart.COM);
    printf("uart speed \t\t: %ld\n",cfg.uart.speed);
    printf("uart available \t\t: %d\n",cfg.uart.available);
    printf("\ncan_config.enabled \t: %d\n",cfg.can.enabled);
    printf("can_config.name \t: %s\n",cfg.can.can_name);
    printf("can_config.speed \t: %ld\n",cfg.can.speed);
    printf("can_config.available \t: %d\n\n",cfg.can.available);
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
        printf("\n\ttype \t\t: %s ",(cfg.data_base[j].type==0)?"INT":"FLOAT");
        printf("\n\ttrigger_type \t: %s",(cfg.data_base[j].trigger_type==0)?"Time":"Event");
        printf("\n\tmedium \t\t: %s\n",(cfg.data_base[j].medium==0)?"CAN":"UART");
    }
}

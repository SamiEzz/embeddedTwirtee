#include "bits.h"

#include <math.h>



void get_bit(uint32 f_in,uint8 offset,uint8* ret){
    *ret = f_in>>offset&1;
}

void set_bit_32(uint32* f_out,uint8 offset,uint8 value){
    uint8 state;
    get_bit(*f_out,offset,&state);
    if(state==0 && value==1){
        *f_out+=(uint32)pow(2,offset);// 2^offset;
    }
    else if(state==1 && value==0){
        *f_out-=(uint32)pow(2,offset);
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
void set_bit_8(uint8* f_out,uint8 offset,uint8 value){
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
}











int main(){

    uint32 _32bits=0xDEADBEEF;
    uint8 ret8;
    uint16 ret16;
    
    char chardata[32]; //=malloc(32*sizeof(char));
    printf("\nhex : %x\n",_32bits);
    int k=0;

    
    // set_bit(&_32bits,0,1);
    // set_bit(&_32bits,1,1);
    // set_bit(&_32bits,2,1);
    // set_bit(&_32bits,3,1);
    // set_bit(&_32bits,7,1);
    // print_bits(_32bits,32);
    uint32tochar(chardata,_32bits);
    printf("\ncharfromint : %s\n",chardata);
    //memcpy((void*)b,((void*)&_32bits),32);
    
    get_uint8(_32bits,4,&ret8);
    printf("\nret8 : %x\n",ret8);
    
    get_uint16(_32bits,0,&ret16);
    printf("ret16 : %x\n",ret16);
}
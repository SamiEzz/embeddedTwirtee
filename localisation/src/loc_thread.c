//////////////////////////////////////////////////////////
//                                                      //
//      (c)         : EZZEROUALI Sami 2018              //
//      Tutor       : JENN Eric                         //
//      Project     : CAPHCA                            //
//      Program     : SPF - Shortest path first         //
//                                                      //
//////////////////////////////////////////////////////////


#include "../include/header.h"
#include "../include/loc_thread.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>
#include <pthread.h>



void config(){
    dwm_cfg_tag_t cfg_tag;
    dwm_cfg_t cfg_node;

    HAL_Print("dwm_init(): dev%d\n", HAL_DevNum());
    dwm_init();

    HAL_Print("Setting to tag: dev%d.\n", HAL_DevNum());
    cfg_tag.low_power_en = 0; 
    cfg_tag.meas_mode = DWM_MEAS_MODE_TWR;
    cfg_tag.loc_engine_en = 1;
    cfg_tag.common.led_en = 1;
    cfg_tag.common.ble_en = 1;
    cfg_tag.common.uwb_mode = DWM_UWB_MODE_ACTIVE;
    cfg_tag.common.fw_update_en = 0;
    HAL_Print("dwm_cfg_tag_set(&cfg_tag): dev%d.\n", HAL_DevNum());
    dwm_cfg_tag_set(&cfg_tag);

    HAL_Print("Wait 2s for node to reset.\n");
    HAL_Delay(2000);  
    dwm_cfg_get(&cfg_node);

    HAL_Print("Comparing set vs. get: dev%d.\n", HAL_DevNum());
    if((cfg_tag.low_power_en        != cfg_node.low_power_en) 
    || (cfg_tag.meas_mode           != cfg_node.meas_mode) 
    || (cfg_tag.loc_engine_en       != cfg_node.loc_engine_en) 
    || (cfg_tag.common.led_en       != cfg_node.common.led_en) 
    || (cfg_tag.common.ble_en       != cfg_node.common.ble_en) 
    || (cfg_tag.common.uwb_mode     != cfg_node.common.uwb_mode) 
    || (cfg_tag.common.fw_update_en != cfg_node.common.fw_update_en))
    {
        HAL_Print("low_power_en        cfg_tag=%d : cfg_node=%d\n", cfg_tag.low_power_en,     cfg_node.low_power_en); 
        HAL_Print("meas_mode           cfg_tag=%d : cfg_node=%d\n", cfg_tag.meas_mode,        cfg_node.meas_mode); 
        HAL_Print("loc_engine_en       cfg_tag=%d : cfg_node=%d\n", cfg_tag.loc_engine_en,    cfg_node.loc_engine_en); 
        HAL_Print("common.led_en       cfg_tag=%d : cfg_node=%d\n", cfg_tag.common.led_en,    cfg_node.common.led_en); 
        HAL_Print("common.ble_en       cfg_tag=%d : cfg_node=%d\n", cfg_tag.common.ble_en,    cfg_node.common.ble_en); 
        HAL_Print("common.uwb_mode     cfg_tag=%d : cfg_node=%d\n", cfg_tag.common.uwb_mode,  cfg_node.common.uwb_mode); 
        HAL_Print("common.fw_update_en cfg_tag=%d : cfg_node=%d\n", cfg_tag.common.fw_update_en, cfg_node.common.fw_update_en);  
        HAL_Print("\nConfiguration failed.\n\n");
    }
    else
    {
        HAL_Print("\nConfiguration succeeded.\n\n");
    }
   
}

void *loc_thread(void *mission_se){

    pthread_exit(NULL);
}


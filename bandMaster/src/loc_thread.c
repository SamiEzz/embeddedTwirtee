//////////////////////////////////////////////////////////
//                                                      //
//      (c)         : EZZEROUALI Sami 2018              //
//      Tutor       : JENN Eric                         //
//      Project     : CAPHCA                            //
//      Program     : SPF - Shortest path first         //
//                                                      //
//////////////////////////////////////////////////////////


#include "../include/loc_thread.h"



void *loc_thread(void *_position){
    
    position_mtx *position = _position;
    position_mtx *Pos = _position;
    

    int i;
    int wait_period = 1000;
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

    dwm_loc_data_t loc;
    dwm_pos_t pos;
    loc.p_pos = &pos;  
    int itest=0;
    //while(1) {      
      HAL_Print("Wait %d ms...\n", wait_period);
      //HAL_Delay(wait_period);        
   
      HAL_Print("dwm_loc_get(&loc):\n");
      if(dwm_loc_get(&loc) == RV_OK)
      {
        printf("\t[%d,%d,%d,%u]\n", loc.p_pos->x, loc.p_pos->y, loc.p_pos->z,loc.p_pos->qf);
        //printf("\033[H\033[2J"); // system("clear")

        printf("___________________________\n");
        printf("| mes \t|\t %d \t|\n",itest);
        printf("| x \t|\t %f |\n",Pos->position_var->x);    
        printf("| y \t|\t %f |\n",Pos->position_var->y);    
        printf("| z \t|\t %f |\n",Pos->position_var->z);    
        printf("| qf \t|\t %f |\n",Pos->position_var->qf);  
        itest+=1;  
        HAL_Delay(1);
        //  pthread_mutex_lock(position->mut);
        //  position->position_var.x=loc.p_pos->x;
        //  position->position_var.y=loc.p_pos->y;
        //  position->position_var.z=loc.p_pos->z;
        //  position->position_var.qf=loc.p_pos->qf;

         Pos->position_var->x=itest+62;
         Pos->position_var->y=itest*3;
         Pos->position_var->z=itest+6;
         Pos->position_var->qf=100;
         itest+=1;

        // pthread_mutex_unlock(position->mut);
         

         for (i = 0; i < loc.anchors.dist.cnt; ++i) 
         {
            HAL_Print("\t%u)", i);
            HAL_Print("0x%llx", loc.anchors.dist.addr[i]);
            if (i < loc.anchors.an_pos.cnt) 
            {
               HAL_Print("[%d,%d,%d,%u]", loc.anchors.an_pos.pos[i].x,
                     loc.anchors.an_pos.pos[i].y,
                     loc.anchors.an_pos.pos[i].z,
                     loc.anchors.an_pos.pos[i].qf);
               
            }
            HAL_Print("=%u,%u\n", loc.anchors.dist.dist[i], loc.anchors.dist.qf[i]);
         }
      }

   //}
   
    pthread_exit((void *)position);
}

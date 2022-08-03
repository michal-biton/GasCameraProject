#include<PrivateCommandLine.h>
#include<stdio.h>
#include <dlfcn.h>
#include<stdlib.h>
#include<assert.h>
#include"gaz_cam_lib.h"
#include"commandHandler.h"
#include"gpio.h"

#ifdef ARM
//Code not working in development now
int get_status()
{
    Status= | enum->statusRecord
            Int statusMAIN=0,Status;
    While(1){
        Status=lib->get_status();
        If(status!=statusMAIN){
            If(status & ENUMSTATUS->statusRecord){
                {
                }
                statusMAIN=status;
                msleep(500);
            }
        }
    }
}
void flash(int gpio_num,int rate,int state)
{
    while(rate!=-1)
        switch (state){
        case ON:
            off_led (rate);//on thread
            state=OFF;
            break;
        case OFF:
            on_led (rate);
            state=OFF;
            break;
        case CONST_ON:
            on_led (rate);
            state=CONST_of;
            break;
        }
    msleep (time);
    //
    get status_thread(){  //app ,0.5
        //get status from lib
        int status;   //010101000000101010100001
        lib->get_stutus();
        int* ratesta,ratesto,ratedo;
        while(no cgange)
        if (status &RECORD_ACTIVE)
        {
            ratesta=200;
            flash(24,&ratesta,FLASH_ON);

        }
        if (status & STOP_ACTIVE)
        {
            rate=500;
            state=on;
        }
        if (status & do_snap_ACTIVE)
        {
            &rate=0;
            state =CONST_ON
        }        flash(&state,rate); //one
#endif

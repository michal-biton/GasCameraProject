#include<stdio.h>
#include <dlfcn.h>
#include<stdlib.h>
#include<assert.h>
#include"gaz_cam_lib.h"
#include"gpio.h"
#include"unistd.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
extern gas_api* p_gaz;
extern p_handler handler;

#ifdef ARM
void on_led(int rate)
{
    int fd = export_gpio();
    while(1) {
        assert(write(fd, "1", 1));
        usleep(rate);
        assert(write(fd, "0", 1));
        usleep(rate);
    }
}
int export_gpio(){
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    assert(fd);
    assert(write(fd, "24", 2) == 2);
    close(fd);
    fd = open("/sys/class/gpio/gpio24/direction", O_WRONLY);
    assert(fd!=-1);
    assert(write(fd, "24", 2) == 3);
    close(fd);
    fd = open("/sys/class/gpio/gpio24/value", O_WRONLY);
    assert(fd!=-1);
    return fd;
}
int unexport_gpio(){
    int fd = open("/sys/class/gpio/unexport", O_WRONLY);
    assert(fd);
    assert(write(fd, "24", 2) == 2);
    close(fd);
    return 1;
}
void off_led()
{
    unexport_gpio();
}
void * flash(void * flash)
{
    flash_t* current_flesh=(flash_t*)flash;
    while(*(current_flesh->rate)>=0)
    {
        switch(current_flesh->state)
        {
        case FLASH_ON:
            off_led();
            current_flesh->state=FLASH_OFF;
            break;
        case FLASH_OFF:
            on_led(current_flesh->gpio);
            current_flesh->state=FLASH_ON;
            break;
        case CONST_ON:
            on_led(current_flesh->gpio);
            break;
        }
        p_gaz->msleep(*(current_flesh->rate));
    }
    off_led();
    return NULL;
}

void init_flash(flash_t * flash,int rate,int gpio,enum STATE_MACHIN state){
    flash->gpio=gpio;
    flash->rate=&rate;
    flash->state=state;
}

void* main_gpio(void* x)
{
    int status,last_status=0;
    flash_t * start=(flash_t*)calloc(1,sizeof (flash_t));
    assert(start);
    flash_t * stop=(flash_t*)calloc(1,sizeof (flash_t));
    assert(stop);
    is_active=1;
    pthread_t thread_start,thread_stop;
    while(is_active)
    {
        status=p_gaz->get_status(handler);
        if(status!=last_status)
        {
            if(status & RECORD_ACTIVE){
                init_flash(start,500,24,FLASH_ON);
                pthread_create(&thread_start,NULL,flash,start);
            }
            else
                *(start->rate)=-1;
            if(status & STOP_RECORD_ACTIVE){
                init_flash(stop,200,24,FLASH_ON);
                pthread_create(&thread_stop,NULL,flash,stop);
            }
            else
                *(stop->rate)=-1;
        }
        last_status=status;
        sleep(p_gaz->msleep(500));
        printf("i'm sleeping");
    }
    free(start);
    free(stop);
}
#endif

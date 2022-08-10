#ifndef GPIO_H
#define GPIO_H

enum STATE_MACHIN{
    FLASH_ON,
    FLASH_OFF,
    CONST_ON,
};

typedef struct flash_t{
    int gpio;
    int* rate;
    enum STATE_MACHIN state;
}flash_t;
static int is_active;
void on_led(int );
void off_led();
void * flash(void *);
int export_gpio();
int unexport_gpio();
void init_flash(flash_t * ,int ,int ,enum STATE_MACHIN );
void* main_gpio(void*);
#endif // GPIO_H

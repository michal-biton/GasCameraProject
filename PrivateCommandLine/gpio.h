#ifndef GPIO_H
#define GPIO_H

enum status{
   CAPTURE_ACTIVE      = 1,
   RECORD_ACTIVE       = 2,
   STOP_RECORD_ACTIVE  = 4,
   SNAPSHOT_ACTIVE     = 8
};
enum STATE_MACHIN{
FLASH_ON,
FLASH_OFF,
CONST_ON,
CONST_OFF
};

#endif // GPIO_H

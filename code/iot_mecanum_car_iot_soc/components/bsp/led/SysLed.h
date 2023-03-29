#ifndef _SYS_LED_H_
#define _SYS_LED_H_

#include <stdint.h>

class SysLed{
public:
    static SysLed sysLed;
    void start();
    void stop();
    void setFre( float fre , uint8_t onMs );
private:
    SysLed(){};
    static void ledTask(void* param);
    uint16_t _offMs;
    uint8_t  _onMs;
};

#endif  //_SYS_LED_H_


#ifndef _HMI_H_
#define _HMI_H_

#define KEY_SCAN_CYCLE      20
#define KEY_LONG_THRESHOLD  800     //长按阈值

#define KEY_INPUT_BEEP_FRE  500
#define KEY_INPUT_BEEP_TIME 160

#define SYS_INIT_BEEP_FRE   500
#define SYS_INIT_BEEP_COUNT 3
#define SYS_INIT_BEEP_TIME  120
#define SYS_INIT_BEEP_OFF_TIME  100

#include <stdint.h>

typedef enum{
    BEEP_NONE,
    BEEP_KEY_INPUT,
    BEEP_SYS_INIT_FINNISH,
}beep_notice_t;

typedef enum{
    KEY_PRESS,
    KEY_RELEASE,
    KEY_SHORT,
    KEY_TIMEOUT,
    KEY_DOUBLE
}keyboard_event_t;

typedef struct{
    keyboard_event_t evt;
    uint8_t keycode;
}keyboard_hmi_input_t;

void hmi_start( void );
void beep_notice( beep_notice_t notice );

#endif  //_HMI_H_

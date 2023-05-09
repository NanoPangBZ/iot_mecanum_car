#ifndef _LORI_GUI_TYDEF_H_
#define _LORI_GUI_TYDEF_H_

#include "lori_gui.h"

typedef enum{
    GUI_EVT_NEXT,
    GUI_EVT_LAST,
    GUI_EVT_SURE,
    GUI_EVT_CANCEL
}gui_evt_t;

typedef struct{
    void (*init)(void);
    void (*deinit)(void);
    void (*timer_handler)( uint16_t ms );
    void (*event_handle)( gui_evt_t evt );
}page_t;

#endif  //_LORI_GUI_TYDEF_H_


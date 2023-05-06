#ifndef _PAGE_DEFINE_H_
#define _PAGE_DEFINE_H_

#include "gui.h"

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

extern const page_t welcome_page;
extern const page_t home_page;

#endif  //_PAGE_DEFINE_H_


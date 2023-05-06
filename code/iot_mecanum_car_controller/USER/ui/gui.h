#ifndef _GUI_H_
#define _GUI_H_

#include <stdint.h>

typedef struct{
    void (*init)(void);
    void (*deinit)(void);
    void (*timer_handler)( uint16_t ms );
}page_t;

extern const page_t welcome_page;

void ui_init( const page_t* first_page );
void ui_deinit( void );
void ui_timer_handler( uint16_t ms );
void ui_set_page( const page_t *opt );

#endif  //_UI_H_


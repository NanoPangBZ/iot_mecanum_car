#ifndef _GUI_H_
#define _GUI_H_

#include <stdint.h>
#include "page_def.h"

void gui_init( const page_t* first_page );
void gui_deinit( void );
void gui_timer_handler( uint16_t ms );
void gui_set_page( const page_t *opt );
void gui_evt_input( gui_evt_t evt );

#endif  //_UI_H_


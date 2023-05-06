#include <stdio.h>
#include "gui.h"

static const page_t* current_page = NULL;

static void current_page_init( void );
static void current_page_deinit( void );
static void current_page_timer_handler( uint16_t ms );
static void current_page_event_handler( gui_evt_t evt );

void current_page_init( void )
{
    if( current_page != NULL && current_page->init != NULL )
    {
        current_page->init();
        current_page->timer_handler(0);
    }
}

void current_page_deinit( void )
{
    if( current_page != NULL && current_page->deinit != NULL )
    {
        current_page->deinit();
    }
}

void current_page_timer_handler( uint16_t ms )
{
    if( current_page != NULL && current_page->timer_handler != NULL )
    {
        current_page->timer_handler( ms );
    }
}

void current_page_event_handler( gui_evt_t evt )
{
    if( current_page != NULL && current_page->event_handle != NULL )
    {
        current_page->event_handle( evt );
    }
}

void gui_evt_input( gui_evt_t evt )
{
    current_page_event_handler( evt );
}

void gui_init( const page_t* first_page )
{
    current_page = first_page;
    current_page_init();
}

void gui_deinit( void )
{
    current_page_deinit();
    current_page = NULL;
}

void gui_timer_handler( uint16_t ms )
{
    if( current_page != NULL )
    {
        current_page->timer_handler( ms );
    }
}

void gui_set_page( const page_t *opt )
{
    current_page_deinit();
    current_page = opt;
    current_page_init();
}

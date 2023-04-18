#include <stdio.h>
#include "ui.h"

static const page_t* current_page = NULL;

static void current_page_init( void )
{
    if( current_page != NULL && current_page->init != NULL )
    {
        current_page->init();
        current_page->timer_handler(0);
    }
}

static void current_page_deinit( void )
{
    if( current_page != NULL && current_page->deinit != NULL )
    {
        current_page->init();
    }
}

static void current_page_timer_handler( uint16_t ms )
{
    if( current_page != NULL && current_page->timer_handler != NULL )
    {
        current_page->timer_handler( ms );
    }
}

void ui_init( const page_t* first_page )
{
    current_page = first_page;
    current_page_init();
}

void ui_deinit( void )
{
    current_page_deinit();
    current_page = NULL;
}

void ui_timer_handler( uint16_t ms )
{
    if( current_page != NULL )
    {
        current_page->timer_handler( ms );
    }
}

void ui_set_page( const page_t *opt )
{
    current_page = opt;
    if( current_page->init != NULL )
    {
        
    }
}

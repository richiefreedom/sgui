/*
 * button.c
 * This file is part of sgui
 *
 * Copyright (C) 2012 - David Oberhollenzer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include "sgui_button.h"
#include "sgui_skin.h"
#include "sgui_font_manager.h"

#include "widget_internal.h"

#include <stdlib.h>
#include <string.h>



typedef struct
{
    sgui_widget widget;

    unsigned char* text;
    unsigned int text_width;
    int state;
}
sgui_button;



void sgui_button_on_event( sgui_widget* widget, sgui_window* wnd,
                           int type, sgui_event* event )
{
    sgui_button* b = (sgui_button*)widget;

    if( type == SGUI_DRAW_EVENT )
    {
        sgui_skin_draw_button( wnd, widget->x, widget->y, b->state,
                               widget->width, b->text_width,
                               widget->height, b->text );
    }
    else if( type == SGUI_MOUSE_LEAVE_EVENT )
    {
        b->state = 0;
        b->widget.need_redraw = 1;
    }
    else if( type == SGUI_MOUSE_PRESS_EVENT )
    {
        if( event->mouse_press.button != SGUI_MOUSE_BUTTON_LEFT )
            return;

        if( event->mouse_press.pressed )
        {
            b->state = 1;
        }
        else
        {
            if( b->state )
            {
                sgui_internal_widget_fire_event( widget,
                                                 SGUI_BUTTON_CLICK_EVENT );
            }

            b->state = 0;
        }

        b->widget.need_redraw = 1;
    }
}



sgui_widget* sgui_button_create( int x, int y, const unsigned char* text )
{
    sgui_button* b;

    unsigned int w, h, len = strlen( (const char*)text );

    b = malloc( sizeof(sgui_button) );

    sgui_skin_get_button_extents( text, &w, &h, &b->text_width );

    sgui_internal_widget_init( (sgui_widget*)b, x, y, w, h, 1 );

    b->widget.window_event_callback = sgui_button_on_event;
    b->text                         = malloc( len + 1 );
    b->state                        = 0;

    memcpy( b->text, text, len + 1 );

    return (sgui_widget*)b;
}

void sgui_button_destroy( sgui_widget* button )
{
    if( button )
    {
        sgui_internal_widget_deinit( button );

        free( ((sgui_button*)button)->text );
        free( button );
    }
}


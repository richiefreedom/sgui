/*
 * group_box.c
 * This file is part of sgio
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
#include "sgui_group_box.h"
#include "sgui_widget_manager.h"
#include "sgui_skin.h"

#include "widget_internal.h"

#include <stdlib.h>
#include <string.h>



typedef struct
{
    sgui_widget widget;

    sgui_widget_manager* mgr;

    char* caption;
}
sgui_group_box;




void group_box_on_event( sgui_widget* widget, int type, sgui_event* event )
{
    sgui_group_box* b = (sgui_group_box*)widget;

    sgui_widget_manager_send_window_event( b->mgr, type, event );
}

void group_box_update( sgui_widget* widget )
{
    sgui_group_box* b = (sgui_group_box*)widget;

    widget->need_redraw = sgui_widget_manager_update( b->mgr );
}

void group_box_draw( sgui_widget* widget, sgui_canvas* cv )
{
    sgui_group_box* b = (sgui_group_box*)widget;

    /* draw background */
    sgui_skin_draw_group_box( cv, widget->x, widget->y,
                                  widget->width, widget->height, b->caption );

    /* adjust offset and scissor rect to box area */
    sgui_canvas_set_offset( cv, widget->x, widget->y );
    sgui_canvas_set_scissor_rect( cv, 0, 0, widget->width, widget->height );

    /* draw the widgets */
    sgui_widget_manager_force_draw( b->mgr, cv, 0, 0,
                                    widget->width, widget->height );

    /* restore scissor rect and offset */
    sgui_canvas_set_scissor_rect( cv, 0, 0, 0, 0 );
    sgui_canvas_restore_offset( cv );
}





sgui_widget* sgui_group_box_create( int x, int y,
                                    unsigned int width, unsigned int height,
                                    const char* caption )
{
    sgui_group_box* b = malloc( sizeof(sgui_group_box) );

    if( !b )
        return NULL;

    b->mgr = sgui_widget_manager_create( );

    if( !b->mgr )
    {
        free( b );
        return NULL;
    }

    sgui_widget_manager_enable_clear( b->mgr, 0 );

    b->caption = malloc( strlen( caption ) + 1 );

    if( !b->caption )
    {
        sgui_widget_manager_destroy( b->mgr );
        free( b );
        return NULL;
    }

    strcpy( b->caption, caption );

    sgui_internal_widget_init( (sgui_widget*)b, x, y, width, height, 0 );

    b->widget.draw_callback         = group_box_draw;
    b->widget.update_callback       = group_box_update;
    b->widget.window_event_callback = group_box_on_event;

    return (sgui_widget*)b;

}

void sgui_group_box_destroy( sgui_widget* box )
{
    sgui_group_box* b = (sgui_group_box*)box;

    if( b )
    {
        sgui_internal_widget_deinit( box );
        sgui_widget_manager_destroy( b->mgr );
        free( b->caption );
        free( b );
    }
}

void sgui_group_box_add_widget( sgui_widget* box, sgui_widget* w )
{
    sgui_group_box* b = (sgui_group_box*)box;

    if( b )
        sgui_widget_manager_add_widget( b->mgr, w );
}

void sgui_group_box_remove_widget( sgui_widget* box, sgui_widget* w )
{
    sgui_group_box* b = (sgui_group_box*)box;

    if( b )
        sgui_widget_manager_remove_widget( b->mgr, w );
}

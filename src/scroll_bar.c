/*
 * scroll_bar.c
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
#include "sgui_scroll_bar.h"
#include "sgui_skin.h"
#include "widget_internal.h"

#include <stdlib.h>



typedef struct
{
    sgui_widget widget;
    unsigned int bw, bh, length, p_length, v_length, p_offset, v_offset,
                 v_max;
    int horizontal, inc_button_state, dec_button_state;
}
sgui_scroll_bar;



void scroll_bar_on_event_h( sgui_widget* widget, int type, sgui_event* event )
{
    sgui_scroll_bar* b = (sgui_scroll_bar*)widget;
    unsigned int l = b->length - 2*b->bw;

    if( type==SGUI_MOUSE_RELEASE_EVENT || type==SGUI_MOUSE_LEAVE_EVENT )
    {
        if( b->dec_button_state || b->inc_button_state )
            widget->need_redraw = 1;

        b->dec_button_state = b->inc_button_state = 0;
    }
    else if( type==SGUI_MOUSE_PRESS_EVENT )
    {
        b->dec_button_state = event->mouse_press.x <
                              (widget->x+(int)b->bw);
        b->inc_button_state = event->mouse_press.x >
                              (widget->x+(int)b->length-(int)b->bw);

        widget->need_redraw = 1;

        if( b->inc_button_state )
        {
            if( ((b->v_offset + b->v_length + b->v_length/4) < b->v_max) &&
                ((b->p_offset + b->p_length + b->p_length/4) < l) )
            {
                b->p_offset += b->p_length / 4;
                b->v_offset += b->v_length / 4;
                widget->need_redraw = 1;
            }
            else
            {
                b->p_offset = l - b->p_length;
                b->v_offset = b->v_max - b->v_length;
                widget->need_redraw = 1;            
            }
        }
        else if( b->dec_button_state )
        {
            if( (b->p_offset > (b->p_length/4)) &&
                (b->v_offset > (b->v_length/4)) )
            {
                b->p_offset -= b->p_length / 4;
                b->v_offset -= b->v_length / 4;
                widget->need_redraw = 1;
            }
            else
            {
                b->p_offset = 0;
                b->v_offset = 0;
                widget->need_redraw = 1;            
            }
        }
    }
}

void scroll_bar_on_event_v( sgui_widget* widget, int type, sgui_event* event )
{
    sgui_scroll_bar* b = (sgui_scroll_bar*)widget;
    unsigned int l = b->length - 2*b->bh;

    if( type==SGUI_MOUSE_RELEASE_EVENT || type==SGUI_MOUSE_LEAVE_EVENT )
    {
        if( b->dec_button_state || b->inc_button_state )
            widget->need_redraw = 1;

        b->dec_button_state = b->inc_button_state = 0;
    }
    else if( type==SGUI_MOUSE_PRESS_EVENT )
    {
        b->dec_button_state = event->mouse_press.y <
                              (widget->y+(int)b->bh);
        b->inc_button_state = event->mouse_press.y >
                              (widget->y+(int)b->length-(int)b->bh);

        widget->need_redraw = 1;

        if( b->inc_button_state )
        {
            if( ((b->v_offset + b->v_length + b->v_length/4) < b->v_max) &&
                ((b->p_offset + b->p_length + b->p_length/4) < l) )
            {
                b->p_offset += b->p_length / 4;
                b->v_offset += b->v_length / 4;
                widget->need_redraw = 1;
            }
            else
            {
                b->p_offset = l - b->p_length;
                b->v_offset = b->v_max - b->v_length;
                widget->need_redraw = 1;            
            }
        }
        else if( b->dec_button_state )
        {
            if( (b->p_offset > (b->p_length/4)) &&
                (b->v_offset > (b->v_length/4)) )
            {
                b->p_offset -= b->p_length / 4;
                b->v_offset -= b->v_length / 4;
                widget->need_redraw = 1;
            }
            else
            {
                b->p_offset = 0;
                b->v_offset = 0;
                widget->need_redraw = 1;            
            }
        }
    }
}

void scroll_bar_draw( sgui_widget* widget, sgui_canvas* cv )
{
    sgui_scroll_bar* b = (sgui_scroll_bar*)widget;

    sgui_skin_draw_scroll_bar( cv, widget->x, widget->y,
                               b->horizontal, b->length,
                               b->p_offset, b->p_length,
                               b->inc_button_state, b->dec_button_state );
}



sgui_widget* sgui_scroll_bar_create( int x, int y, int horizontal,
                                     unsigned int length,
                                     unsigned int scroll_area_length,
                                     unsigned int disp_area_length )
{
    sgui_scroll_bar* b = malloc( sizeof(sgui_scroll_bar) );
    unsigned int w, h;

    sgui_skin_get_scroll_bar_extents( horizontal, length, &w, &h,
                                      &b->bw, &b->bh );

    sgui_internal_widget_init( (sgui_widget*)b, x, y, w, h, 0 );

    if( horizontal )
        b->widget.window_event_callback = scroll_bar_on_event_h;
    else
        b->widget.window_event_callback = scroll_bar_on_event_v;

    b->widget.draw_callback = scroll_bar_draw;
    b->horizontal           = horizontal;
    b->length               = length;
    b->v_length             = disp_area_length;
    b->v_max                = scroll_area_length;
    b->p_offset             = 0;
    b->v_offset             = 0;
    b->inc_button_state     = 0;
    b->dec_button_state     = 0;
    b->p_length             = ((float)b->v_length / (float)b->v_max) *
                              (length - 2*(horizontal ? b->bw : b->bh));

    return (sgui_widget*)b;
}

void sgui_scroll_bar_destroy( sgui_widget* bar )
{
    if( bar )
    {
        sgui_internal_widget_deinit( bar );
        free( bar );
    }
}

void sgui_scroll_bar_set_offset( sgui_widget* bar, unsigned int offset )
{
    sgui_scroll_bar* b = (sgui_scroll_bar*)bar;

    if( b )
    {
        unsigned int l = b->length - 2*(b->horizontal ? b->bh : b->bw);

        if( (offset + b->v_length) < b->v_max )
        {
            b->v_offset = offset;
            b->p_offset = ((float)offset/(float)b->v_max) * l;
        }
        else
        {
            b->v_offset = b->v_max - b->v_length;
            b->p_offset = l - b->p_length;
        }

        bar->need_redraw = 1;
    }
}

unsigned int sgui_scroll_bar_get_offset( sgui_widget* bar )
{
    return bar ? (((sgui_scroll_bar*)bar)->v_offset) : 0;
}

void sgui_scroll_bar_set_area( sgui_widget* bar,
                               unsigned int scroll_area_length,
                               unsigned int disp_area_length )
{
    sgui_scroll_bar* b = (sgui_scroll_bar*)bar;

    if( b )
    {
        b->v_length = disp_area_length;
        b->v_max    = scroll_area_length;
        b->p_length = ((float)b->v_length / (float)b->v_max) *
                       (b->length - 2*(b->horizontal ? b->bw : b->bh));

        bar->need_redraw = 1;
    }
}

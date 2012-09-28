/*
 * edit_box.c
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
#include "sgui_edit_box.h"
#include "sgui_skin.h"
#include "sgui_internal.h"

#include <stdlib.h>
#include <string.h>



typedef struct
{
    sgui_widget widget;

    unsigned int max_chars, num_entered;
    unsigned int end;

    unsigned int cursor;
    unsigned int offset;

    int draw_cursor;

    char* buffer;
}
sgui_edit_box;



#define ROLL_BACK_UTF8( buffer, index )\
            --(index);\
            while( (index) && (((buffer)[ index ]) & 0xC0) == 0x80 )\
                --(index);

#define ADVANCE_UTF8( buffer, index )\
            ++(index);\
            while( (((buffer)[ index ]) & 0xC0) == 0x80 )\
                ++(index);


/* Adjust text render offset after moving the cursor */
void sgui_edit_box_determine_offset( sgui_edit_box* b )
{
    unsigned int cx, w;

    if( !b->num_entered )
        return;

    cx = sgui_skin_default_font_extents( b->buffer + b->offset,
                                         b->cursor - b->offset, 0, 0 );

    w = SGUI_RECT_WIDTH( b->widget.area );

    if( b->offset && (b->cursor <= b->offset) )
    {
        /* roll offset back, until render space is exceeded */
        while( b->offset && (cx < w) )
        {
            ROLL_BACK_UTF8( b->buffer, b->offset );

            cx = sgui_skin_default_font_extents( b->buffer + b->offset,
                                                 b->cursor - b->offset,
                                                 0, 0 );
        }

        /* move offset forward by one, to fit exactely into the bar */
        if( b->offset )
        {
            ADVANCE_UTF8( b->buffer, b->offset );
        }
    }
    else if( cx > w )
    {
        b->offset = b->cursor;

        ROLL_BACK_UTF8( b->buffer, b->offset );
    }
}

/* get a cursor position from a mouse offset */
unsigned int sgui_edit_box_cursor_from_mouse( sgui_edit_box* b, int mouse_x )
{
    unsigned int len = 0, cur = b->offset;

    while( (len < (unsigned int)mouse_x) && (cur < b->end) )
    {
        ADVANCE_UTF8( b->buffer, cur );

        len = sgui_skin_default_font_extents( b->buffer+b->offset,
                                              cur      -b->offset, 0, 0 );
    }

    return cur;
}



void sgui_edit_box_draw( sgui_widget* widget, sgui_canvas* cv )
{
    sgui_edit_box* b = (sgui_edit_box*)widget;

    sgui_skin_draw_edit_box( cv, widget->area.left, widget->area.top,
                             b->buffer + b->offset,
                             SGUI_RECT_WIDTH(widget->area),
                             b->draw_cursor ?
                             (int)(b->cursor-b->offset) : -1 );
}

void sgui_edit_box_on_event( sgui_widget* widget, int type,
                             sgui_event* event )
{
    sgui_edit_box* b = (sgui_edit_box*)widget;
    sgui_rect r;

    sgui_widget_get_rect( widget, &r );

    if( type == SGUI_FOCUS_EVENT )
    {
        b->draw_cursor = 1;
        sgui_widget_manager_add_dirty_rect( widget->mgr, &r );
    }
    else if( type == SGUI_FOCUS_LOSE_EVENT )
    {
        b->draw_cursor = 0;
        sgui_widget_manager_add_dirty_rect( widget->mgr, &r );
    }
    else if( (type == SGUI_MOUSE_RELEASE_EVENT) &&
             (event->mouse_press.button == SGUI_MOUSE_BUTTON_LEFT) &&
             b->num_entered )
    {
        unsigned int new_cur;
    
        new_cur = sgui_edit_box_cursor_from_mouse( b, event->mouse_press.x );

        if( new_cur != b->cursor )
        {
            b->cursor = new_cur;
            sgui_widget_manager_add_dirty_rect( widget->mgr, &r );
        }
    }
    else if( type == SGUI_KEY_PRESSED_EVENT )
    {
        if( (event->keyboard_event.code==SGUI_KC_BACK) && b->num_entered &&
            b->cursor )
        {
            unsigned int old = b->cursor;

            ROLL_BACK_UTF8( b->buffer, b->cursor );

            /* move entire text block back by one character */
            memmove( b->buffer+b->cursor, b->buffer+old, b->end-old+1 );

            /* update state */
            b->num_entered -= 1;
            b->end -= (old - b->cursor);

            sgui_widget_manager_add_dirty_rect( widget->mgr, &r );
            sgui_edit_box_determine_offset( b );
        }
        else if( (event->keyboard_event.code==SGUI_KC_DELETE) &&
                 (b->cursor < b->end) && b->num_entered )
        {
            unsigned int offset = b->cursor;

            ADVANCE_UTF8( b->buffer, offset );

            /* move entire text block back by one character */
            memmove( b->buffer+b->cursor, b->buffer+offset, b->end-offset+1 );

            /* update state */
            b->num_entered -= 1;
            b->end -= (offset - b->cursor);

            sgui_widget_manager_add_dirty_rect( widget->mgr, &r );
            sgui_edit_box_determine_offset( b );
        }
        else if( (event->keyboard_event.code==SGUI_KC_LEFT) && b->cursor )
        {
            ROLL_BACK_UTF8( b->buffer, b->cursor );

            sgui_widget_manager_add_dirty_rect( widget->mgr, &r );
            sgui_edit_box_determine_offset( b );
        }
        else if( (event->keyboard_event.code==SGUI_KC_RIGHT) &&
                 (b->cursor < b->end) )
        {
            ADVANCE_UTF8( b->buffer, b->cursor );

            sgui_widget_manager_add_dirty_rect( widget->mgr, &r );
            sgui_edit_box_determine_offset( b );
        }
        else if( (event->keyboard_event.code==SGUI_KC_HOME) &&
                 (b->cursor || b->offset) )
        {
            b->cursor = 0;
            b->offset = 0;
            sgui_widget_manager_add_dirty_rect( widget->mgr, &r );
        }
        else if( (event->keyboard_event.code==SGUI_KC_END) &&
                 (b->cursor < b->end) )
        {
            b->cursor = b->end;
            b->offset = b->end;
            sgui_widget_manager_add_dirty_rect( widget->mgr, &r );
            sgui_edit_box_determine_offset( b );
        }
    }
    else if( (type == SGUI_CHAR_EVENT) && (b->num_entered < b->max_chars) )
    {   
        unsigned int len;

        len = strlen( event->char_event.as_utf8_str );

        /* move entire text block right by the character length */
        memmove( b->buffer + b->cursor + len, b->buffer + b->cursor,
                 b->end - b->cursor + 1 );

        /* insert the character */
        memcpy( b->buffer + b->cursor, event->char_event.as_utf8_str, len );

        /* update state */
        b->end += len;
        b->cursor += len;
        b->num_entered += 1;
        sgui_widget_manager_add_dirty_rect( widget->mgr, &r );
        sgui_edit_box_determine_offset( b );
    }
}



sgui_widget* sgui_edit_box_create( int x, int y, unsigned int width,
                                   unsigned int max_chars )
{
    sgui_edit_box* b;

    b = malloc( sizeof(sgui_edit_box) );

    if( !b )
        return NULL;

    memset( b, 0, sizeof(sgui_edit_box) );

    b->buffer = malloc( max_chars * 6 + 1 );

    if( !b->buffer )
    {
        free( b );
        return NULL;
    }

    sgui_internal_widget_init( (sgui_widget*)b, x, y, width,
                               sgui_skin_get_edit_box_height( ), 0 );

    b->widget.window_event_callback = sgui_edit_box_on_event;
    b->widget.draw_callback         = sgui_edit_box_draw;
    b->max_chars                    = max_chars;
    b->buffer[0]                    = '\0';

    return (sgui_widget*)b;
}

void sgui_edit_box_destroy( sgui_widget* box )
{
    if( box )
    {
        sgui_internal_widget_deinit( box );

        free( ((sgui_edit_box*)box)->buffer );
        free( box );
    }
}

const char* sgui_edit_box_get_text( sgui_widget* box )
{
    return box ? ((sgui_edit_box*)box)->buffer : NULL;
}

void sgui_edit_box_set_text( sgui_widget* box, const char* text )
{
    unsigned int i;
    sgui_edit_box* b = (sgui_edit_box*)box;
    sgui_rect r;

    if( !box )
        return;

    if( !text )
    {
        b->num_entered = 0;
        b->buffer[ 0 ] = '\0';
        b->end = 0;
        b->cursor = 0;
        b->offset = 0;
        return;
    }

    for( b->num_entered=0, i=0; b->num_entered<b->max_chars && *text;
         ++b->num_entered )
    {
        b->buffer[ i++ ] = *(text++);

        while( (*text & 0xC0) == 0x80 )
            b->buffer[ i++ ] = *(text++);
    }

    b->buffer[ i ] = '\0';
    b->end = i;

    b->cursor = 0;
    b->offset = 0;

    sgui_widget_get_rect( box, &r );
    sgui_widget_manager_add_dirty_rect( box->mgr, &r );
}

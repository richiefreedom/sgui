/*
 * ctx_window.c
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
#include "sgui_ctx_window.h"
#include "sgui_tex_canvas.h"
#include "sgui_ctx_wm.h"
#include "sgui_event.h"
#include "sgui_skin.h"

#include <stdlib.h>
#include <string.h>



static void get_mouse_position( sgui_window* super, int* x, int* y )
{
    sgui_ctx_window* this = (sgui_ctx_window*)super;

    sgui_window_get_mouse_position( this->parent, x, y );
    *x -= super->x;
    *y -= super->y;
}

static void set_mouse_position( sgui_window* this, int x, int y )
{
    sgui_window_set_mouse_position( ((sgui_ctx_window*)this)->parent,
                                    x + this->x, y + this->y, 0 );
}

static void set_visible( sgui_window* this, int visible )
{
    this->visible = visible;
}

static void set_title( sgui_window* this, const char* title )
{
    unsigned char color[4] = { 0x00, 0x00, 0x00, 0xFF };
    sgui_rect r;

    sgui_rect_set_size( &r, 0, 0, this->w, 20 );

    sgui_canvas_begin( this->ctx.canvas, &r );
    sgui_canvas_clear( this->ctx.canvas, &r );
    sgui_canvas_draw_text_plain( this->ctx.canvas, 15, 0, 1, 0,
                                 sgui_skin_get( )->font_color,
                                 title, -1 );

    sgui_canvas_draw_line( this->ctx.canvas, 0, r.bottom, this->w, 1,
                           color, SGUI_RGBA8 );
    sgui_canvas_end( this->ctx.canvas );
}

static void set_size( sgui_window* super,
                      unsigned int width, unsigned int height )
{
    /* TODO: implement */
    (void)super;
    (void)width;
    (void)height;
}

static void move_center( sgui_window* super )
{
    sgui_ctx_window* this = (sgui_ctx_window*)super;
    unsigned int w, h;

    sgui_window_get_size( this->parent, &w, &h );

    super->x = w/2 - super->w/2;
    super->y = h/2 - super->h/2;
}

static void move( sgui_window* this, int x, int y )
{
    this->x = x;
    this->y = y;
}

static void destroy( sgui_window* super )
{
    sgui_ctx_window* this = (sgui_ctx_window*)super;

    if( this->wm )
        sgui_ctx_wm_remove_window( this->wm, super );

    sgui_canvas_destroy( super->ctx.canvas );
    free( this );
}

static void force_redraw( sgui_window* super, sgui_rect* r )
{
    /* TODO: clarify what to do, implement */
    (void)super; (void)r;
}

static void get_platform_data( const sgui_window* super, void* ptr )
{
    /* TODO: clarify what to do, implement */
    (void)super; (void)ptr;
}

static void write_clipboard( sgui_window* this, const char* text,
                             unsigned int length )
{
    sgui_window_write_clipboard( ((sgui_ctx_window*)this)->parent,
                                 text, length );
}

static const char* read_clipboard( sgui_window* this )
{
    return sgui_window_read_clipboard( ((sgui_ctx_window*)this)->parent );
}

/****************************************************************************/

sgui_window* sgui_ctx_window_create( sgui_window* parent,
                                     unsigned int width, unsigned int height,
                                     int flags )
{
    sgui_ctx_window* this;
    unsigned int pw, ph;
    sgui_window* super;

    /* saity check */
    if( !parent || !width || !height )
        return NULL;

    if( parent->backend==SGUI_NATIVE || parent->backend==SGUI_CUSTOM )
        return NULL;

    if( flags & (~SGUI_FIXED_SIZE) )
        return NULL;

    /* create structure */
    this = malloc( sizeof(sgui_ctx_window) );
    super = (sgui_window*)this;

    if( !this )
        return NULL;

    memset( this, 0, sizeof(sgui_ctx_window) );

    /* create canvas */
    super->ctx.canvas = sgui_tex_canvas_create( parent, parent->ctx.ctx,
                                                width, height );

    if( !super->ctx.canvas )
    {
        free( this );
        return NULL;
    }

    /* clear canvas */
    sgui_canvas_begin( super->ctx.canvas, NULL );
    sgui_canvas_clear( super->ctx.canvas, NULL );
    sgui_canvas_end( super->ctx.canvas );

    /* initialize */
    sgui_window_get_size( parent, &pw, &ph );

    this->parent = parent;

    super->x       = pw/2 - width/2;
    super->y       = ph/2 - height/2;
    super->w       = width;
    super->h       = height;
    super->backend = SGUI_NATIVE;

    /* hook callbacks */
    super->get_mouse_position = get_mouse_position;
    super->set_mouse_position = set_mouse_position;
    super->set_visible        = set_visible;
    super->set_title          = set_title;
    super->set_size           = set_size;
    super->move_center        = move_center;
    super->move               = move;
    super->force_redraw       = force_redraw;
    super->get_platform_data  = get_platform_data;
    super->write_clipboard    = write_clipboard;
    super->read_clipboard     = read_clipboard;
    super->destroy            = destroy;

    return super;
}

void sgui_ctx_window_update_canvas( sgui_window* this )
{
    if( this )
        sgui_canvas_redraw_widgets( this->ctx.canvas, 1 );
}

void* sgui_ctx_window_get_texture( sgui_window* this )
{
    return this ? sgui_tex_canvas_get_texture( this->ctx.canvas ) : NULL;
}

void sgui_ctx_window_inject_event( sgui_window* this, const sgui_event* ev )
{
    sgui_event copy;

    if( this && ev )
    {
        /* copy event, change source window */
        copy = *ev;
        copy.src.window = this;

        /* reposition mouse events */
        if( copy.type == SGUI_MOUSE_MOVE_EVENT )
        {
            copy.arg.i2.x -= this->x;
            copy.arg.i2.y -= this->y;
        }
        else if( copy.type == SGUI_MOUSE_PRESS_EVENT ||
                 copy.type == SGUI_MOUSE_RELEASE_EVENT )
        {
            copy.arg.i3.x -= this->x;
            copy.arg.i3.y -= this->y;
        }

        /* send to canvas */
        sgui_canvas_send_window_event( this->ctx.canvas, &copy );
    }
}


/*
 * subview.c
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
#define SGUI_BUILDING_DLL
#include "sgui_subview.h"
#include "sgui_event.h"
#include "sgui_widget.h"
#include "sgui_internal.h"

#include <stdlib.h>



typedef struct
{
    sgui_widget super;

    sgui_window* subwnd;

    sgui_subview_window_fun window_fun;
    sgui_subview_draw_fun draw_fun;
}
sgui_subview;



/* processes events from the parent canvas containing a subview widget */
static void subview_on_parent_event( sgui_widget* super, const sgui_event* e )
{
    sgui_subview* this = (sgui_subview*)super;

    switch( e->type )
    {
    case SGUI_TAB_DESELECTED:
        /* subview is inside a tab that got deselected, hide the subwindow */
        sgui_widget_set_visible( super, SGUI_INVISIBLE );
        break;
    case SGUI_TAB_SELECTED:
        /* subview is inside a tab that got selected, show the subwindow */
        sgui_widget_set_visible( super, SGUI_VISIBLE );
        break;

    /* events generated by the canvas that may be of interesst */
    case SGUI_MOUSE_ENTER_EVENT:
    case SGUI_MOUSE_LEAVE_EVENT:
    case SGUI_FOCUS_EVENT:
    case SGUI_FOCUS_LOSE_EVENT:
        if( this->window_fun )
            this->window_fun( super, e );
        break;
    }
}

/* processes events from a sub window managed by a subview widget */
static void subview_on_subwindow_event( sgui_subview* this,
                                        const sgui_event* e )
{
    switch( e->type )
    {
    case SGUI_USER_CLOSED_EVENT:    /* sub window -> not possible */
    case SGUI_API_DESTROY_EVENT:    /* caused by sgui_widget_destroy */
        break;
    case SGUI_SIZE_CHANGE_EVENT:
    case SGUI_EXPOSE_EVENT:
        if( this->draw_fun )
        {
            sgui_window_make_current( this->subwnd );
            this->draw_fun( (sgui_widget*)this );
            sgui_window_swap_buffers( this->subwnd );
            sgui_window_make_current( NULL );
        }
    default:
        if( this->window_fun )
            this->window_fun( (sgui_widget*)this, e );
    }
}

/* process state change events of a subview widget */
static void subview_on_state_change( sgui_widget* super, int change )
{
    sgui_subview* this = (sgui_subview*)super;
    unsigned int ww, wh;
    int x, y, visible;

    /* adjust size and position of the subwindow */
    if( change & (WIDGET_POSITION_CHANGED|WIDGET_PARENT_CHANGED) )
    {
        sgui_widget_get_absolute_position( super, &x, &y );
        sgui_widget_get_size( super, &ww, &wh );

        sgui_window_move( this->subwnd, x, y );
        sgui_window_set_size( this->subwnd, ww, wh );
    }

    /* "adjust" visibillity of the window */
    if( change & (WIDGET_VISIBILLITY_CHANGED|WIDGET_PARENT_CHANGED) )
    {
        visible = sgui_widget_is_absolute_visible( super );

        sgui_window_set_visible( this->subwnd, visible );
    }
}

static void subview_destroy( sgui_widget* this )
{
    sgui_window_destroy( ((sgui_subview*)this)->subwnd );
    free( this );
}

/****************************************************************************/

sgui_widget* sgui_subview_create( sgui_window* parent, int x, int y,
                                  unsigned int width, unsigned int height,
                                  int backend, sgui_window_description* cfg )
{
    sgui_window_description desc;
    sgui_subview* this;
    sgui_widget* super;

    /* sanity check */
    if( !parent )
        return NULL;

    /* allocate storage for the view */
    this = malloc( sizeof(sgui_subview) );
    super = (sgui_widget*)this;

    if( !this )
        return NULL;

    /* initialise the base widget */
    sgui_internal_widget_init( super, x, y, width, height );
    super->window_event_callback = subview_on_parent_event;
    super->state_change_callback = subview_on_state_change;
    super->destroy               = subview_destroy;
    super->focus_policy          = SGUI_FOCUS_ACCEPT|SGUI_FOCUS_DROP_ESC|
                                   SGUI_FOCUS_DROP_TAB;
    this->window_fun             = NULL;
    this->draw_fun               = NULL;

    /* initialise window configuration if not given*/
    if( !cfg )
    {
        desc.parent         = parent;
        desc.share          = NULL;
        desc.width          = width;
        desc.height         = height;
        desc.resizeable     = SGUI_FIXED_SIZE;
        desc.doublebuffer   = SGUI_SINGLEBUFFERED;
        desc.bits_per_pixel = 32;
        desc.depth_bits     = 16;
        desc.stencil_bits   = 0;
        desc.samples        = 0;

        cfg = &desc;
    }

    cfg->backend = backend;

    /* create the subwindow */
    this->subwnd = sgui_window_create_desc( cfg );

    if( !this->subwnd )
    {
        free( this );
        return NULL;
    }

    /* register callbacks */
    sgui_window_on_event( this->subwnd,
                          (sgui_window_callback)subview_on_subwindow_event );
    sgui_window_set_userptr( this->subwnd, this );

    return super;
}

sgui_window* sgui_subview_get_window( sgui_widget* this )
{
    return this ? ((sgui_subview*)this)->subwnd : NULL;
}

void sgui_subview_set_draw_callback( sgui_widget* this,
                                     sgui_subview_draw_fun drawcb )
{
    if( this )
    {
        ((sgui_subview*)this)->draw_fun = drawcb;
    }
}

void sgui_subview_on_window_event( sgui_widget* this,
                                   sgui_subview_window_fun windowcb )
{
    if( this )
    {
        ((sgui_subview*)this)->window_fun = windowcb;
    }
}

void sgui_subview_refresh( sgui_widget* this )
{
    if( this )
    {
        sgui_rect r;

        sgui_rect_set_size( &r, 0, 0, SGUI_RECT_WIDTH(this->area),
                                      SGUI_RECT_HEIGHT(this->area) );

        sgui_window_force_redraw( ((sgui_subview*)this)->subwnd, &r );
    }
}


/*
 * window.c
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
#include "platform.h"

#include "sgui_config.h"
#include "sgui_event.h"
#include "sgui_context.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static void xlib_window_size_hints(Window wnd, unsigned int w, unsigned int h)
{
    XSizeHints hints;

    memset( &hints, 0, sizeof(hints) );
    hints.flags = PSize | PMinSize | PMaxSize;
    hints.min_width = hints.base_width = hints.max_width = (int)w;
    hints.min_height = hints.base_height = hints.max_height = (int)h;

    XSetWMNormalHints( x11.dpy, wnd, &hints );
}

static void xlib_window_get_mouse_position(sgui_window* this, int* x, int* y)
{
    Window t1, t2;   /* values we are not interested */
    int t3, t4;      /* into but xlib does not accept */
    unsigned int t5; /* a NULL pointer for these */

    sgui_internal_lock_mutex( );
    XQueryPointer(x11.dpy, TO_X11(this)->wnd, &t1, &t2, &t3, &t4, x, y, &t5);
    sgui_internal_unlock_mutex( );
}

static void xlib_window_set_mouse_position( sgui_window* this, int x, int y )
{
    sgui_internal_lock_mutex( );
    XWarpPointer(x11.dpy,None,TO_X11(this)->wnd,0,0,this->w,this->h,x,y);
    XFlush( x11.dpy );

    ++(TO_X11(this)->mouse_warped);
    sgui_internal_unlock_mutex( );
}

static void xlib_window_set_visible( sgui_window* this, int visible )
{
    sgui_internal_lock_mutex( );

    if( visible )
        XMapWindow( x11.dpy, TO_X11(this)->wnd );
    else
        XUnmapWindow( x11.dpy, TO_X11(this)->wnd );

    sgui_internal_unlock_mutex( );
}

static void xlib_window_set_title( sgui_window* this, const char* title )
{
    sgui_internal_lock_mutex( );
    XStoreName( x11.dpy, TO_X11(this)->wnd, title );
    sgui_internal_unlock_mutex( );
}

static void xlib_window_set_size( sgui_window* this,
                                  unsigned int width, unsigned int height )
{
    XWindowAttributes attr;

    sgui_internal_lock_mutex( );

    if( this->flags & SGUI_FIXED_SIZE )
        xlib_window_size_hints( TO_X11(this)->wnd, width, height );

    XResizeWindow( x11.dpy, TO_X11(this)->wnd, width, height );
    XFlush( x11.dpy );

    /* get the real geometry as the window manager is free to change it */
    XGetWindowAttributes( x11.dpy, TO_X11(this)->wnd, &attr );
    this->w = (unsigned int)attr.width;
    this->h = (unsigned int)attr.height;

    sgui_internal_unlock_mutex( );
}

static void xlib_window_move_center( sgui_window* this )
{
    sgui_internal_lock_mutex( );
    this->x = (DPY_WIDTH  >> 1) - (int)(this->w >> 1);
    this->y = (DPY_HEIGHT >> 1) - (int)(this->h >> 1);

    XMoveWindow( x11.dpy, TO_X11(this)->wnd, this->x, this->y );
    sgui_internal_unlock_mutex( );
}

static void xlib_window_move( sgui_window* this, int x, int y )
{
    sgui_internal_lock_mutex( );
    XMoveWindow( x11.dpy, TO_X11(this)->wnd, x, y );
    sgui_internal_unlock_mutex( );
}

static void xlib_window_force_redraw( sgui_window* this, sgui_rect* r )
{
    XExposeEvent exp;

    memset( &exp, 0, sizeof(exp) );
    exp.type       = Expose;
    exp.send_event = 1;
    exp.display    = x11.dpy;
    exp.window     = TO_X11(this)->wnd;
    exp.x          = r->left;
    exp.y          = r->top;
    exp.width      = SGUI_RECT_WIDTH_V(r);
    exp.height     = SGUI_RECT_HEIGHT_V(r);

    sgui_internal_lock_mutex( );
    XSendEvent(x11.dpy,TO_X11(this)->wnd,False,ExposureMask,(XEvent*)&exp);
    sgui_internal_unlock_mutex( );
}

static void xlib_window_destroy( sgui_window* this )
{
    sgui_internal_lock_mutex( );
    remove_window( TO_X11(this) );

    if( this->backend==SGUI_NATIVE )
        sgui_canvas_destroy( this->ctx.canvas );
    else if( this->backend!=SGUI_CUSTOM )
        this->ctx.ctx->destroy( this->ctx.ctx );

    XDestroyIC( TO_X11(this)->ic );
    XDestroyWindow( x11.dpy, TO_X11(this)->wnd );
    sgui_internal_unlock_mutex( );

    free( this );
}

static void xlib_window_get_platform_data( const sgui_window* this,
                                           void* window )
{
    *((Window*)window) = TO_X11(this)->wnd;
}

static void xlib_window_make_topmost( sgui_window* this )
{
    sgui_internal_lock_mutex( );
    if( this->flags & SGUI_VISIBLE )
        XRaiseWindow( x11.dpy, TO_X11(this)->wnd );
    sgui_internal_unlock_mutex( );
}

/****************************************************************************/

void handle_window_events( sgui_window_xlib* this, XEvent* e )
{
    sgui_window* super = (sgui_window*)this;
    sgui_event se;
    Status stat;
    KeySym sym;

    se.src.window = super;

    switch( e->type )
    {
    case KeyRelease:
        /*
            Mimic Windows(R) behaviour: Generate a sequence of keydown events
            when a key is held down. X11 generates a sequence of KeyPress and
            KeyRelease events. Remove the additional KeyRelease events.
         */
        if( XPending( e->xkey.display ) > 0 )
        {
            XEvent nev;
            XPeekEvent( e->xkey.display, &nev );
            if( (nev.type == KeyPress) &&
                (nev.xkey.keycode == e->xkey.keycode) &&
                (nev.xkey.time - e->xkey.time) < 2 )
            {
                break;
            }
        }

        sym = XLookupKeysym( &e->xkey, 0 );
        se.arg.i = key_entries_translate( sym );
        se.type = SGUI_KEY_RELEASED_EVENT;
        sgui_internal_window_fire_event( super, &se );
        break;
    case KeyPress:
        memset( se.arg.utf8, 0, sizeof(se.arg.utf8) );

        Xutf8LookupString( this->ic, &e->xkey, (char*)se.arg.utf8,
                           sizeof(se.arg.utf8), &sym, &stat );

        if( (stat==XLookupChars || stat==XLookupBoth) &&
            ((se.arg.utf8[0] & 0x80) || !iscntrl( se.arg.utf8[0] )) )
        {
            se.type = SGUI_CHAR_EVENT;
            sgui_internal_window_fire_event( super, &se );
        }

        if( stat==XLookupKeySym || stat==XLookupBoth )
        {
            se.arg.i = key_entries_translate( sym );
            se.type = SGUI_KEY_PRESSED_EVENT;
            sgui_internal_window_fire_event( super, &se );
        }
        break;
    case ButtonPress:
    case ButtonRelease:
        /* Button4 and Button5 are mouse wheel up and down */
        if( e->xbutton.button==Button4 || e->xbutton.button==Button5 )
        {
            if( e->type==ButtonPress )
            {
                se.type = SGUI_MOUSE_WHEEL_EVENT;
                se.arg.i = (e->xbutton.button==Button4) ? 1 : -1;
                sgui_internal_window_fire_event( super, &se );
            }
        }
        else
        {
            switch( e->xbutton.button )
            {
            case Button1: se.arg.i3.z = SGUI_MOUSE_BUTTON_LEFT;   break;
            case Button2: se.arg.i3.z = SGUI_MOUSE_BUTTON_MIDDLE; break;
            case Button3: se.arg.i3.z = SGUI_MOUSE_BUTTON_RIGHT;  break;
            }

            se.arg.i3.x = e->xbutton.x;
            se.arg.i3.y = e->xbutton.y;
            se.type = e->type==ButtonPress ? SGUI_MOUSE_PRESS_EVENT :
                                             SGUI_MOUSE_RELEASE_EVENT;

            sgui_internal_window_fire_event( super, &se );

            if( e->type==ButtonRelease && e->xbutton.button==Button1 &&
                check_double_click( this ) )
            {
                se.type = SGUI_DOUBLE_CLICK_EVENT;
                sgui_internal_window_fire_event( super, &se );
            }
        }
        break;
    case MotionNotify:
        interrupt_double_click( );

        if( this->mouse_warped )
        {
            --(this->mouse_warped);
        }
        else
        {
            se.arg.i2.x = e->xmotion.x<0 ? 0 : e->xmotion.x;
            se.arg.i2.y = e->xmotion.y<0 ? 0 : e->xmotion.y;
            se.type = SGUI_MOUSE_MOVE_EVENT;
            sgui_internal_window_fire_event( super, &se );
        }
        break;
    case ConfigureNotify:
        se.arg.ui2.x = e->xconfigure.width;
        se.arg.ui2.y = e->xconfigure.height;
        se.type = SGUI_SIZE_CHANGE_EVENT;

        super->x = e->xconfigure.x;
        super->y = e->xconfigure.y;

        if( !se.arg.ui2.x || !se.arg.ui2.y )
            break;

        if( se.arg.ui2.x==super->w && se.arg.ui2.y==super->h )
            break;

        super->w = (unsigned int)e->xconfigure.width;
        super->h = (unsigned int)e->xconfigure.height;

        if( super->backend==SGUI_NATIVE )
            sgui_canvas_resize( super->ctx.canvas, super->w, super->h );

        sgui_internal_window_fire_event( super, &se );
        break;
    case DestroyNotify:
        super->flags &= ~SGUI_VISIBLE;
        this->wnd = 0;
        break;
    case MapNotify:
        if( super->backend!=SGUI_NATIVE )
        {
            sgui_rect_set_size( &se.arg.rect, 0, 0, super->w, super->h );
            se.type = SGUI_EXPOSE_EVENT;
            sgui_internal_window_fire_event( super, &se );
        }
        break;
    case UnmapNotify:
        if( e->xunmap.window==this->wnd && (super->flags & IS_CHILD) )
        {
            se.type = SGUI_USER_CLOSED_EVENT;
            super->flags &= ~SGUI_VISIBLE;
            sgui_internal_window_fire_event( super, &se );
        }
        break;
    case ClientMessage:
        if( e->xclient.data.l[0] == (long)x11.atom_wm_delete )
        {
            se.type = SGUI_USER_CLOSED_EVENT;
            super->flags &= ~SGUI_VISIBLE;
            XUnmapWindow( x11.dpy, this->wnd );
            XUnmapSubwindows( x11.dpy, this->wnd );
            sgui_internal_window_fire_event( super, &se );
        }
        break;
    case Expose:
        if( super->backend==SGUI_NATIVE )
        {
            sgui_rect r;
            sgui_rect_set_size( &r, e->xexpose.x, e->xexpose.y,
                                    e->xexpose.width, e->xexpose.height );
            sgui_canvas_redraw_area( super->ctx.canvas, &r, 1 );
        }
        else
        {
            se.type = SGUI_EXPOSE_EVENT;
            sgui_rect_set_size( &se.arg.rect, 0, 0, super->w, super->h );
            sgui_internal_window_fire_event( super, &se );
        }
        break;
    case FocusIn:
        se.type = SGUI_FOCUS_EVENT;
        sgui_internal_window_fire_event( super, &se );
        break;
    case FocusOut:
        se.type = SGUI_FOCUS_LOSE_EVENT;
        sgui_internal_window_fire_event( super, &se );
        break;
    };
}

/****************************************************************************/

sgui_window* sgui_window_create_desc( const sgui_window_description* desc )
{
    Window x_parent = desc->parent ? TO_X11(desc->parent)->wnd : x11.root;
    sgui_window_xlib* this;
    sgui_window* super;
    XWindowAttributes attr;
    unsigned long color = 0;
    unsigned char rgb[3];

    if( !desc->width || !desc->height )
        return NULL;

    if( desc->flags & (~SGUI_ALL_WINDOW_FLAGS) )
        return NULL;

    this = calloc( 1, sizeof(sgui_window_xlib) );
    super = (sgui_window*)this;

    if( !this )
        return NULL;

    sgui_internal_lock_mutex( );

    /******************** create the window ********************/
    if( desc->backend==SGUI_OPENGL_CORE || desc->backend==SGUI_OPENGL_COMPAT )
    {
        this->wnd = create_glx_window( super, desc, x_parent );
    }
    else if( desc->backend==SGUI_NATIVE || desc->backend==SGUI_CUSTOM )
    {
        memcpy( rgb, sgui_skin_get( )->window_color, 3 );

        color = (rgb[0] << 16) | (rgb[1] << 8) | (rgb[2]);

        this->wnd = XCreateSimpleWindow( x11.dpy, x_parent, 0, 0,
                                         desc->width, desc->height,
                                         0, 0, color );
    }

    if( !this->wnd )
        goto fail;

    if( desc->flags & SGUI_FIXED_SIZE )
        xlib_window_size_hints( this->wnd, desc->width, desc->height );

    /* Get the actual geometry after the window manager changed it */
    XGetWindowAttributes( x11.dpy, this->wnd, &attr );

    /* tell X11 what events we will handle */
    XSelectInput( x11.dpy, this->wnd, X11_EVENT_MASK );
    XSetWMProtocols( x11.dpy, this->wnd, &x11.atom_wm_delete, 1 );
    XFlush( x11.dpy );

    /*********** Create an input context ************/
    this->ic = XCreateIC( x11.im, XNInputStyle,
                          XIMPreeditNothing|XIMStatusNothing, XNClientWindow,
                          this->wnd, XNFocusWindow, this->wnd, NULL );

    if( !this->ic )
        goto failic;

    /********************** create canvas **********************/
    if( desc->backend==SGUI_NATIVE )
    {
        super->ctx.canvas = canvas_x11_create( this->wnd,
                                               attr.width, attr.height, 1 );
    }
    else if(desc->backend==SGUI_OPENGL_CORE||desc->backend==SGUI_OPENGL_COMPAT)
    {
        super->ctx.ctx = gl_context_create(super,desc->backend,desc->share);
    }

    if( !super->ctx.canvas && !super->ctx.ctx && desc->backend!=SGUI_CUSTOM )
        goto failcv;

    if( desc->flags & SGUI_VISIBLE )
        XMapWindow( x11.dpy, this->wnd );

    super->x = attr.x;
    super->y = attr.y;
    super->flags = desc->flags | (desc->parent!=NULL ? IS_CHILD : 0);

    sgui_internal_window_post_init( super, attr.width, attr.height,
                                    desc->backend );

    super->get_mouse_position = xlib_window_get_mouse_position;
    super->set_mouse_position = xlib_window_set_mouse_position;
    super->set_visible        = xlib_window_set_visible;
    super->set_title          = xlib_window_set_title;
    super->set_size           = xlib_window_set_size;
    super->move_center        = xlib_window_move_center;
    super->move               = xlib_window_move;
    super->force_redraw       = xlib_window_force_redraw;
    super->get_platform_data  = xlib_window_get_platform_data;
    super->write_clipboard    = xlib_window_clipboard_write;
    super->read_clipboard     = xlib_window_clipboard_read;
    super->make_topmost       = xlib_window_make_topmost;
    super->destroy            = xlib_window_destroy;

    add_window( this );
    sgui_internal_unlock_mutex( );
    return (sgui_window*)this;
    /* failure cleanup */
failcv: XDestroyIC( this->ic );
failic: XDestroyWindow( x11.dpy, this->wnd );
fail:   free( this );
    sgui_internal_unlock_mutex( );
    return NULL;
}


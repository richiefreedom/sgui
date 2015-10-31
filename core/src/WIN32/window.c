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
#include "sgui_event.h"



static void resize_pixmap( sgui_window_w32* this )
{
    sgui_window* super = (sgui_window*)this;

    /* adjust size in the header */
    this->info.bmiHeader.biWidth  = super->w;
    this->info.bmiHeader.biHeight = -((int)super->h);

    /* unbind the the dib section and delete it */
    SelectObject( this->hDC, 0 );
    DeleteObject( this->bitmap );

    /* create a new dib section */
    this->bitmap = CreateDIBSection( this->hDC, &this->info, DIB_RGB_COLORS,
                                     &this->data, 0, 0 );

    /* bind it */
    SelectObject( this->hDC, this->bitmap );

    /* tell the memory canvas about the new data pointer */
    sgui_memory_canvas_set_buffer( super->ctx.canvas, TO_W32(this)->data );
}

/****************************************************************************/

static void w32_window_get_mouse_position( sgui_window* this, int* x, int* y )
{
    POINT pos = { 0, 0 };

    sgui_internal_lock_mutex( );
    GetCursorPos( &pos );
    ScreenToClient( TO_W32(this)->hWnd, &pos );
    sgui_internal_unlock_mutex( );

    *x = pos.x;
    *y = pos.y;
}

static void w32_window_set_mouse_position( sgui_window* this, int x, int y )
{
    POINT pos;

    pos.x = x;
    pos.y = y;

    sgui_internal_lock_mutex( );
    ClientToScreen( TO_W32(this)->hWnd, &pos );
    SetCursorPos( pos.x, pos.y );
    sgui_internal_unlock_mutex( );
}

static void w32_window_set_visible( sgui_window* this, int visible )
{
    sgui_internal_lock_mutex( );
    ShowWindow( TO_W32(this)->hWnd, visible ? SW_SHOWNORMAL : SW_HIDE );
    sgui_internal_unlock_mutex( );
}

static void w32_window_set_title( sgui_window* this, const char* title )
{
    WCHAR* utf16 = NULL;
    int isascii = 1;
    unsigned int i;

    /* check if the title contains non-ascii characters */
    for( i=0; isascii && title[i]; ++i )
    {
        if( title[i] & 0x80 )
            isascii = 0;
    }

    /* if so, try to convert to utf16 */
    if( !isascii )
    {
        utf16 = utf8_to_utf16( title, -1 );

        if( !utf16 )
            return;
    }

    /* change title */
    sgui_internal_lock_mutex( );

    if( isascii )
    {
        SetWindowTextA( TO_W32(this)->hWnd, title );
    }
    else
    {
        SetWindowTextW( TO_W32(this)->hWnd, utf16 );
    }

    sgui_internal_unlock_mutex( );

    free( utf16 );
}

static void w32_window_set_size( sgui_window* this,
                                 unsigned int width, unsigned int height )
{
    RECT rcClient, rcWindow;
    POINT ptDiff;

    sgui_internal_lock_mutex( );

    /* Determine the actual window size for the given client size */
    GetClientRect( TO_W32(this)->hWnd, &rcClient );
    GetWindowRect( TO_W32(this)->hWnd, &rcWindow );

    ptDiff.x = (rcWindow.right  - rcWindow.left) - rcClient.right;
    ptDiff.y = (rcWindow.bottom - rcWindow.top ) - rcClient.bottom;

    MoveWindow( TO_W32(this)->hWnd, rcWindow.left, rcWindow.top,
                (int)width + ptDiff.x, (int)height + ptDiff.y, TRUE );

    this->w = width;
    this->h = height;

    /* resize the canvas pixmap */
    if( this->backend == SGUI_NATIVE )
    {
        resize_pixmap( TO_W32(this) );
        sgui_canvas_resize( this->ctx.canvas, width, height );
    }
#ifndef SGUI_NO_D3D11
    else if( this->backend == SGUI_DIRECT3D_11 )
    {
        d3d11_resize( this->ctx.ctx );
    }
#endif

    sgui_internal_unlock_mutex( );
}

static void w32_window_move_center( sgui_window* this )
{
    RECT desktop, window;
    int w, h, dw, dh;

    sgui_internal_lock_mutex( );

    GetClientRect( GetDesktopWindow( ), &desktop );
    GetWindowRect( TO_W32(this)->hWnd, &window );

    w = window.right  - window.left;
    h = window.bottom - window.top;

    dw = desktop.right  - desktop.left;
    dh = desktop.bottom - desktop.top;

    MoveWindow( TO_W32(this)->hWnd, (dw-w)/2, (dh-h)/2, w, h, TRUE );

    sgui_internal_unlock_mutex( );
}

static void w32_window_move( sgui_window* this, int x, int y )
{
    RECT r;

    sgui_internal_lock_mutex( );

    GetWindowRect( TO_W32(this)->hWnd, &r );

    MoveWindow( TO_W32(this)->hWnd, x, y, r.right - r.left,
                r.bottom - r.top, TRUE );

    sgui_internal_unlock_mutex( );
}

static void w32_window_force_redraw( sgui_window* this, sgui_rect* r )
{
    RECT r0;

    sgui_internal_lock_mutex( );
    SetRect( &r0, r->left, r->top, r->right+1, r->bottom+1 );
    InvalidateRect( TO_W32(this)->hWnd, &r0, TRUE );
    sgui_internal_unlock_mutex( );
}

static void w32_window_get_platform_data( const sgui_window* this,
                                          void* window )
{
    *((HWND*)window) = TO_W32(this)->hWnd;
}

static void w32_window_destroy( sgui_window* this )
{
    MSG msg;

    sgui_internal_lock_mutex( );

    if( this->backend==SGUI_NATIVE && this->ctx.canvas )
    {
        sgui_canvas_destroy( this->ctx.canvas );

        SelectObject( TO_W32(this)->hDC, 0 );

        if( TO_W32(this)->bitmap )
            DeleteObject( TO_W32(this)->bitmap );
    }
    else if( this->backend!=SGUI_NATIVE && this->ctx.ctx )
    {
        this->ctx.ctx->destroy( this->ctx.ctx );
    }

    if( TO_W32(this)->hDC )
        DeleteDC( TO_W32(this)->hDC );

    if( TO_W32(this)->hWnd )
    {
        this->ctx.canvas = NULL; /* XXX: DestroyWindow calls message proc */
        this->ctx.ctx    = NULL;

        DestroyWindow( TO_W32(this)->hWnd );
        PeekMessageA( &msg, TO_W32(this)->hWnd, WM_QUIT, WM_QUIT, PM_REMOVE );
    }

    DeleteObject( TO_W32(this)->bgbrush );

    remove_window( (sgui_window_w32*)this );
    sgui_internal_unlock_mutex( );

    free( this );
}

/****************************************************************************/

void update_window( sgui_window_w32* this )
{
    sgui_window* super = (sgui_window*)this;
    unsigned int i, num;
    sgui_rect sr;
    RECT r;

    if( super->backend == SGUI_NATIVE )
    {
        num = sgui_canvas_num_dirty_rects( super->ctx.canvas );

        for( i=0; i<num; ++i )
        {
            sgui_canvas_get_dirty_rect( super->ctx.canvas, &sr, i );

            SetRect( &r, sr.left, sr.top, sr.right+1, sr.bottom+1 );
            InvalidateRect( this->hWnd, &r, TRUE );
        }

        sgui_canvas_redraw_widgets( super->ctx.canvas, 1 );
    }
#ifndef SGUI_NO_D3D9
    else if( super->backend == SGUI_DIRECT3D_9 )
    {
        IDirect3DDevice9* dev = ((sgui_d3d9_context*)super->ctx.ctx)->device;
        sgui_event e;

        if( IDirect3DDevice9_TestCooperativeLevel( dev )==D3DERR_DEVICELOST )
        {
            e.type       = SGUI_D3D9_DEVICE_LOST;
            e.src.window = (sgui_window*)this;
            sgui_internal_window_fire_event( super, &e );
        }
    }
#endif
}

int handle_window_events( sgui_window_w32* this, UINT msg, WPARAM wp,
                          LPARAM lp )
{
    sgui_window* super = (sgui_window*)this;
    BLENDFUNCTION ftn;
    RECT r;
    sgui_event e;
    PAINTSTRUCT ps;
    HDC hDC;
    WCHAR c[2];
    UINT key;

    e.src.window = super;

    switch( msg )
    {
    case WM_SETFOCUS:       e.type = SGUI_FOCUS_EVENT;        goto send_ev;
    case WM_KILLFOCUS:      e.type = SGUI_FOCUS_LOSE_EVENT;   goto send_ev;
    case WM_LBUTTONDBLCLK:  e.type = SGUI_DOUBLE_CLICK_EVENT; goto event_xy;
    case WM_MOUSEMOVE:      e.type = SGUI_MOUSE_MOVE_EVENT;   goto event_xy;
    case WM_DESTROY:
        super->visible = 0;
        e.type = SGUI_USER_CLOSED_EVENT;
        goto send_ev;
    case WM_MOUSEWHEEL:
        e.arg.i = GET_WHEEL_DELTA_WPARAM( wp ) / WHEEL_DELTA;
        e.type = SGUI_MOUSE_WHEEL_EVENT;
        goto send_ev;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        e.arg.i3.z = msg==WM_LBUTTONDOWN ? SGUI_MOUSE_BUTTON_LEFT : 
                     (msg==WM_RBUTTONDOWN ? SGUI_MOUSE_BUTTON_RIGHT :
                      SGUI_MOUSE_BUTTON_MIDDLE);
        e.type = SGUI_MOUSE_PRESS_EVENT;
        goto event_xy;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        e.arg.i3.z = msg==WM_LBUTTONUP ? SGUI_MOUSE_BUTTON_LEFT : 
                     (msg==WM_RBUTTONUP ? SGUI_MOUSE_BUTTON_RIGHT :
                      SGUI_MOUSE_BUTTON_MIDDLE);
        e.type = SGUI_MOUSE_RELEASE_EVENT;
        goto event_xy;
    case WM_CHAR:
        c[0] = (WCHAR)wp;
        c[1] = '\0';

        if( (c[0] < 0x80) && iscntrl( c[0] ) )
            break;

        WideCharToMultiByte( CP_UTF8, 0, c, 2, e.arg.utf8, 8, NULL, NULL );
        e.type = SGUI_CHAR_EVENT;
        goto send_ev;
    case WM_SYSKEYUP:
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    case WM_KEYUP:
        key = (UINT)wp;

        if( key==VK_SHIFT || key==VK_CONTROL || key==VK_MENU )
            key = MapVirtualKey( (lp>>16) & 0xFF, MAPVK_VSC_TO_VK_EX );

        if( (lp & 0x1000000) && (key==VK_CONTROL) )
            key = VK_RCONTROL;

        if( (lp & 0x1000000) && (key==VK_MENU) )
            key = VK_RMENU;

        /* Send event */
        if( msg==WM_KEYDOWN || msg==WM_SYSKEYDOWN )
            e.type = SGUI_KEY_PRESSED_EVENT;
        else
            e.type = SGUI_KEY_RELEASED_EVENT;

        e.arg.i = key;
        sgui_internal_window_fire_event( super, &e );

        /* let DefWindowProc handle system keys, except ALT */
        if( (msg==WM_SYSKEYUP || msg==WM_SYSKEYDOWN) &&
            !(key==VK_MENU || key==VK_LMENU || key==VK_RMENU) )
        {
            return DefWindowProcA( this->hWnd, msg, wp, lp );
        }
        break;
    case WM_SIZE:
        e.arg.ui2.x = super->w = LOWORD( lp );
        e.arg.ui2.y = super->h = HIWORD( lp );
        e.type = SGUI_SIZE_CHANGE_EVENT;

        /* resize canvas and redraw everything */
        if( super->backend==SGUI_NATIVE )
        {
            resize_pixmap( this );
            sgui_canvas_resize( super->ctx.canvas, super->w, super->h );
        }
#ifndef SGUI_NO_D3D11
        else if( super->backend==SGUI_DIRECT3D_11 )
        {
            d3d11_resize( super->ctx.ctx );
        }
#endif
        sgui_internal_window_fire_event( super, &e );

        if( super->backend==SGUI_NATIVE )
            sgui_canvas_draw_widgets( super->ctx.canvas, 1 );
        break;
    case WM_MOVE:
        super->x = LOWORD( lp );
        super->y = HIWORD( lp );
        break;
    case WM_PAINT:                        /* XXX: falls through to default */
        if( super->backend==SGUI_NATIVE )
        {
            ftn.BlendOp = AC_SRC_OVER;
            ftn.BlendFlags = 0;
            ftn.SourceConstantAlpha = 0xFF;
            ftn.AlphaFormat = AC_SRC_ALPHA;

            SetRect( &r, 0, 0, super->w, super->h );

            hDC = BeginPaint( this->hWnd, &ps );
            FillRect( hDC, &r, this->bgbrush );
            AlphaBlend( hDC, 0, 0, super->w, super->h, this->hDC,
                        0, 0, super->w, super->h, ftn );
            EndPaint( this->hWnd, &ps );
        }
        else
        {
            e.type = SGUI_EXPOSE_EVENT;
            sgui_rect_set_size( &e.arg.rect, 0, 0, super->w, super->h );
            sgui_internal_window_fire_event( super, &e );
        }
    default:
        return DefWindowProcA( this->hWnd, msg, wp, lp );
    }

    return 0;
event_xy:
    e.arg.i3.x = LOWORD( lp );
    e.arg.i3.y = HIWORD( lp );
send_ev:
    sgui_internal_window_fire_event( super, &e );
    return 0;
}

/****************************************************************************/

sgui_window* sgui_window_create_desc( const sgui_window_description* desc )
{
    sgui_window_w32* this;
    sgui_window* super;
    unsigned char color[4];
    HWND parent_hnd = 0;
    DWORD style;
    RECT r;

    if( !desc || !desc->width || !desc->height || (desc->flags&(~ALL_FLAGS)) )
        return NULL;

#ifdef SGUI_NO_OPENGL
    if( desc->backend==SGUI_OPENGL_CORE || desc->backend==SGUI_OPENGL_COMPAT )
        return NULL;
#endif

#ifdef SGUI_NO_D3D9
    if( desc->backend==SGUI_DIRECT3D_9 )
        return NULL;
#endif

#ifdef SGUI_NO_D3D11
    if( desc->backend==SGUI_DIRECT3D_11 )
        return NULL;
#endif

    /*************** allocate space for the window structure ***************/
    this = calloc( 1, sizeof(sgui_window_w32) );
    super = (sgui_window*)this;

    if( !this )
        return NULL;

    sgui_internal_lock_mutex( );
    add_window( this );

    /*************************** create a window ***************************/
    SetRect( &r, 0, 0, desc->width, desc->height );

    if( desc->parent )
    {
        parent_hnd = TO_W32(desc->parent)->hWnd;
        style = WS_CHILD;
    }
    else
    {
        style = (desc->flags & SGUI_FIXED_SIZE) ? (WS_CAPTION | WS_SYSMENU) :
                                                  WS_OVERLAPPEDWINDOW;
        AdjustWindowRect( &r, style, FALSE );
    }

    style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    this->hWnd = CreateWindowExA(0, w32.wndclass, "", style, 0, 0,
                                 r.right-r.left, r.bottom-r.top,
                                 parent_hnd, 0, w32.hInstance, 0);

    if( !this->hWnd )
        goto failure;

    SET_USER_PTR( this->hWnd, this );

    /**************************** create canvas ****************************/
    if( desc->backend==SGUI_NATIVE )
    {
        /* create an offscreen Device Context */
        if( !(this->hDC = CreateCompatibleDC( NULL )) )
            goto failure;

        /*fill the bitmap header */
        this->info.bmiHeader.biSize        = sizeof(this->info.bmiHeader);
        this->info.bmiHeader.biBitCount    = 32;
        this->info.bmiHeader.biCompression = BI_RGB;
        this->info.bmiHeader.biPlanes      = 1;
        this->info.bmiHeader.biWidth       = desc->width;
        this->info.bmiHeader.biHeight      = -((int)desc->height);

        /* create a DIB section = bitmap with accessable data pointer */
        this->bitmap = CreateDIBSection( this->hDC, &this->info,
                                         DIB_RGB_COLORS, &this->data, 0, 0 );

        if( !this->bitmap )
            goto failure;

        /* bind the dib section to the offscreen context */
        SelectObject( this->hDC, this->bitmap );

        super->ctx.canvas = sgui_memory_canvas_create(this->data, desc->width,
                                                      desc->height,
                                                      SGUI_RGBA8, 1 );

        if( !super->ctx.canvas )
            goto failure;
    }
#ifndef SGUI_NO_OPENGL
    else if(desc->backend==SGUI_OPENGL_CORE||desc->backend==SGUI_OPENGL_COMPAT)
    {
        if( !(this->hDC = GetDC( this->hWnd )) )
            goto failure;

        if( !set_pixel_format( this, desc ) )
            goto failure;

        super->backend = desc->backend;
        super->ctx.ctx = gl_context_create( super,
                                            desc->backend==SGUI_OPENGL_CORE,
                                            (sgui_gl_context*)desc->share );

        if( !super->ctx.ctx )
            goto failure;

        super->swap_buffers = gl_swap_buffers;
        super->set_vsync = gl_set_vsync;
    }
#endif
#ifndef SGUI_NO_D3D9
    else if( desc->backend==SGUI_DIRECT3D_9 )
    {
        super->backend = desc->backend;
        super->ctx.ctx = d3d9_context_create(super, desc,
                                             (sgui_d3d9_context*)desc->share);

        if( !super->ctx.ctx )
            goto failure;

        super->swap_buffers = d3d9_swap_buffers;
        super->set_vsync = d3d9_set_vsync;
    }
#endif
#ifndef SGUI_NO_D3D11
    else if( desc->backend==SGUI_DIRECT3D_11 )
    {
        super->backend = desc->backend;
        super->ctx.ctx = d3d11_context_create( super, desc );

        if( !super->ctx.ctx )
            goto failure;

        super->swap_buffers = d3d11_swap_buffers;
        super->set_vsync = d3d11_set_vsync;
    }
#endif
    sgui_internal_window_post_init( (sgui_window*)this,
                                     desc->width, desc->height,
                                     desc->backend );

    memcpy( color, sgui_skin_get( )->window_color, 3 );
    this->bgbrush = CreateSolidBrush( RGB(color[0],color[1],color[2]) );

    /* store entry points */
    super->get_mouse_position = w32_window_get_mouse_position;
    super->set_mouse_position = w32_window_set_mouse_position;
    super->set_visible        = w32_window_set_visible;
    super->set_title          = w32_window_set_title;
    super->set_size           = w32_window_set_size;
    super->move_center        = w32_window_move_center;
    super->move               = w32_window_move;
    super->force_redraw       = w32_window_force_redraw;
    super->get_platform_data  = w32_window_get_platform_data;
    super->destroy            = w32_window_destroy;
    super->write_clipboard    = w32_window_write_clipboard;
    super->read_clipboard     = w32_window_read_clipboard;

    sgui_internal_unlock_mutex( );
    return (sgui_window*)this;
failure:
    sgui_internal_unlock_mutex( );
    w32_window_destroy( (sgui_window*)this );
    return NULL;
}


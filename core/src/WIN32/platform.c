/*
 * platform.c
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
#include "sgui.h"
#include "internal.h"


static sgui_window_w32* list = NULL;
static sgui_pixmap* skin_pixmap = NULL;

FT_Library freetype;
HINSTANCE hInstance;
const char* wndclass = "sgui_wnd_class";





static LRESULT CALLBACK WindowProcFun( HWND hWnd, UINT msg, WPARAM wp,
                                       LPARAM lp )
{
    sgui_window_w32* wnd;
    int result = -1;

    /* get window pointer and redirect */
    wnd = (sgui_window_w32*)GET_USER_PTR( hWnd );

    if( wnd )
        result = handle_window_events( wnd, msg, wp, lp );

    /* return result, call default window proc if result < 0 */
    return result < 0 ? DefWindowProc( hWnd, msg, wp, lp ) : result;
}




void add_window( sgui_window_w32* wnd )
{
    SGUI_ADD_TO_LIST( list, wnd );
}

void remove_window( sgui_window_w32* wnd )
{
    sgui_window_w32* i;

    SGUI_REMOVE_FROM_LIST( list, i, wnd );
}

sgui_pixmap* get_skin_pixmap( void )
{
    unsigned int width, height;

    if( !skin_pixmap )
    {
        sgui_skin_get_pixmap_size( &width, &height );

        skin_pixmap = sgui_internal_mem_pixmap_create( width, height,
                                                       SGUI_RGBA8, 1 );

        if( skin_pixmap )
            sgui_skin_to_pixmap( skin_pixmap );
    }

    return skin_pixmap;
}

/****************************************************************************/

int sgui_init( void )
{
    WNDCLASSEX wc;

    /* initialise freetype library */
    if( FT_Init_FreeType( &freetype ) )
    {
        sgui_deinit( );
        return 0;
    }

    /* get hInstance */
    hInstance = GetModuleHandle( NULL );

    if( !hInstance )
    {
        sgui_deinit( );
        return 0;
    }

    /* Register window class */
    memset( &wc, 0, sizeof(WNDCLASSEX) );

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProcFun;
    wc.hInstance     = hInstance;
    wc.lpszClassName = wndclass;
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );

    if( RegisterClassEx( &wc ) == 0 )
    {
        sgui_deinit( );
        return 0;
    }

    /* initialise default GUI skin */
    sgui_skin_set( NULL );

    return 1;
}

void sgui_deinit( void )
{
    /* destroy skin pixmap */
    if( skin_pixmap )
        sgui_pixmap_destroy( skin_pixmap );

    sgui_skin_unload( );

    /* unregister window class */
    UnregisterClass( wndclass, hInstance );

    if( freetype )
        FT_Done_FreeType( freetype );

    /* reset values */
    skin_pixmap = NULL;
    freetype = 0;
    hInstance = 0;
    list = NULL;
}

int sgui_main_loop_step( void )
{
    sgui_window_w32* i;
    MSG msg;

    /* handle a message if there is one */
    if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    /* check if there's at least 1 window still active */
    for( i=list; i!=NULL; i=i->next )
        if( i->super.visible )
            return 1;

    return 0;
}

void sgui_main_loop( void )
{
    sgui_window_w32* i;
    int active;
    MSG msg;

    do
    {
        /* handle message */
        GetMessage( &msg, 0, 0, 0 );
        TranslateMessage( &msg );
        DispatchMessage( &msg );

        /* check if there's at least 1 window still active */
        for( i=list, active=0; i!=NULL && !active; i=i->next )
            active |= i->super.visible;
    }
    while( active );
}


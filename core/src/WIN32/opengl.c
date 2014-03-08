/*
 * opengl.c
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
#include "sgui_event.h"
#include "internal.h"

#ifndef SGUI_NO_OPENGL
#include <GL/gl.h>
#include "opengl.h"




static int glversions[][2] = { {4,4}, {4,3}, {4,2}, {4,1}, {4,0},
                                      {3,3}, {3,2}, {3,1}, {3,0} };


/* transform a pixel attribute array into a pixelformat descriptor */
static int get_descriptor_from_array( PIXELFORMATDESCRIPTOR* pfd,
                                      int* pixel_attribs )
{
    int i, need_new = 0;

#define ATTRIB( field ) (field) = pixel_attribs[ ++i ]
#define FLAG( flag )\
        pfd->dwFlags=pixel_attribs[++i] ? (pfd->dwFlags | (flag)) :\
                                          (pfd->dwFlags & ~(flag))

    ZeroMemory( pfd, sizeof(PIXELFORMATDESCRIPTOR) );
    pfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd->nVersion = 1;

    for( i=0; pixel_attribs[i]; ++i )
    {
        switch( pixel_attribs[i] )
        {
        case WGL_DRAW_TO_WINDOW_ARB:   FLAG( PFD_DRAW_TO_WINDOW ); break;
        case WGL_DRAW_TO_BITMAP_ARB:   FLAG( PFD_DRAW_TO_BITMAP ); break;
        case WGL_SUPPORT_OPENGL_ARB:   FLAG( PFD_SUPPORT_OPENGL ); break;
        case WGL_DOUBLE_BUFFER_ARB:    FLAG( PFD_DOUBLEBUFFER   ); break;
        case WGL_SUPPORT_GDI_ARB:      FLAG( PFD_SUPPORT_GDI    ); break;
        case WGL_STEREO_ARB:           FLAG( PFD_STEREO         ); break;
        case WGL_COLOR_BITS_ARB:       ATTRIB( pfd->cColorBits      ); break;
        case WGL_DEPTH_BITS_ARB:       ATTRIB( pfd->cDepthBits      ); break;
        case WGL_STENCIL_BITS_ARB:     ATTRIB( pfd->cStencilBits    ); break;
        case WGL_RED_BITS_ARB:         ATTRIB( pfd->cRedBits        ); break;
        case WGL_RED_SHIFT_ARB:        ATTRIB( pfd->cRedShift       ); break;
        case WGL_GREEN_BITS_ARB:       ATTRIB( pfd->cGreenBits      ); break;
        case WGL_GREEN_SHIFT_ARB:      ATTRIB( pfd->cGreenShift     ); break;
        case WGL_BLUE_BITS_ARB:        ATTRIB( pfd->cBlueBits       ); break;
        case WGL_BLUE_SHIFT_ARB:       ATTRIB( pfd->cBlueShift      ); break;
        case WGL_ALPHA_BITS_ARB:       ATTRIB( pfd->cAlphaBits      ); break;
        case WGL_ALPHA_SHIFT_ARB:      ATTRIB( pfd->cAlphaShift     ); break;
        case WGL_ACCUM_BITS_ARB:       ATTRIB( pfd->cAccumBits      ); break;
        case WGL_ACCUM_RED_BITS_ARB:   ATTRIB( pfd->cAccumRedBits   ); break;
        case WGL_ACCUM_GREEN_BITS_ARB: ATTRIB( pfd->cAccumGreenBits ); break;
        case WGL_ACCUM_BLUE_BITS_ARB:  ATTRIB( pfd->cAccumBlueBits  ); break;
        case WGL_ACCUM_ALPHA_BITS_ARB: ATTRIB( pfd->cAccumAlphaBits ); break;
        case WGL_AUX_BUFFERS_ARB:      ATTRIB( pfd->cAuxBuffers     ); break;
        case WGL_PIXEL_TYPE_ARB:
            pfd->iPixelType = pixel_attribs[ ++i ]==WGL_TYPE_RGBA_ARB ?
                              PFD_TYPE_RGBA : PFD_TYPE_COLORINDEX;
            break;
        default:
            ++i;
            need_new = 1;
        };
    }
#undef FLAG
#undef ATTRIB
    return need_new;
}

/*
    On Windows, the ChosePixelFormat / SetPixelFormat functions were used to
    get a pixel format for OpenGL rendering. The first function gets a pointer
    to a structure describing the format und returns a unique ID of the best
    matching format that then gets passed into the later function.

    When multisampling and other things were added, it turned out that this
    was not expandable. So a new function was introduced as an extension that
    accepts an array of key value pairs. So now if we want pixel format
    extensions like multisampling, we have to set up a dummy OpenGL context
    to get the extension, determine the unique identifier, and use the
    identifier in the actual window setup process. Since SetPixelFormat is
    only allowed ONCE per window, we even have to create a dummy window for
    the whole process.

    This is basically what this function does. It takes a new style pixel
    format attribute array as input, derives an old style structure from it,
    determines the pixel format, checks if it can determine the pixel format
    from the actual array, do that, return the identifier.
 */
static int determine_pixel_format( int* pixel_attribs, int only_new )
{
    WGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
    int pixelformat, format, need_new = 0;
    PIXELFORMATDESCRIPTOR pfd;
    HGLRC temprc, oldctx;
    HDC tempdc, olddc;
    UINT numFormats;
    HWND tempwnd;

    /* Generate an old style pixel format descriptor */
    need_new = get_descriptor_from_array( &pfd, pixel_attribs );

    /* Create a dummy window */
    tempwnd = CreateWindow(wndclass,"",0,0,0,100,100,0,0,hInstance,0);
    tempdc = GetWindowDC( tempwnd );
    pixelformat = ChoosePixelFormat( tempdc, &pfd );
    SetPixelFormat( tempdc, pixelformat, NULL );

    /* try to get a pixel format descriptor from the array if required */
    if( need_new )
    {
        /* Create a dummy OpenGL context */
        oldctx = wglGetCurrentContext( );
        olddc = wglGetCurrentDC( );
        temprc = wglCreateContext( tempdc );
        wglMakeCurrent( tempdc, temprc );

        /* Try to use the extension to determine the format from the array */
        wglChoosePixelFormatARB = (WGLCHOOSEPIXELFORMATARBPROC)
        wglGetProcAddress( "wglChoosePixelFormatARB" );

        pixelformat = only_new ? 0 : pixelformat;

        if( wglChoosePixelFormatARB )
        {
            wglChoosePixelFormatARB( tempdc, pixel_attribs, NULL,
                                     1, &format, &numFormats );

            pixelformat = numFormats>=1 ? format : pixelformat;
        }

        /* reset and destroy dummy context */
        wglMakeCurrent( olddc, oldctx );
        wglDeleteContext( temprc );
    }

    /* Clean up */
    ReleaseDC( tempwnd, tempdc );
    DestroyWindow( tempwnd );

    return pixelformat;
}

static void set_attributes( int* attr, int bpp, int depth, int stencil,
                            int doublebuffer, int samples )
{
    int i=0;

    bpp = (bpp!=16 && bpp!=24 && bpp!=32) ? 32 : bpp;
    depth = (depth!=0 && depth!=16 && depth!=24 && depth!=32) ? 24 : depth;
    stencil = (stencil!=0 && stencil!=8) ? 8 : stencil;

#define ATTRIB( name, value ) attr[ i++ ] = (name); attr[ i++ ] = (value)
    ATTRIB( WGL_DRAW_TO_WINDOW_ARB, GL_TRUE         );
    ATTRIB( WGL_SUPPORT_OPENGL_ARB, GL_TRUE         );
    ATTRIB( WGL_COLOR_BITS_ARB,     bpp             );
    ATTRIB( WGL_RED_BITS_ARB,       bpp==16 ? 5 : 8 );
    ATTRIB( WGL_GREEN_BITS_ARB,     bpp==16 ? 6 : 8 );
    ATTRIB( WGL_BLUE_BITS_ARB,      bpp==16 ? 5 : 8 );

    if( bpp==32      ) { ATTRIB( WGL_ALPHA_BITS_ARB,    8       ); }
    if( depth        ) { ATTRIB( WGL_DEPTH_BITS_ARB,    depth   ); }
    if( stencil      ) { ATTRIB( WGL_STENCIL_BITS_ARB,  stencil ); }
    if( doublebuffer ) { ATTRIB( WGL_DOUBLE_BUFFER_ARB, GL_TRUE ); }

    if( samples )
    {
        ATTRIB( WGL_SAMPLE_BUFFERS_ARB, GL_TRUE );
        ATTRIB( WGL_SAMPLES_ARB,        samples );
    }

    ATTRIB( 0, 0 );
#undef ATTRIB
}

int set_pixel_format( sgui_window_w32* this,
                      const sgui_window_description* desc )
{
    int attribs[20], samples, format;

    samples = desc->samples;

    do
    {
        set_attributes( attribs, desc->bits_per_pixel, desc->depth_bits,
                        desc->stencil_bits, desc->doublebuffer, samples-- );

        format = determine_pixel_format( attribs, 1 );
    }
    while( samples>=0 && format<1 );

    if( !format )
        format = determine_pixel_format( attribs, 0 );

    if( !format )
        return 0;

    SetPixelFormat( this->hDC, format, NULL );
    return 1;
}

/****************************************************************************/

struct sgui_gl_context
{
    HGLRC hRC;
};

sgui_gl_context* sgui_gl_context_create( sgui_window* wnd,
                                         sgui_gl_context* share,
                                         int core )
{
    WGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
    sgui_gl_context* this;
    HGLRC temp, oldctx;
    int attribs[20];
    unsigned int i;
    HDC olddc;
    HGLRC src;

    this = malloc( sizeof(sgui_gl_context) );

    if( !this )
        return NULL;

    sgui_internal_lock_mutex( );
    this->hRC = 0;
    src = share ? share->hRC : 0;

    /********** create an old fashioned OpenGL temporary context **********/
    if( !(temp = wglCreateContext( TO_W32(wnd)->hDC )) )
    {
        free( this );
        sgui_internal_unlock_mutex( );
        return NULL;
    }

    if( !core )
    {
        this->hRC = temp;
        if( src )
            wglShareLists( src, this->hRC );
        sgui_internal_unlock_mutex( );
        return this;
    }

    /********** try to create a new context **********/
    /* make the temporary context current */
    oldctx = wglGetCurrentContext( );
    olddc = wglGetCurrentDC( );
    wglMakeCurrent( TO_W32(wnd)->hDC, temp );

    /* try to load the context creation funciont */
    wglCreateContextAttribsARB = (WGLCREATECONTEXTATTRIBSARBPROC)
    wglGetProcAddress( "wglCreateContextAttribsARB" );

    if( !wglCreateContextAttribsARB )
    {
        wglMakeCurrent( olddc, oldctx );
        this->hRC = temp;
        sgui_internal_unlock_mutex( );
        return this;
    }

    /* fill attrib array */
    attribs[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
    attribs[1] = 0;
    attribs[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
    attribs[3] = 0;
    attribs[4] = WGL_CONTEXT_PROFILE_MASK_ARB;
    attribs[5] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
    attribs[6] = WGL_CONTEXT_FLAGS_ARB;
    attribs[7] = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
    attribs[8] = 0;

    /* try to create 4.3 down to 3.0 context */
    for( i=0; !this->hRC && i<sizeof(glversions)/sizeof(glversions[0]); ++i )
    {
        attribs[1] = glversions[i][0];
        attribs[3] = glversions[i][1];
        this->hRC = wglCreateContextAttribsARB(TO_W32(wnd)->hDC,src,attribs);
    }

    /* restore the privous context */
    wglMakeCurrent( olddc, oldctx );

    if( this->hRC )
    {
        wglDeleteContext( temp );
    }
    else
    {
        this->hRC = temp;

        if( src )
            wglShareLists( src, this->hRC );
    }

    sgui_internal_unlock_mutex( );
    return this;
}

void sgui_gl_context_destroy( sgui_gl_context* this )
{
    if( this && this->hRC )
    {
        if( wglGetCurrentContext( )==this->hRC )
            wglMakeCurrent( NULL, NULL );

        wglDeleteContext( this->hRC );
    }
}

void sgui_gl_context_make_current( sgui_gl_context* this, sgui_window* wnd )
{
    if( this &&
        (wnd->backend==SGUI_OPENGL_CORE || wnd->backend==SGUI_OPENGL_COMPAT) )
    {
        wglMakeCurrent( TO_W32(wnd)->hDC, this->hRC );
    }
    else
    {
        wglMakeCurrent( NULL, NULL );
    }
}

sgui_funptr sgui_gl_context_load( sgui_gl_context* ctx, const char* name )
{
    (void)ctx;
    return (sgui_funptr)wglGetProcAddress( name );
}

void gl_swap_buffers( sgui_window* this )
{
    glFlush( );
    SwapBuffers( ((sgui_window_w32*)this)->hDC );
}

void gl_set_vsync( sgui_window* this, int interval )
{
    WGLSWAPINTERVALEXT wglSwapIntervalEXT;
    (void)this;

    sgui_internal_lock_mutex( );

    wglSwapIntervalEXT = (WGLSWAPINTERVALEXT)
                         wglGetProcAddress( "wglSwapIntervalEXT" );

    if( wglSwapIntervalEXT )
        wglSwapIntervalEXT( interval );

    sgui_internal_unlock_mutex( );
}
#else
sgui_gl_context* sgui_gl_context_create( sgui_window* wnd,
                                         sgui_gl_context* share, int core )
{
    (void)wnd; (void)share; (void)core;
    return NULL;
}

void sgui_gl_context_destroy( sgui_gl_context* this )
{
    (void)this;
}

void sgui_gl_context_make_current( sgui_gl_context* this, sgui_window* wnd )
{
    (void)this; (void)wnd;
}

sgui_funptr sgui_gl_context_load( sgui_gl_context* ctx, const char* name )
{
    (void)ctx; (void)name;
    return NULL;
}
#endif


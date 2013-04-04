/*
 * pixmap.c
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
#include "internal.h"



struct sgui_pixmap
{
    unsigned int width, height;
    int format, backend;

    union
    {
        struct
        {
            HDC hDC;
            HBITMAP bitmap;
            unsigned char* ptr;
        }
        native;

#ifndef SGUI_NO_OPENGL
        GLuint opengl;
#endif
    }
    pm;
};



sgui_pixmap* sgui_pixmap_create( unsigned int width, unsigned int height,
                                 int format, int backend )
{
    sgui_pixmap* pix;

    if( !width || !height || !format )
        return NULL;

    pix = malloc( sizeof(sgui_pixmap) );

    if( !pix )
        return NULL;

    pix->width   = width;
    pix->height  = height;
    pix->format  = format;
    pix->backend = backend;

    if( backend==SGUI_OPENGL_CORE || backend==SGUI_OPENGL_COMPAT )
    {
#ifndef SGUI_NO_OPENGL
        GLint current;

        glGenTextures( 1, &pix->pm.opengl );

        if( !pix->pm.opengl )
        {
            free( pix );
            pix = NULL;
        }
        else
        {
            /* get current texture and bind ours */
            glGetIntegerv( GL_TEXTURE_BINDING_2D, &current );
            glBindTexture( GL_TEXTURE_2D, pix->pm.opengl );

            /* reserve texture memory */
            if( format==SGUI_RGBA8 )
            {
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
                              0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
            }
            else
            {
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0,
                              GL_RGB, GL_UNSIGNED_BYTE, NULL );
            }

            /* disable mipmapping */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            /* rebind old texture */
            glBindTexture( GL_TEXTURE_2D, current );
        }
#endif
    }
    else
    {
        BITMAPINFO info;

        info.bmiHeader.biSize        = sizeof(info.bmiHeader);
        info.bmiHeader.biBitCount    = 32;
        info.bmiHeader.biCompression = BI_RGB;
        info.bmiHeader.biPlanes      = 1;
        info.bmiHeader.biWidth       = width;
        info.bmiHeader.biHeight      = -((int)height);

        pix->pm.native.hDC = CreateCompatibleDC( NULL );

        if( !pix->pm.native.hDC )
        {
            free( pix );
            return NULL;
        }

        pix->pm.native.bitmap = CreateDIBSection( pix->pm.native.hDC, &info,
                                                  DIB_RGB_COLORS,
                                                  (void**)&pix->pm.native.ptr,
                                                  0, 0 );

        if( !pix->pm.native.bitmap )
        {
            DeleteDC( pix->pm.native.hDC );
            free( pix );
            return NULL;
        }

        SelectObject( pix->pm.native.hDC, pix->pm.native.bitmap );
    }

    return pix;
}

void sgui_pixmap_load( sgui_pixmap* pixmap, sgui_rect* dstrect,
                       const unsigned char* data, int srcx, int srcy,
                       unsigned int width, unsigned int height,
                       int format )
{
    int dstx, dsty, subw, subh;

    if( !pixmap || !data || !width || !height )
        return;

    dstx = dstrect ? dstrect->left : 0;
    dsty = dstrect ? dstrect->top  : 0;
    subw = dstrect ? SGUI_RECT_WIDTH_V( dstrect ) : (int)width;
    subh = dstrect ? SGUI_RECT_HEIGHT_V( dstrect ) : (int)height;

    data += (srcy*width + srcx)*(format==SGUI_RGB8 ? 3 : 4);

    if( pixmap->backend==SGUI_OPENGL_CORE ||
        pixmap->backend==SGUI_OPENGL_COMPAT )
    {
#ifndef SGUI_NO_OPENGL
        GLint current;

        glGetIntegerv( GL_TEXTURE_BINDING_2D, &current );
        glBindTexture( GL_TEXTURE_2D, pixmap->pm.opengl );

        glPixelStorei( GL_UNPACK_ROW_LENGTH, width );

        glTexSubImage2D( GL_TEXTURE_2D, 0, dstx, dsty, subw, subh,
                         format==SGUI_RGB8 ? GL_RGB : GL_RGBA,
                         GL_UNSIGNED_BYTE, data );

        glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );

        glBindTexture( GL_TEXTURE_2D, current );
#endif
    }
    else
    {
        unsigned char* dst;
        const unsigned char *src, *row;
        int i, j, bpp = format==SGUI_RGB8 ? 3 : 4, alpha, dstbpp;

        dstbpp = pixmap->format==SGUI_RGBA8 ? 4 : 3;
        dst = pixmap->pm.native.ptr + (dstx + dsty*pixmap->width)*dstbpp;

        for( src=data, j=0; j<subh; ++j, src+=width*bpp )
        {
            for( row=src, i=0; i<subw; ++i, row+=bpp )
            {
                alpha = bpp==4 ? row[3] : 0xFF;

                *(dst++) = row[2]*alpha >> 8;
                *(dst++) = row[1]*alpha >> 8;
                *(dst++) = row[0]*alpha >> 8;

                if( pixmap->format==SGUI_RGBA8 )
                    *(dst++) = alpha;
            }
        }
    }
}

void sgui_pixmap_get_size( sgui_pixmap* pixmap, unsigned int* width,
                           unsigned int* height )
{
    if( pixmap )
    {
        if( width  ) *width  = pixmap->width;
        if( height ) *height = pixmap->height;
    }
    else
    {
        if( width  ) *width  = 0;
        if( height ) *height = 0;
    }
}

void sgui_pixmap_destroy( sgui_pixmap* pixmap )
{
    if( !pixmap )
        return;

    if( pixmap->backend==SGUI_OPENGL_CORE ||
        pixmap->backend==SGUI_OPENGL_COMPAT )
    {
#ifndef SGUI_NO_OPENGL
        glDeleteTextures( 1, &pixmap->pm.opengl );
#endif
    }
    else
    {
        DeleteObject( pixmap->pm.native.bitmap );
        DeleteDC( pixmap->pm.native.hDC );
    }

    free( pixmap );
}

/****************************************************************************/

void pixmap_blit( sgui_pixmap* pixmap, HDC hDC, int x, int y,
                  int srcx, int srcy,
                  unsigned int width, unsigned int height )
{
    if( pixmap && pixmap->backend==SGUI_NATIVE )
    {
        BitBlt( hDC, x, y, width, height, pixmap->pm.native.hDC,
                srcx, srcy, SRCCOPY );
    }
}

void pixmap_blend( sgui_pixmap* pixmap, HDC hDC, int x, int y,
                   int srcx, int srcy,
                   unsigned int width, unsigned int height )
{
    BLENDFUNCTION ftn;

    if( pixmap && pixmap->backend==SGUI_NATIVE )
    {
        ftn.BlendOp             = AC_SRC_OVER;
        ftn.BlendFlags          = 0;
        ftn.SourceConstantAlpha = 0xFF;
        ftn.AlphaFormat         = AC_SRC_ALPHA;

        AlphaBlend( hDC, x, y, width, height, pixmap->pm.native.hDC,
                    srcx, srcy, width, height, ftn );
    }
}


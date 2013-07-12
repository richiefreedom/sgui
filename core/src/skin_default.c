/*
 * skin_default.c
 * This file is part of sgui
 *
 * Copyright (C) 2012 - David Oberhollenzer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions
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
#include "sgui_skin.h"
#include "sgui_pixmap.h"



#define SET_ELEMENT(skin,id,x,y,w,h)\
        sgui_rect_set_size((skin)->elements+(id),x,y,w,h)



static void default_skin_to_pixmap( sgui_pixmap* pixmap )
{
    unsigned char buffer[ 20*20*4 ];
    unsigned int x, y;

    if( !pixmap )
        return;

    /* draw unchecked checkbox */
    for( y=0; y<12; ++y )
    {
        for( x=0; x<12; ++x )
        {
            buffer[ (y*12 + x)*4     ] = 0x00;
            buffer[ (y*12 + x)*4 + 1 ] = 0x00;
            buffer[ (y*12 + x)*4 + 2 ] = 0x00;
            buffer[ (y*12 + x)*4 + 3 ] = 0x80;
        }
    }

    for( x=0; x<12; ++x )
    {
        buffer[ x*12*4     ] = buffer[ x*4     ] = 0x00;
        buffer[ x*12*4 + 1 ] = buffer[ x*4 + 1 ] = 0x00;
        buffer[ x*12*4 + 2 ] = buffer[ x*4 + 2 ] = 0x00;
        buffer[ x*12*4 + 3 ] = buffer[ x*4 + 3 ] = 0xFF;
    }

    for( x=0; x<12; ++x )
        for( y=0; y<4; ++y )
            buffer[ (x*12 + 11)*4 + y ] = buffer[ (11*12 + x)*4 + y ] = 0xFF;

    sgui_pixmap_load( pixmap, 0, 0, buffer, 0, 0, 12, 12, 12, SGUI_RGBA8 );

    /* draw checked checkbox */
    for( y=0; y<3; ++y )
    {
        for( x=0; x<4; ++x )
        {
            buffer[ ((4+y)*12 + 2)*4 + x ] = 0xFF;
            buffer[ ((5+y)*12 + 3)*4 + x ] = 0xFF;
            buffer[ ((6+y)*12 + 4)*4 + x ] = 0xFF;
            buffer[ ((5+y)*12 + 5)*4 + x ] = 0xFF;
            buffer[ ((4+y)*12 + 6)*4 + x ] = 0xFF;
            buffer[ ((3+y)*12 + 7)*4 + x ] = 0xFF;
            buffer[ ((2+y)*12 + 8)*4 + x ] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 12, 0, buffer, 0, 0, 12, 12, 12, SGUI_RGBA8 );

    /* draw radio button */
    for( y=0; y<12; ++y )
    {
        for( x=0; x<12; ++x )
        {
            buffer[ (y*12 + x)*4     ] = buffer[ (y*12 + x)*4 + 1 ] = 0x00;
            buffer[ (y*12 + x)*4 + 2 ] = buffer[ (y*12 + x)*4 + 3 ] = 0x00;
        }
    }

    for( y=0; y<8; ++y )
    {
        for( x=0; x<8; ++x )
        {
            buffer[ ((y+2)*12 + x+2)*4     ] = 0x00;
            buffer[ ((y+2)*12 + x+2)*4 + 1 ] = 0x00;
            buffer[ ((y+2)*12 + x+2)*4 + 2 ] = 0x00;
            buffer[ ((y+2)*12 + x+2)*4 + 3 ] = 0x80;
        }
    }

    for( x=0; x<4; ++x )
    {
        buffer[ ((x+4)*12)*4     ] = buffer[ (x+4)*4     ] = 0x00;
        buffer[ ((x+4)*12)*4 + 1 ] = buffer[ (x+4)*4 + 1 ] = 0x00;
        buffer[ ((x+4)*12)*4 + 2 ] = buffer[ (x+4)*4 + 2 ] = 0x00;
        buffer[ ((x+4)*12)*4 + 3 ] = buffer[ (x+4)*4 + 3 ] = 0xFF;

        buffer[ ((x+4)*12 + 11)*4     ] = buffer[ (11*12 + x+4)*4     ]=0xFF;
        buffer[ ((x+4)*12 + 11)*4 + 1 ] = buffer[ (11*12 + x+4)*4 + 1 ]=0xFF;
        buffer[ ((x+4)*12 + 11)*4 + 2 ] = buffer[ (11*12 + x+4)*4 + 2 ]=0xFF;
        buffer[ ((x+4)*12 + 11)*4 + 3 ] = buffer[ (11*12 + x+4)*4 + 3 ]=0xFF;

        buffer[ ((x+4)*12+1)*4     ] = buffer[ (12+x+4)*4     ] = 0x00;
        buffer[ ((x+4)*12+1)*4 + 1 ] = buffer[ (12+x+4)*4 + 1 ] = 0x00;
        buffer[ ((x+4)*12+1)*4 + 2 ] = buffer[ (12+x+4)*4 + 2 ] = 0x00;
        buffer[ ((x+4)*12+1)*4 + 3 ] = buffer[ (12+x+4)*4 + 3 ] = 0x80;

        buffer[ ((x+4)*12 + 10)*4     ] = buffer[ (10*12 + x+4)*4     ]=0x00;
        buffer[ ((x+4)*12 + 10)*4 + 1 ] = buffer[ (10*12 + x+4)*4 + 1 ]=0x00;
        buffer[ ((x+4)*12 + 10)*4 + 2 ] = buffer[ (10*12 + x+4)*4 + 2 ]=0x00;
        buffer[ ((x+4)*12 + 10)*4 + 3 ] = buffer[ (10*12 + x+4)*4 + 3 ]=0x80;
    }

    for( x=0; x<2; ++x )
    {
        buffer[ (1*12 + x+2)*4     ] = buffer[ (1*12 + x+8)*4     ] = 0x00;
        buffer[ (1*12 + x+2)*4 + 1 ] = buffer[ (1*12 + x+8)*4 + 1 ] = 0x00;
        buffer[ (1*12 + x+2)*4 + 2 ] = buffer[ (1*12 + x+8)*4 + 2 ] = 0x00;
        buffer[ (1*12 + x+2)*4 + 3 ] = buffer[ (1*12 + x+8)*4 + 3 ] = 0xFF;

        buffer[ ((x+2)*12 + 1)*4     ] = buffer[ ((x+8)*12 + 1)*4     ]=0x00;
        buffer[ ((x+2)*12 + 1)*4 + 1 ] = buffer[ ((x+8)*12 + 1)*4 + 1 ]=0x00;
        buffer[ ((x+2)*12 + 1)*4 + 2 ] = buffer[ ((x+8)*12 + 1)*4 + 2 ]=0x00;
        buffer[ ((x+2)*12 + 1)*4 + 3 ] = buffer[ ((x+8)*12 + 1)*4 + 3 ]=0xFF;

        buffer[ (10*12 + x+2)*4     ] = buffer[ (10*12 + x+8)*4     ] = 0xFF;
        buffer[ (10*12 + x+2)*4 + 1 ] = buffer[ (10*12 + x+8)*4 + 1 ] = 0xFF;
        buffer[ (10*12 + x+2)*4 + 2 ] = buffer[ (10*12 + x+8)*4 + 2 ] = 0xFF;
        buffer[ (10*12 + x+2)*4 + 3 ] = buffer[ (10*12 + x+8)*4 + 3 ] = 0xFF;

        buffer[ ((x+2)*12 + 10)*4     ]=buffer[ ((x+8)*12 + 10)*4     ]=0xFF;
        buffer[ ((x+2)*12 + 10)*4 + 1 ]=buffer[ ((x+8)*12 + 10)*4 + 1 ]=0xFF;
        buffer[ ((x+2)*12 + 10)*4 + 2 ]=buffer[ ((x+8)*12 + 10)*4 + 2 ]=0xFF;
        buffer[ ((x+2)*12 + 10)*4 + 3 ]=buffer[ ((x+8)*12 + 10)*4 + 3 ]=0xFF;
    }

    sgui_pixmap_load( pixmap, 24, 0, buffer, 0, 0, 12, 12, 12, SGUI_RGBA8 );

    /* draw selected radio button */
    for( y=0; y<6; ++y )
    {
        for( x=0; x<4; ++x )
        {
            buffer[ ((y+3)*12 + x+4)*4     ] = 0xFF;
            buffer[ ((y+3)*12 + x+4)*4 + 1 ] = 0xFF;
            buffer[ ((y+3)*12 + x+4)*4 + 2 ] = 0xFF;
            buffer[ ((y+3)*12 + x+4)*4 + 3 ] = 0xFF;
        }
    }

    for( y=0; y<4; ++y )
    {
        for( x=0; x<6; ++x )
        {
            buffer[ ((y+4)*12 + x+3)*4     ] = 0xFF;
            buffer[ ((y+4)*12 + x+3)*4 + 1 ] = 0xFF;
            buffer[ ((y+4)*12 + x+3)*4 + 2 ] = 0xFF;
            buffer[ ((y+4)*12 + x+3)*4 + 3 ] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 36, 0, buffer, 0, 0, 12, 12, 12, SGUI_RGBA8 );

    /* scroll bar down button */
    for( y=0; y<20; ++y )
    {
        for( x=0; x<20; ++x )
        {
            buffer[ (y*20 + x)*4     ] = 0x64;
            buffer[ (y*20 + x)*4 + 1 ] = 0x64;
            buffer[ (y*20 + x)*4 + 2 ] = 0x64;
            buffer[ (y*20 + x)*4 + 3 ] = 0xFF;
        }
    }

    for( x=0; x<20; ++x )
    {
        buffer[ x*4     ] = buffer[ x*20*4     ] = 0xFF;
        buffer[ x*4 + 1 ] = buffer[ x*20*4 + 1 ] = 0xFF;
        buffer[ x*4 + 2 ] = buffer[ x*20*4 + 2 ] = 0xFF;
        buffer[ x*4 + 3 ] = buffer[ x*20*4 + 3 ] = 0xFF;
    }

    for( x=0; x<20; ++x )
    {
        buffer[ (19*20+x)*4     ] = buffer[ (x*20+19)*4     ] = 0x00;
        buffer[ (19*20+x)*4 + 1 ] = buffer[ (x*20+19)*4 + 1 ] = 0x00;
        buffer[ (19*20+x)*4 + 2 ] = buffer[ (x*20+19)*4 + 2 ] = 0x00;
        buffer[ (19*20+x)*4 + 3 ] = buffer[ (x*20+19)*4 + 3 ] = 0xFF;
    }

    for( y=0; y<6; ++y )
    {
        for( x=0; x<(11-2*y); ++x )
        {
            buffer[ ((7+y)*20 + 5+y+x)*4     ] = 0xFF;
            buffer[ ((7+y)*20 + 5+y+x)*4 + 1 ] = 0xFF;
            buffer[ ((7+y)*20 + 5+y+x)*4 + 2 ] = 0xFF;
            buffer[ ((7+y)*20 + 5+y+x)*4 + 3 ] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 48, 0, buffer, 0, 0, 20, 20, 20, SGUI_RGBA8 );

    /* scroll bar down button pressed */
    for( y=0; y<20; ++y )
    {
        for( x=0; x<20; ++x )
        {
            buffer[ (y*20 + x)*4     ] = 0x64;
            buffer[ (y*20 + x)*4 + 1 ] = 0x64;
            buffer[ (y*20 + x)*4 + 2 ] = 0x64;
            buffer[ (y*20 + x)*4 + 3 ] = 0xFF;
        }
    }

    for( x=0; x<20; ++x )
    {
        buffer[ x*4     ] = buffer[ x*20*4     ] = 0x00;
        buffer[ x*4 + 1 ] = buffer[ x*20*4 + 1 ] = 0x00;
        buffer[ x*4 + 2 ] = buffer[ x*20*4 + 2 ] = 0x00;
        buffer[ x*4 + 3 ] = buffer[ x*20*4 + 3 ] = 0xFF;
    }

    for( x=0; x<20; ++x )
    {
        buffer[ (19*20+x)*4     ] = buffer[ (x*20+19)*4     ] = 0xFF;
        buffer[ (19*20+x)*4 + 1 ] = buffer[ (x*20+19)*4 + 1 ] = 0xFF;
        buffer[ (19*20+x)*4 + 2 ] = buffer[ (x*20+19)*4 + 2 ] = 0xFF;
        buffer[ (19*20+x)*4 + 3 ] = buffer[ (x*20+19)*4 + 3 ] = 0xFF;
    }

    for( y=0; y<6; ++y )
    {
        for( x=0; x<(11-2*y); ++x )
        {
            buffer[ ((7+y-1)*20 + 5+y+x-1)*4     ] = 0xFF;
            buffer[ ((7+y-1)*20 + 5+y+x-1)*4 + 1 ] = 0xFF;
            buffer[ ((7+y-1)*20 + 5+y+x-1)*4 + 2 ] = 0xFF;
            buffer[ ((7+y-1)*20 + 5+y+x-1)*4 + 3 ] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 68, 0, buffer, 0, 0, 20, 20, 20, SGUI_RGBA8 );

    /* scroll bar up button */
    for( y=0; y<20; ++y )
    {
        for( x=0; x<20; ++x )
        {
            buffer[ (y*20 + x)*4     ] = 0x64;
            buffer[ (y*20 + x)*4 + 1 ] = 0x64;
            buffer[ (y*20 + x)*4 + 2 ] = 0x64;
            buffer[ (y*20 + x)*4 + 3 ] = 0xFF;
        }
    }

    for( x=0; x<20; ++x )
    {
        buffer[ x*4     ] = buffer[ x*20*4     ] = 0xFF;
        buffer[ x*4 + 1 ] = buffer[ x*20*4 + 1 ] = 0xFF;
        buffer[ x*4 + 2 ] = buffer[ x*20*4 + 2 ] = 0xFF;
        buffer[ x*4 + 3 ] = buffer[ x*20*4 + 3 ] = 0xFF;
    }

    for( x=0; x<20; ++x )
    {
        buffer[ (19*20+x)*4     ] = buffer[ (x*20+19)*4     ] = 0x00;
        buffer[ (19*20+x)*4 + 1 ] = buffer[ (x*20+19)*4 + 1 ] = 0x00;
        buffer[ (19*20+x)*4 + 2 ] = buffer[ (x*20+19)*4 + 2 ] = 0x00;
        buffer[ (19*20+x)*4 + 3 ] = buffer[ (x*20+19)*4 + 3 ] = 0xFF;
    }

    for( y=0; y<6; ++y )
    {
        for( x=0; x<(11-2*y); ++x )
        {
            buffer[ ((12-y)*20 + 5+y+x)*4     ] = 0xFF;
            buffer[ ((12-y)*20 + 5+y+x)*4 + 1 ] = 0xFF;
            buffer[ ((12-y)*20 + 5+y+x)*4 + 2 ] = 0xFF;
            buffer[ ((12-y)*20 + 5+y+x)*4 + 3 ] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 88, 0, buffer, 0, 0, 20, 20, 20, SGUI_RGBA8 );

    /* scroll bar up button pressed */
    for( y=0; y<20; ++y )
    {
        for( x=0; x<20; ++x )
        {
            buffer[ (y*20 + x)*4     ] = 0x64;
            buffer[ (y*20 + x)*4 + 1 ] = 0x64;
            buffer[ (y*20 + x)*4 + 2 ] = 0x64;
            buffer[ (y*20 + x)*4 + 3 ] = 0xFF;
        }
    }

    for( x=0; x<20; ++x )
    {
        buffer[ x*4     ] = buffer[ x*20*4     ] = 0x00;
        buffer[ x*4 + 1 ] = buffer[ x*20*4 + 1 ] = 0x00;
        buffer[ x*4 + 2 ] = buffer[ x*20*4 + 2 ] = 0x00;
        buffer[ x*4 + 3 ] = buffer[ x*20*4 + 3 ] = 0xFF;
    }

    for( x=0; x<20; ++x )
    {
        buffer[ (19*20+x)*4     ] = buffer[ (x*20+19)*4     ] = 0xFF;
        buffer[ (19*20+x)*4 + 1 ] = buffer[ (x*20+19)*4 + 1 ] = 0xFF;
        buffer[ (19*20+x)*4 + 2 ] = buffer[ (x*20+19)*4 + 2 ] = 0xFF;
        buffer[ (19*20+x)*4 + 3 ] = buffer[ (x*20+19)*4 + 3 ] = 0xFF;
    }

    for( y=0; y<6; ++y )
    {
        for( x=0; x<(11-2*y); ++x )
        {
            buffer[ ((12-y-1)*20 + 5+y+x-1)*4     ] = 0xFF;
            buffer[ ((12-y-1)*20 + 5+y+x-1)*4 + 1 ] = 0xFF;
            buffer[ ((12-y-1)*20 + 5+y+x-1)*4 + 2 ] = 0xFF;
            buffer[ ((12-y-1)*20 + 5+y+x-1)*4 + 3 ] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 108, 0, buffer, 0, 0, 20, 20, 20, SGUI_RGBA8 );

    /* scroll bar left button */
    for( y=0; y<20; ++y )
    {
        for( x=0; x<20; ++x )
        {
            buffer[ (y*20 + x)*4     ] = 0x64;
            buffer[ (y*20 + x)*4 + 1 ] = 0x64;
            buffer[ (y*20 + x)*4 + 2 ] = 0x64;
            buffer[ (y*20 + x)*4 + 3 ] = 0xFF;
        }
    }

    for( x=0; x<20; ++x )
    {
        buffer[ x*4     ] = buffer[ x*20*4     ] = 0xFF;
        buffer[ x*4 + 1 ] = buffer[ x*20*4 + 1 ] = 0xFF;
        buffer[ x*4 + 2 ] = buffer[ x*20*4 + 2 ] = 0xFF;
        buffer[ x*4 + 3 ] = buffer[ x*20*4 + 3 ] = 0xFF;
    }

    for( x=0; x<20; ++x )
    {
        buffer[ (19*20+x)*4     ] = buffer[ (x*20+19)*4     ] = 0x00;
        buffer[ (19*20+x)*4 + 1 ] = buffer[ (x*20+19)*4 + 1 ] = 0x00;
        buffer[ (19*20+x)*4 + 2 ] = buffer[ (x*20+19)*4 + 2 ] = 0x00;
        buffer[ (19*20+x)*4 + 3 ] = buffer[ (x*20+19)*4 + 3 ] = 0xFF;
    }

    for( x=0; x<6; ++x )
    {
        for( y=0; y<(11-2*x); ++y )
        {
            buffer[ ((5+y+x)*20 + 12-x)*4     ] = 0xFF;
            buffer[ ((5+y+x)*20 + 12-x)*4 + 1 ] = 0xFF;
            buffer[ ((5+y+x)*20 + 12-x)*4 + 2 ] = 0xFF;
            buffer[ ((5+y+x)*20 + 12-x)*4 + 3 ] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 48, 20, buffer, 0, 0, 20, 20, 20, SGUI_RGBA8 );

    /* scroll bar left button pressed */
    for( y=0; y<20; ++y )
    {
        for( x=0; x<20; ++x )
        {
            buffer[ (y*20 + x)*4     ] = 0x64;
            buffer[ (y*20 + x)*4 + 1 ] = 0x64;
            buffer[ (y*20 + x)*4 + 2 ] = 0x64;
            buffer[ (y*20 + x)*4 + 3 ] = 0xFF;
        }
    }

    for( x=0; x<20; ++x )
    {
        buffer[ x*4     ] = buffer[ x*20*4     ] = 0x00;
        buffer[ x*4 + 1 ] = buffer[ x*20*4 + 1 ] = 0x00;
        buffer[ x*4 + 2 ] = buffer[ x*20*4 + 2 ] = 0x00;
        buffer[ x*4 + 3 ] = buffer[ x*20*4 + 3 ] = 0xFF;
    }

    for( x=0; x<20; ++x )
    {
        buffer[ (19*20+x)*4     ] = buffer[ (x*20+19)*4     ] = 0xFF;
        buffer[ (19*20+x)*4 + 1 ] = buffer[ (x*20+19)*4 + 1 ] = 0xFF;
        buffer[ (19*20+x)*4 + 2 ] = buffer[ (x*20+19)*4 + 2 ] = 0xFF;
        buffer[ (19*20+x)*4 + 3 ] = buffer[ (x*20+19)*4 + 3 ] = 0xFF;
    }

    for( x=0; x<6; ++x )
    {
        for( y=0; y<(11-2*x); ++y )
        {
            buffer[ ((5+y+x-1)*20 + 12-x-1)*4     ] = 0xFF;
            buffer[ ((5+y+x-1)*20 + 12-x-1)*4 + 1 ] = 0xFF;
            buffer[ ((5+y+x-1)*20 + 12-x-1)*4 + 2 ] = 0xFF;
            buffer[ ((5+y+x-1)*20 + 12-x-1)*4 + 3 ] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 68, 20, buffer, 0, 0, 20, 20, 20, SGUI_RGBA8 );

    /* scroll bar right button */
    for( y=0; y<20; ++y )
    {
        for( x=0; x<20; ++x )
        {
            buffer[ (y*20 + x)*4     ] = 0x64;
            buffer[ (y*20 + x)*4 + 1 ] = 0x64;
            buffer[ (y*20 + x)*4 + 2 ] = 0x64;
            buffer[ (y*20 + x)*4 + 3 ] = 0xFF;
        }
    }

    for( x=0; x<20; ++x )
    {
        buffer[ x*4     ] = buffer[ x*20*4     ] = 0xFF;
        buffer[ x*4 + 1 ] = buffer[ x*20*4 + 1 ] = 0xFF;
        buffer[ x*4 + 2 ] = buffer[ x*20*4 + 2 ] = 0xFF;
        buffer[ x*4 + 3 ] = buffer[ x*20*4 + 3 ] = 0xFF;
    }

    for( x=0; x<20; ++x )
    {
        buffer[ (19*20+x)*4     ] = buffer[ (x*20+19)*4     ] = 0x00;
        buffer[ (19*20+x)*4 + 1 ] = buffer[ (x*20+19)*4 + 1 ] = 0x00;
        buffer[ (19*20+x)*4 + 2 ] = buffer[ (x*20+19)*4 + 2 ] = 0x00;
        buffer[ (19*20+x)*4 + 3 ] = buffer[ (x*20+19)*4 + 3 ] = 0xFF;
    }

    for( x=0; x<6; ++x )
    {
        for( y=0; y<(11-2*x); ++y )
        {
            buffer[ ((5+y+x)*20 + 7+x)*4     ] = 0xFF;
            buffer[ ((5+y+x)*20 + 7+x)*4 + 1 ] = 0xFF;
            buffer[ ((5+y+x)*20 + 7+x)*4 + 2 ] = 0xFF;
            buffer[ ((5+y+x)*20 + 7+x)*4 + 3 ] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 88, 20, buffer, 0, 0, 20, 20, 20, SGUI_RGBA8 );

    /* scroll bar right button pressed */
    for( y=0; y<20; ++y )
    {
        for( x=0; x<20; ++x )
        {
            buffer[ (y*20 + x)*4     ] = 0x64;
            buffer[ (y*20 + x)*4 + 1 ] = 0x64;
            buffer[ (y*20 + x)*4 + 2 ] = 0x64;
            buffer[ (y*20 + x)*4 + 3 ] = 0xFF;
        }
    }

    for( x=0; x<20; ++x )
    {
        buffer[ x*4     ] = buffer[ x*20*4     ] = 0x00;
        buffer[ x*4 + 1 ] = buffer[ x*20*4 + 1 ] = 0x00;
        buffer[ x*4 + 2 ] = buffer[ x*20*4 + 2 ] = 0x00;
        buffer[ x*4 + 3 ] = buffer[ x*20*4 + 3 ] = 0xFF;
    }

    for( x=0; x<20; ++x )
    {
        buffer[ (19*20+x)*4     ] = buffer[ (x*20+19)*4     ] = 0xFF;
        buffer[ (19*20+x)*4 + 1 ] = buffer[ (x*20+19)*4 + 1 ] = 0xFF;
        buffer[ (19*20+x)*4 + 2 ] = buffer[ (x*20+19)*4 + 2 ] = 0xFF;
        buffer[ (19*20+x)*4 + 3 ] = buffer[ (x*20+19)*4 + 3 ] = 0xFF;
    }

    for( x=0; x<6; ++x )
    {
        for( y=0; y<(11-2*x); ++y )
        {
            buffer[ ((5+y+x-1)*20 + 7+x-1)*4     ] = 0xFF;
            buffer[ ((5+y+x-1)*20 + 7+x-1)*4 + 1 ] = 0xFF;
            buffer[ ((5+y+x-1)*20 + 7+x-1)*4 + 2 ] = 0xFF;
            buffer[ ((5+y+x-1)*20 + 7+x-1)*4 + 3 ] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 108, 20, buffer, 0, 0, 20, 20, 20, SGUI_RGBA8 );

    /* scroll bar background */
    for( x=0; x<20; ++x )
    {
        for( y=0; y<20; ++y )
        {
            buffer[(y*20+x)*4]=buffer[(y*20+x)*4+1]=buffer[(y*20+x)*4+2]=0x64;
            buffer[(y*20+x)*4+3] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 30, 42, buffer, 0, 0, 20, 20, 20, SGUI_RGBA8 );

    /* horizontal progress bar */
    for( y=0; y<29; ++y )
    {
        buffer[ y*4 ] = buffer[ y*4+1 ] = buffer[ y*4+2 ] = 0x00;
        buffer[ y*4+3 ] = 0xFF;
    }

    buffer[ y*4 ] = buffer[ y*4+1 ] = buffer[ y*4+2 ] = 0xFF;
    buffer[ y*4+3 ] = 0xFF;

    sgui_pixmap_load( pixmap, 0, 12, buffer, 0, 0, 1, 30, 1, SGUI_RGBA8 );

    for( x=0; x<12; ++x )
    {
        for( y=0; y<30; ++y )
        {
            buffer[(y*12+x)*4]=buffer[(y*12+x)*4+1]=buffer[(y*12+x)*4+2]=0x00;
            buffer[ (y*12 + x)*4 + 3 ] = 0x80;
        }
    }

    for( x=0; x<12; ++x )
    {
        buffer[ x*4 ] = buffer[ x*4 + 1 ] = buffer[ x*4 + 2 ] = 0x00;
        buffer[ x*4 + 3 ] = 0xFF;
    }

    for( x=0; x<12; ++x )
    {
        buffer[(29*12+x)*4]=buffer[(29*12+x)*4+1]=buffer[(29*12+x)*4+2]=0xFF;
        buffer[(29*12+x)*4+3] = 0xFF;
    }

    sgui_pixmap_load( pixmap, 1, 12, buffer, 0, 0, 12, 30, 12, SGUI_RGBA8 );

    for( x=0; x<7; ++x )
    {
        for( y=0; y<20; ++y )
        {
            buffer[((y+5)*12+x)*4  ] = buffer[((y+5)*12+x)*4+1] = 0xFF;
            buffer[((y+5)*12+x)*4+2] = 0xFF;
            buffer[((y+5)*12+x)*4+3] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 13, 12, buffer, 0, 0, 12, 30, 12, SGUI_RGBA8 );

    for( x=0; x<12; ++x )
    {
        for( y=0; y<28; ++y )
        {
            buffer[((y+1)*12+x)*4]=buffer[((y+1)*12+x)*4+1]=0xFF;
            buffer[((y+1)*12+x)*4+2]=0x00;
            buffer[((y+1)*12+x)*4+3]=0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 26, 12, buffer, 0, 0, 12, 30, 12, SGUI_RGBA8 );

    for( y=0; y<30; ++y )
    {
        buffer[y*4] = buffer[y*4+1] = buffer[y*4+2] = buffer[y*4+3] = 0xFF;
    }

    sgui_pixmap_load( pixmap, 25, 12, buffer, 0, 0, 1, 30, 1, SGUI_RGBA8 );

    /* vertical progress bar */
    for( y=0; y<29; ++y )
    {
        buffer[ y*4 ] = buffer[ y*4+1 ] = buffer[ y*4+2 ] = 0x00;
        buffer[ y*4+3 ] = 0xFF;
    }

    buffer[ y*4 ] = buffer[ y*4+1 ] = buffer[ y*4+2 ] = 0xFF;
    buffer[ y*4+3 ] = 0xFF;

    sgui_pixmap_load( pixmap, 0, 42, buffer, 0, 0, 30, 1, 30, SGUI_RGBA8 );

    for( x=0; x<30; ++x )
    {
        for( y=0; y<12; ++y )
        {
            buffer[(y*30+x)*4]=buffer[(y*30+x)*4+1]=buffer[(y*30+x)*4+2]=0x00;
            buffer[ (y*30 + x)*4 + 3 ] = 0x80;
        }
    }

    for( y=0; y<12; ++y )
    {
        buffer[ y*30*4 ] = buffer[ y*30*4 + 1 ] = buffer[ y*30*4 + 2 ] = 0x00;
        buffer[ y*30*4 + 3 ] = 0xFF;
    }

    for( y=0; y<12; ++y )
    {
        buffer[(y*30+29)*4]=buffer[(y*30+29)*4+1]=buffer[(y*30+29)*4+2]=0xFF;
        buffer[ (y*30+29)*4 + 3 ] = 0xFF;
    }

    sgui_pixmap_load( pixmap, 0, 43, buffer, 0, 0, 30, 12, 30, SGUI_RGBA8 );

    for( y=0; y<7; ++y )
    {
        for( x=0; x<20; ++x )
        {
            buffer[(y*30+x+5)*4  ] = buffer[(y*30+x+5)*4+1] = 0xFF;
            buffer[(y*30+x+5)*4+2] = buffer[(y*30+x+5)*4+3] = 0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 0, 55, buffer, 0, 0, 30, 12, 30, SGUI_RGBA8 );

    for( x=0; x<28; ++x )
    {
        for( y=0; y<12; ++y )
        {
            buffer[(y*30+x+1)*4]=buffer[(y*30+x+1)*4+1]=0xFF;
            buffer[(y*30+x+1)*4+2]=0x00;
            buffer[(y*30+x+1)*4+3]=0xFF;
        }
    }

    sgui_pixmap_load( pixmap, 0, 68, buffer, 0, 0, 30, 12, 30, SGUI_RGBA8 );

    for( x=0; x<30; ++x )
    {
        buffer[x*4] = buffer[x*4+1] = buffer[x*4+2] = buffer[x*4+3] = 0xFF;
    }

    sgui_pixmap_load( pixmap, 0, 67, buffer, 0, 0, 30, 1, 30, SGUI_RGBA8 );

    /* button normal */
    for( x=0; x<200; ++x )
    {
        buffer[x*4] = buffer[x*4+1] = buffer[x*4+2] = buffer[x*4+3] = 0;
    }

    sgui_pixmap_load( pixmap, 38, 12, buffer, 0, 0, 10, 20, 10, SGUI_RGBA8 );

    for( x=0; x<10; ++x )
    {
        buffer[x*4] = buffer[x*4+1] = buffer[x*4+2] = buffer[x*4+3] = 0xFF;
    }

    sgui_pixmap_load( pixmap, 38, 12, buffer, 0, 0, 10, 1, 10, SGUI_RGBA8 );
    sgui_pixmap_load( pixmap, 38, 12, buffer, 0, 0, 1, 10, 1, SGUI_RGBA8 );

    for( x=0; x<10; ++x )
    {
        buffer[x*4] = buffer[x*4+1] = buffer[x*4+2] = 0x00;
        buffer[x*4+3] = 0xFF;
    }

    sgui_pixmap_load( pixmap, 38, 21, buffer, 0, 0, 10, 1, 10, SGUI_RGBA8 );
    sgui_pixmap_load( pixmap, 47, 12, buffer, 0, 0, 1, 10, 1, SGUI_RGBA8 );

    /* button pressed */
    for( x=0; x<10; ++x )
    {
        buffer[x*4] = buffer[x*4+1] = buffer[x*4+2] = 0x00;
        buffer[x*4+3] = 0xFF;
    }

    sgui_pixmap_load( pixmap, 38, 22, buffer, 0, 0, 10, 1, 10, SGUI_RGBA8 );
    sgui_pixmap_load( pixmap, 38, 22, buffer, 0, 0, 1, 10, 1, SGUI_RGBA8 );

    for( x=0; x<10; ++x )
    {
        buffer[x*4] = buffer[x*4+1] = buffer[x*4+2] = buffer[x*4+3] = 0xFF;
    }

    sgui_pixmap_load( pixmap, 38, 31, buffer, 0, 0, 10, 1, 10, SGUI_RGBA8 );
    sgui_pixmap_load( pixmap, 47, 22, buffer, 0, 0, 1, 10, 1, SGUI_RGBA8 );

    /* edit box cursor */
    for( x=0; x<20; ++x )
    {
        buffer[x*4]=buffer[x*4+1]=buffer[x*4+2]=0x7F;
        buffer[x*4+3] = 0xFF;
    }

    sgui_pixmap_load( pixmap, 50, 42, buffer, 0, 0, 1, 20, 1, SGUI_RGBA8 );

    /* group box */
    for( x=0; x<20; ++x )
    {
        buffer[x*4]=buffer[x*4+1]=buffer[x*4+2]=buffer[x*4+3]=0xFF;
    }

    sgui_pixmap_load( pixmap, 31, 63, buffer, 0, 0, 1, 20, 1, SGUI_RGBA8 );
    sgui_pixmap_load( pixmap, 31, 63, buffer, 0, 0, 20, 1, 20, SGUI_RGBA8 );
    sgui_pixmap_load( pixmap, 50, 63, buffer, 0, 0, 1, 20, 1, SGUI_RGBA8 );
    sgui_pixmap_load( pixmap, 31, 82, buffer, 0, 0, 20, 1, 20, SGUI_RGBA8 );

    for( x=0; x<20; ++x )
    {
        buffer[x*4]=buffer[x*4+1]=buffer[x*4+2]=0x00;
        buffer[x*4+3]=0xFF;
    }

    sgui_pixmap_load( pixmap, 30, 62, buffer, 0, 0, 1, 20, 1, SGUI_RGBA8 );
    sgui_pixmap_load( pixmap, 30, 62, buffer, 0, 0, 20, 1, 20, SGUI_RGBA8 );
    sgui_pixmap_load( pixmap, 49, 62, buffer, 0, 0, 1, 20, 1, SGUI_RGBA8 );
    sgui_pixmap_load( pixmap, 30, 81, buffer, 0, 0, 20, 1, 20, SGUI_RGBA8 );

    for( y=0; y<17; ++y )
    {
        for( x=0; x<17; ++x )
        {
            buffer[(y*17+x)*4]=buffer[(y*17+x)*4+1]=buffer[(y*17+x)*4+2]=
            buffer[(y*17+x)*4+3]=0x00;
        }
    }

    sgui_pixmap_load( pixmap, 32, 64, buffer, 0, 0, 17, 17, 17, SGUI_RGBA8 );

    /* tab caption */
    for( y=0; y<4; ++y )
    {
        for( x=0; x<10; ++x )
        {
            buffer[(y*10+x)*4]=buffer[(y*10+x)*4+1]=buffer[(y*10+x)*4+2]=0x64;
            buffer[(y*10+x)*4+3]=0xFF;
        }
    }

    for( x=0; x<10; ++x )
    {
        buffer[x*4]=buffer[x*4+1]=buffer[x*4+2]=buffer[x*4+3]=0xFF;
    }

    sgui_pixmap_load( pixmap, 38, 38, buffer, 0, 0, 10, 4, 10, SGUI_RGBA8 );
}

/****************************************************************************/

void sgui_skin_init_default( sgui_skin* skin )
{
    SET_ELEMENT( skin, SGUI_PBAR_H_STIPPLED_START,        0, 12,  5, 30 );
    SET_ELEMENT( skin, SGUI_PBAR_H_STIPPLED_EMPTY,        1, 12, 12, 30 );
    SET_ELEMENT( skin, SGUI_PBAR_H_STIPPLED_FILLED,      13, 12, 12, 30 );
    SET_ELEMENT( skin, SGUI_PBAR_H_STIPPLED_END,         21, 12,  5, 30 );
    SET_ELEMENT( skin, SGUI_PBAR_H_FILLED_START,          0, 12,  1, 30 );
    SET_ELEMENT( skin, SGUI_PBAR_H_FILLED_EMPTY,          1, 12, 12, 30 );
    SET_ELEMENT( skin, SGUI_PBAR_H_FILLED_FILLED,        26, 12,  1, 30 );
    SET_ELEMENT( skin, SGUI_PBAR_H_FILLED_END,           25, 12,  1, 30 );
    SET_ELEMENT( skin, SGUI_PBAR_V_STIPPLED_START,        0, 42, 30,  5 );
    SET_ELEMENT( skin, SGUI_PBAR_V_STIPPLED_EMPTY,        0, 43, 30, 12 );
    SET_ELEMENT( skin, SGUI_PBAR_V_STIPPLED_FILLED,       0, 50, 30, 12 );
    SET_ELEMENT( skin, SGUI_PBAR_V_STIPPLED_END,          0, 63, 30,  5 );
    SET_ELEMENT( skin, SGUI_PBAR_V_FILLED_START,          0, 42, 30,  1 );
    SET_ELEMENT( skin, SGUI_PBAR_V_FILLED_EMPTY,          0, 43, 30, 12 );
    SET_ELEMENT( skin, SGUI_PBAR_V_FILLED_FILLED,         0, 68, 30, 12 );
    SET_ELEMENT( skin, SGUI_PBAR_V_FILLED_END,            0, 67, 30,  1 );
    SET_ELEMENT( skin, SGUI_CHECKBOX,                     0,  0, 12, 12 );
    SET_ELEMENT( skin, SGUI_CHECKBOX_SELECTED,           12,  0, 12, 12 );
    SET_ELEMENT( skin, SGUI_RADIO_BUTTON,                24,  0, 12, 12 );
    SET_ELEMENT( skin, SGUI_RADIO_BUTTON_SELECTED,       36,  0, 12, 12 );
    SET_ELEMENT( skin, SGUI_BUTTON_LEFT_TOP,             38, 12,  5,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_RIGHT_TOP,            43, 12,  5,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_LEFT_BOTTOM,          38, 17,  5,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_RIGHT_BOTTOM,         43, 17,  5,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_LEFT,                 38, 13,  5,  8 );
    SET_ELEMENT( skin, SGUI_BUTTON_RIGHT,                43, 13,  5,  8 );
    SET_ELEMENT( skin, SGUI_BUTTON_TOP,                  39, 12,  8,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_BOTTOM,               39, 17,  8,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_FILL,                 39, 13,  8,  8 );
    SET_ELEMENT( skin, SGUI_BUTTON_IN_LEFT_TOP,          38, 22,  5,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_IN_RIGHT_TOP,         43, 22,  5,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_IN_LEFT_BOTTOM,       38, 27,  5,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_IN_RIGHT_BOTTOM,      43, 27,  5,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_IN_LEFT,              38, 23,  5,  8 );
    SET_ELEMENT( skin, SGUI_BUTTON_IN_RIGHT,             43, 23,  5,  8 );
    SET_ELEMENT( skin, SGUI_BUTTON_IN_TOP,               39, 22,  8,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_IN_BOTTOM,            39, 27,  8,  5 );
    SET_ELEMENT( skin, SGUI_BUTTON_IN_FILL,              39, 23,  8,  8 );
    SET_ELEMENT( skin, SGUI_EDIT_BOX_LEFT,                0, 12,  5, 30 );
    SET_ELEMENT( skin, SGUI_EDIT_BOX_CENTER,              1, 12, 12, 30 );
    SET_ELEMENT( skin, SGUI_EDIT_BOX_RIGHT,              21, 12,  5, 30 );
    SET_ELEMENT( skin, SGUI_EDIT_BOX_CURSOR,             50, 42,  1, 20 );
    SET_ELEMENT( skin, SGUI_FRAME_LEFT_TOP,               0,  0, 10, 10 );  
    SET_ELEMENT( skin, SGUI_FRAME_RIGHT_TOP,              2,  0, 10, 10 );  
    SET_ELEMENT( skin, SGUI_FRAME_LEFT_BOTTOM,            0,  2, 10, 10 );  
    SET_ELEMENT( skin, SGUI_FRAME_RIGHT_BOTTOM,           2,  2, 10, 10 );  
    SET_ELEMENT( skin, SGUI_FRAME_LEFT,                   0,  1, 10, 10 );  
    SET_ELEMENT( skin, SGUI_FRAME_RIGHT,                  2,  1, 10, 10 );  
    SET_ELEMENT( skin, SGUI_FRAME_TOP,                    1,  0, 10, 10 );  
    SET_ELEMENT( skin, SGUI_FRAME_BOTTOM,                 1,  2, 10, 10 );  
    SET_ELEMENT( skin, SGUI_FRAME_CENTER,                 1,  1, 10, 10 );  
    SET_ELEMENT( skin, SGUI_FRAME_BORDER,                 0,  0,  1,  1 );    
    SET_ELEMENT( skin, SGUI_GROUPBOX_LEFT_TOP,           30, 62, 13, 13 );
    SET_ELEMENT( skin, SGUI_GROUPBOX_RIGHT_TOP,          38, 62, 13, 13 );
    SET_ELEMENT( skin, SGUI_GROUPBOX_LEFT_BOTTOM,        30, 70, 13, 13 );
    SET_ELEMENT( skin, SGUI_GROUPBOX_RIGHT_BOTTOM,       38, 70, 13, 13 );
    SET_ELEMENT( skin, SGUI_GROUPBOX_LEFT,               30, 64,  2, 13 ); 
    SET_ELEMENT( skin, SGUI_GROUPBOX_RIGHT,              49, 64,  2, 13 ); 
    SET_ELEMENT( skin, SGUI_GROUPBOX_TOP,                32, 62, 13,  2 ); 
    SET_ELEMENT( skin, SGUI_GROUPBOX_BOTTOM,             32, 81, 13,  2 ); 
    SET_ELEMENT( skin, SGUI_TAB_CAP_LEFT,                25, 12,  1, 24 ); 
    SET_ELEMENT( skin, SGUI_TAB_CAP_CENTER,              38, 38, 10, 24 );
    SET_ELEMENT( skin, SGUI_TAB_CAP_RIGHT,                0, 12,  1, 24 );  
    SET_ELEMENT( skin, SGUI_TAB_LEFT_TOP,                38, 12,  8,  8 );  
    SET_ELEMENT( skin, SGUI_TAB_RIGHT_TOP,               40, 12,  8,  8 );  
    SET_ELEMENT( skin, SGUI_TAB_LEFT_BOTTOM,             38, 14,  8,  8 );  
    SET_ELEMENT( skin, SGUI_TAB_RIGHT_BOTTOM,            40, 14,  8,  8 );  
    SET_ELEMENT( skin, SGUI_TAB_LEFT,                    38, 13,  1,  8 );  
    SET_ELEMENT( skin, SGUI_TAB_RIGHT,                   47, 13,  1,  8 );  
    SET_ELEMENT( skin, SGUI_TAB_TOP,                     39, 12,  8,  1 );  
    SET_ELEMENT( skin, SGUI_TAB_BOTTOM,                  39, 21,  8,  1 );  
    SET_ELEMENT( skin, SGUI_TAB_GAP_LEFT,                 0,  0,  0,  0 );    
    SET_ELEMENT( skin, SGUI_TAB_GAP_RIGHT,                0,  0,  0,  0 );    
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_H_PANE_LEFT,      48, 20,  7, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_H_PANE_CENTER,    49, 20,  6, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_H_PANE_RIGHT,     61, 20,  7, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_V_PANE_TOP,       48,  0, 20,  7 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_V_PANE_CENTER,    48, 13, 20,  6 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_V_PANE_BOTTOM,    48, 13, 20,  7 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_V_BACKGROUND,     30, 42, 20, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_H_BACKGROUND,     30, 42, 20, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_BUTTON_UP,        88,  0, 20, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_BUTTON_DOWN,      48,  0, 20, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_BUTTON_LEFT,      48, 20, 20, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_BUTTON_RIGHT,     88, 20, 20, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_BUTTON_UP_IN,    108,  0, 20, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_BUTTON_DOWN_IN,   68,  0, 20, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_BUTTON_LEFT_IN,   68, 20, 20, 20 );
    SET_ELEMENT( skin, SGUI_SCROLL_BAR_BUTTON_RIGHT_IN, 108, 20, 20, 20 );

    skin->load_to_pixmap = default_skin_to_pixmap;

    skin->window_color[0] = 0x64;
    skin->window_color[1] = 0x64;
    skin->window_color[2] = 0x64;
    skin->window_color[3] = 0x80;

    skin->font_color[0] = 0xFF;
    skin->font_color[1] = 0xFF;
    skin->font_color[2] = 0xFF;
    skin->font_color[3] = 0xFF;

    skin->font_height = 16;

    skin->pixmap_width = 128;
    skin->pixmap_height = 128;
}


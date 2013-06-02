/*
 * font_cache.c
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
#include "internal.h"



typedef struct GLYPH GLYPH;



struct GLYPH
{
    int x, y, bearing;
    unsigned int width, height, codepoint;

    int red;

    sgui_font* font;

    GLYPH* left;
    GLYPH* right;
};



static Pixmap font_pixmap = 0;
static Picture font_pic = 0;
static int next_x = 0, next_y = 0;
static unsigned int row_height = 0;

static GLYPH* root = NULL;



#define FONT_MAP_WIDTH 256
#define FONT_MAP_HEIGHT 256



#define IS_RED( g ) ((g) && (g)->red)



int create_font_cache( void )
{
    XRenderPictFormat* fmt;
    XRenderColor c;

    root = NULL;
    next_x = 0;
    next_y = 0;
    row_height = 0;

    /* create pixmap and picture */
    font_pixmap = XCreatePixmap( dpy, DefaultRootWindow(dpy),
                                 FONT_MAP_WIDTH, FONT_MAP_HEIGHT, 8 );

    if( !font_pixmap )
        return 0;

    fmt = XRenderFindStandardFormat( dpy, PictStandardA8 );
    font_pic = XRenderCreatePicture( dpy, font_pixmap, fmt, 0, NULL );

    if( !font_pic )
    {
        XFreePixmap( dpy, font_pixmap );
        font_pixmap = 0;
        return 0;
    }

    /* "initialise" the font pixmap */
    c.red = c.green = c.blue = c.alpha = 0x0000;

    XRenderFillRectangle( dpy, PictOpSrc, font_pic, &c,
                          0, 0, FONT_MAP_WIDTH, FONT_MAP_HEIGHT );

    return 1;
}

static void destroy_tree( GLYPH* g )
{
    if( g )
    {
        destroy_tree( g->left );
        destroy_tree( g->right );
    }

    free( g );
}

void destroy_font_cache( void )
{
    if( font_pic )
        XRenderFreePicture( dpy, font_pic );

    if( font_pixmap )
        XFreePixmap( dpy, font_pixmap );

    destroy_tree( root );

    root = NULL;
    next_x = 0;
    next_y = 0;
    font_pic = 0;
    font_pixmap = 0;
    row_height = 0;
}

/****************************************************************************/

static GLYPH* create_glyph( sgui_font* font, unsigned int codepoint )
{
    const unsigned char* src;
    unsigned int w, h, i, j;
    XRenderColor c;
    GLYPH* g;
    int b;

    /* load glyph and get metrics */
    sgui_font_load_glyph( font, codepoint );
    sgui_font_get_glyph_metrics( font, &w, &h, &b );
    src = sgui_font_get_glyph( font );

    /* calculate position for new glyph */
    if( (next_x + w) >= FONT_MAP_WIDTH )
    {
        next_x  = 0;
        next_y += row_height;
        row_height = 0;
    }

    if( h > row_height )
        row_height = h;

    /* create glyph */
    g = malloc( sizeof(GLYPH) );
    g->left = NULL;
    g->right = NULL;
    g->red = 1;
    g->codepoint = codepoint;
    g->x = next_x;
    g->y = next_y;
    g->width = w;
    g->height = h;
    g->bearing = b;
    g->font = font;

    /* copy glyph to pixmap */
    if( src )
    {
        for( j=0; j<h; ++j )
        {
            for( i=0; i<w; ++i, ++src )
            {
                c.red = c.green = c.blue = c.alpha = (*src)<<8;

                XRenderFillRectangle( dpy, PictOpSrc, font_pic, &c,
                                      g->x+i, g->y+j, 1, 1 );
            }
        }
    }

    /* advance next glyph position */
    next_x += w;

    return g;
}

static GLYPH* insert( sgui_font* font, GLYPH* g, unsigned int codepoint )
{ 
    GLYPH* x;

    /* Reached a NULL node? Create a new node. */
    if( !g )
        return create_glyph( font, codepoint );

    /* continue traversing down the tree */
    if( codepoint == g->codepoint )
    {
        if( font < g->font )
            g->left = insert( font, g->left, codepoint ); 
        else if( font > g->font )
            g->right = insert( font, g->right, codepoint ); 
    }
    else
    {
        if( codepoint < g->codepoint )
            g->left = insert( font, g->left, codepoint ); 
        else
            g->right = insert( font, g->right, codepoint ); 
    }

    /* rotate left */
    if( IS_RED(g->right) && !IS_RED(g->left) )
    {
        x = g->right;
        g->right = x->left;
        x->left = g;
        x->red = x->left->red;
        x->left->red = 1;
        g = x;
    }

    /* rotate right */
    if( IS_RED(g->left) && IS_RED(g->left->left) )
    {
        x = g->left;
        g->left = x->right;
        x->right = g;
        x->red = x->right->red;
        x->right->red = 1;
        g = x;
    }

    /* flip colors */
    if( IS_RED(g->left) && IS_RED(g->right) )
    {
        g->red = !g->red;
        g->left->red = !g->left->red;
        g->right->red = !g->right->red;
    }

    return g;
}

static void insert_glyph( sgui_font* font, unsigned int codepoint )
{
    root = insert( font, root, codepoint );
    root->red = 0;
}

static GLYPH* find_glyph( sgui_font* font, unsigned int codepoint )
{
    GLYPH* g = root;

    while( g )
    {
        if( g->codepoint == codepoint )
        {
            if( g->font == font )
                return g;

            g = (font < g->font) ? g->left : g->right;
        }
        else
        {
            g = (codepoint < g->codepoint) ? g->left : g->right;
        }
    }

    return NULL;
}

/****************************************************************************/

int draw_glyph( sgui_font* font, unsigned int codepoint, int x, int y,
                Picture dst, Picture pen )
{
    GLYPH* g;

    if( font )
    {
        g = find_glyph( font, codepoint );

        if( !g )
        {
            insert_glyph( font, codepoint );
            g = find_glyph( font, codepoint );
        }

        if( g )
        {
            XRenderComposite( dpy, PictOpOver, pen, font_pic, dst,
                              0, 0, g->x, g->y, x, y + g->bearing,
                              g->width, g->height );

            return g->width;
        }
    }

    return 0;
}


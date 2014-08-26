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
#define SGUI_BUILDING_DLL
#include "sgui_font_cache.h"
#include "sgui_internal.h"
#include "sgui_pixmap.h"
#include "sgui_font.h"

#include <stdlib.h>
#include <string.h>


typedef struct
{
    sgui_icon super;

    int bearing;            /* bearing of the glyph */
    sgui_font* font;        /* the font used by the glyph */
}
GLYPH;


static int glyph_compare( sgui_icon* left, sgui_icon* right )
{
    if( left->id == right->id )
    {
        if( ((GLYPH*)left)->font == ((GLYPH*)right)->font )
            return 0;

        return ((GLYPH*)left)->font < ((GLYPH*)right)->font ? -1 : 1;
    }

    return left->id < right->id ? -1 : 1;
}

static GLYPH* create_glyph( sgui_icon_cache* this, sgui_font* font,
                            unsigned int codepoint )
{
    const unsigned char* src;
    unsigned int w, h;
    GLYPH* g;
    int b;

    /* load glyph and get metrics */
    sgui_font_load_glyph( font, codepoint );
    sgui_font_get_glyph_metrics( font, &w, &h, &b );
    src = sgui_font_get_glyph( font );

    /* create glyph */
    if( !(g = malloc( sizeof(GLYPH) )) )
        return NULL;

    memset( g, 0, sizeof(GLYPH) );
    g->super.red = 1;
    g->super.id = codepoint;
    g->bearing = b;
    g->font = font;

    /* copy glyph to pixmap */
    if( src && w && h )
    {
        if( !sgui_icon_cache_alloc_area( this, w, h, &g->super.area ) )
        {
            free( g );
            return NULL;
        }

        sgui_pixmap_load( this->pixmap, g->super.area.left, g->super.area.top,
                          src, 0, 0, w, h, w, SGUI_A8 );
    }
    else
    {
        g->super.area.right = w-1;      /* empty dummy area */
    }

    this->root = sgui_icon_cache_tree_insert(this, this->root, (sgui_icon*)g);
    this->root->red = 0;
    return g;
}

static GLYPH* fetch_glyph( sgui_icon_cache* this,
                           sgui_font* font, unsigned int codepoint)
{
    sgui_icon* node;

    if( !this || !font )
        return NULL;

    sgui_internal_lock_mutex( );
    node = this->root;

    while( node )
    {
        if( node->id == codepoint )
        {
            if( ((GLYPH*)node)->font == font )
                goto done;

            node = (font < ((GLYPH*)node)->font) ? node->left : node->right;
        }
        else
        {
            node = (codepoint < node->id) ? node->left : node->right;
        }
    }

    node = (sgui_icon*)create_glyph( this, font, codepoint );
done:
    sgui_internal_unlock_mutex( );
    return (GLYPH*)node;
}

/****************************************************************************/

sgui_icon_cache* sgui_font_cache_create( sgui_pixmap* map )
{
    sgui_icon_cache* this = malloc( sizeof(sgui_icon_cache) );

    if( this )
    {
        memset( this, 0, sizeof(sgui_icon_cache) );
        sgui_pixmap_get_size( map, &this->width, &this->height );

        this->pixmap = map;
        this->icon_compare = glyph_compare;
    }
    return this;
}

int sgui_font_cache_draw_glyph( sgui_icon_cache* this, sgui_font* font,
                                unsigned int codepoint, int x, int y,
                                sgui_canvas* cv, const unsigned char* color )
{
    GLYPH* g = NULL;

    if( this && font && cv && color && (g=fetch_glyph(this,font,codepoint)) )
    {
        if( g->super.area.top != g->super.area.bottom )
        {
            cv->blend_glyph( cv, x, y+g->bearing, this->pixmap,
                             &g->super.area, color );
        }
    }

    return g ? SGUI_RECT_WIDTH( g->super.area ) : 0;
}

void sgui_font_cache_load_glyph( sgui_icon_cache* this, sgui_font* font,
                                 unsigned int codepoint )
{
    fetch_glyph( this, font, codepoint );
}


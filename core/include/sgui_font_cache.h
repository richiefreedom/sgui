/*
 * sgui_font_cache.h
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
#ifndef SGUI_FONT_CACHE_H
#define SGUI_FONT_CACHE_H



#include "sgui_predef.h"



#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Create a font cache object
 *
 * \param map A pointer to a pixmap to cache the glyphs on
 *
 * \return A pointer to a new font cache object on success, NULL otherwise.
 */
SGUI_DLL sgui_font_cache* sgui_font_cache_create( sgui_pixmap* map );

/**
 * \brief Destroy a font cache object
 *
 * \param cache The font cache object
 */
SGUI_DLL void sgui_font_cache_destroy( sgui_font_cache* cache );

/**
 * \brief Render a glyph using a font cache object
 *
 * If the given codepoint and font combination is unknown to the font cache,
 * the coresponding glyph is loaded onto the pixmap object used by the font
 * cache and the codepoint/font combination stored internally.
 * If it is possible to obtain a glyph location of a codepoint/font pair, the
 * glyph from the caching pixmap is used to render the glyph onto the given
 * canvas.
 *
 * \param cache     A pointer to a font cache object
 * \param font      A pointer to a font object to use the glyph from
 * \param codepoint The unicode codepoint of the glyph to draw
 * \param x         Dinstance from the left of the canvas to draw the glyph at
 * \param y         Dinstance from the top of the canvas to draw the glyph at
 * \param canvas    A pointer to the canvas to draw to
 * \param color     The color to use for drawing the glyph
 *
 * \return The width of the given glyph in pixels
 */
int sgui_font_cache_draw_glyph( sgui_font_cache* cache, sgui_font* font,
                                unsigned int codepoint, int x, int y,
                                sgui_canvas* canvas, unsigned char* color );

#ifdef __cplusplus
}
#endif

#endif /* SGUI_FONT_CACHE_H */


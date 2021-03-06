/*
 * sgui_rect.h
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

/**
 * \file sgui_rect.h
 *
 * \brief Contains functions for the sgui_rect helper datatype used to
 *        handle operations on rectangles.
 */
#ifndef SGUI_RECT_H
#define SGUI_RECT_H



#include "sgui_predef.h"



/**
 * \struct sgui_rect
 *
 * \brief An axis aligned rectangle represented by four edge distances
 */
struct sgui_rect
{
    int left;
    int top;
    int right;
    int bottom;
};



#define SGUI_RECT_WIDTH( r ) ((r).right - (r).left + 1)
#define SGUI_RECT_HEIGHT( r ) ((r).bottom - (r).top + 1)

#define SGUI_RECT_WIDTH_V( r ) ((r)->right - (r)->left + 1)
#define SGUI_RECT_HEIGHT_V( r ) ((r)->bottom - (r)->top + 1)

#define SGUI_RECT_SET( R, l, t, r, b )\
        (R).left=(l); (R).top=(t); (R).right=(r); (R).bottom=(b)



#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Make sure left is left of right and top is above bottom
 *
 * \memberof sgui_rect
 *
 * \param r A pointer to a rect
 */
SGUI_DLL void sgui_rect_repair( sgui_rect* r );

/**
 * \brief Set the coordinates of a rect using position and size
 *
 * \memberof sgui_rect
 *
 * Coordinates are given as signed intgers. The horizontal axis of the
 * coordinate system points to the left, the vertical axis points down, so
 * that the top edge coordinate of a rect is smaller than the bottom edge
 * coordinate and the left coordinate is smaller than the right.
 *
 * \param r      A pointer to the rect to set
 * \param left   The horizontal distance of the left edge from the origin
 * \param top    The vertical distance of the top edge from the origin
 * \param width  The width of the rectangle
 * \param height The height of the rectangle
 */
SGUI_DLL void sgui_rect_set_size( sgui_rect* r, int left, int top,
                                  unsigned int width, unsigned int height );

/**
 * \brief Set the coordinates of rect
 *
 * \memberof sgui_rect
 *
 * Coordinates are given as signed intgers. The horizontal axis of the
 * coordinate system points to the left, the vertical axis points down, so
 * that the top edge coordinate of a rect is smaller than the bottom edge
 * coordinate and the left coordinate is smaller than the right.
 *
 * \param r      The rectangle to set
 * \param left   The horizontal distance of the left edge from the origin
 * \param top    The vertical distance of the top edge from the origin
 * \param right  The horizontal distance of the right edge from the origin
 * \param bottom The vertical distance of the bottom edge from the origin
 */
static SGUI_INLINE void sgui_rect_set( sgui_rect* r, int left, int top,
                                       int right, int bottom )
{
    r->left   = left;
    r->top    = top;
    r->right  = right;
    r->bottom = bottom;
}

/**
 * \brief Reposition a given rect
 *
 * \memberof sgui_rect
 *
 * \param left The horizontal distance of the left edge from the origin
 * \param top  The vertical distance of the top edge from the origin
 */
static SGUI_INLINE
void sgui_rect_set_position( sgui_rect* r, int left, int top )
{
    r->right  = r->right  - r->left + left;
    r->bottom = r->bottom - r->top  + top;
    r->left   = left;
    r->top    = top;
}

/**
 * \brief Add an offset to a rect
 *
 * \memberof sgui_rect
 *
 * \param h A horizontal offset, added to left and right
 * \param v A vertical offset, added to top and bottom
 */
static SGUI_INLINE void sgui_rect_add_offset( sgui_rect* r, int h, int v )
{
    r->left   += h;
    r->right  += h;
    r->top    += v;
    r->bottom += v;
}

/**
 * \brief Extend a rect
 *
 * \memberof sgui_rect
 *
 * \param h A horizontal extension, added to right and subtracted from left
 * \param v A vertical extension, added to bottom and subtracted from top
 */
static SGUI_INLINE void sgui_rect_extend( sgui_rect* r, int h, int v )
{
    r->left   -= h;
    r->right  += h;
    r->top    -= v;
    r->bottom += v;
}

/**
 * \brief Get the intersection between to rectangles
 *
 * \memberof sgui_rect
 *
 * This performs an intersection test between two rectangles and returns the
 * intersection. The pointer to the intersection rectangle can be safely set
 * to one of the testing rectangles as it is not touched until the
 * intersection test is done. If the intersection test fails, it is set to
 * zero.
 *
 * \param r If non-NULL, returns the intersection area between the rectangles
 * \param a The first rectangle to test
 * \param b The second rectangle to test
 *
 * \return Non-zero if the rectangles intersect, zero if they don't
 */
SGUI_DLL int sgui_rect_get_intersection( sgui_rect* r, const sgui_rect* a,
                                         const sgui_rect* b );

/**
 * \brief Join two rectangles
 *
 * \memberof sgui_rect
 *
 * \param acc           The accumulator rectangle.
 * \param r             The rectangle to join to the accumulator rectangle.
 * \param only_if_touch If non-zero, the rectangles are only joined if they
 *                      touch.
 *
 * \return Non-zero on success, zero otherwise.
 */
SGUI_DLL int sgui_rect_join( sgui_rect* acc, const sgui_rect* r,
                             int only_if_touch );

/**
 * \brief Returns non-zero if a given point lies within the given rectangle
 *
 * \memberof sgui_rect
 *
 * \param r The rectangle to test for
 * \param x The hotizontal component of the position
 * \param y The vertical component of the position
 */
SGUI_DLL int sgui_rect_is_point_inside( const sgui_rect* r, int x, int y );

#ifdef __cplusplus
}
#endif

#endif /* SGUI_RECT_H */


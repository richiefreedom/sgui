/*
 * sgui_scroll_bar.h
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
 * \file sgui_scroll_bar.h
 *
 * \brief Contains the declarations of the sgui_scroll_bar widget
 */
#ifndef SGUI_SCROLL_BAR_HEADER
#define SGUI_SCROLL_BAR_HEADER



#include "sgui_predef.h"



#define SGUI_SCROLL_BAR_HORIZONTAL 1
#define SGUI_SCROLL_BAR_VERTICAL   0



/**
 * \brief Scroll bar callback
 *
 * Signature of a callback that gets called when the scroll bar gets scrolled.
 *
 * \param userptr    A predefined user pointer
 * \param new_offset The new view offset of the scroll bar
 * \param delta      The difference between the old and the new view offsets
 */
typedef void(* sgui_scrollbar_callback )( void* userptr, int new_offset,
                                          int delta );



/**
 * \struct sgui_scroll_bar
 *
 * \extends sgui_widget
 *
 * \brief A scroll bar widget
 *
 * \image html frame.png "A horizontal scroll bar can be seen on the right"
 */



#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief Create a scroll bar widget
 *
 * \memberof sgui_scroll_bar
 *
 * \param x                  The horizontal component of the bars position.
 * \param y                  The vertical component of the bars position.
 * \param horizontal         Non-zero for a horizontal bar, zero for a
 *                           vertical bar.
 * \param length             The length of the scroll bar in pixels.
 * \param scroll_area_length The length of the scrolled area in pixels.
 * \param disp_area_length   The length of the displayable area in pixels.
 */
SGUI_DLL sgui_widget* sgui_scroll_bar_create( int x, int y, int horizontal,
                                              unsigned int length,
                                              unsigned int scroll_area_length,
                                              unsigned int disp_area_length );

/**
 * \brief Register a function to be called when the user scrolls
 *
 * \memberof sgui_scroll_bar
 *
 * \param bar     A pointer to the scroll bar
 * \param fun     A pointer to the function to be called
 * \param userptr A user pointer to be stored in the scroll bar and passed to
 *                the callback function
 */
SGUI_DLL void sgui_scroll_bar_on_scroll( sgui_widget* bar,
                                         sgui_scrollbar_callback fun,
                                         void* userptr );

/**
 * \brief Set the scroll area offset in pixels of a scroll bar
 *
 * \memberof sgui_scroll_bar
 *
 * \param bar    The scroll bar
 * \param offset The offset of the scroll area in pixels
 */
SGUI_DLL void sgui_scroll_bar_set_offset( sgui_widget* bar,
                                          unsigned int offset );

/**
 * \brief Get the scroll area offset in pixels from a scroll bar
 *
 * \memberof sgui_scroll_bar
 *
 * \param bar The scroll bar
 *
 * \return The offset of the scroll area in pixels
 */
SGUI_DLL unsigned int sgui_scroll_bar_get_offset( sgui_widget* bar );

/**
 * \brief Inform the scrollbar of a change in the scroll area size
 *
 * \memberof sgui_scroll_bar
 *
 * \param bar                The scroll bar
 * \param scroll_area_length The length of the scrolled area in pixels.
 * \param disp_area_length   The length of the displayable area in pixels.
 */
SGUI_DLL void sgui_scroll_bar_set_area( sgui_widget* bar,
                                        unsigned int scroll_area_length,
                                        unsigned int disp_area_length );

/**
 * \brief Resize a scrollbar
 *
 * \memberof sgui_scroll_bar
 *
 * \note This does not adjust the pane length of the scrollbar, this adjusts
 *       the length of the scrollbar itself. (The pane length is adjusted
 *       automatically). To adjust the pane area, use sgui_scrollbar_set_area.
 *
 * \param bar    The scroll bar
 * \param length The new length of the scroll bar
 */
SGUI_DLL void sgui_scroll_bar_set_length( sgui_widget* bar,
                                          unsigned int length );

#ifdef __cplusplus
}
#endif

#endif /* SGUI_SCROLL_BAR_H */


/*
 * widget_internal.h
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
#ifndef SGUI_WIDGET_INTERNAL_H
#define SGUI_WIDGET_INTERNAL_H



#include "sgui_widget.h"



struct sgui_widget
{
    int x;                  /**< \brief x component of widget position */
    int y;                  /**< \brief y component of widget position */
    unsigned int width;     /**< \brief widget width */
    unsigned int height;    /**< \brief widget height */

    int need_redraw;        /**< \brief non-zero if need for redrawing */

    /**
     * \brief Callback that is called to update a widget
     *
     * \param widget A pointer to the widget to update (for C++
     *               people: practically a this pointer).
     */
    void (* update_callback )( sgui_widget* widget );

    /**
     * \brief Callback that is called to inject window events
     *
     * \param widget A pointer to the widget to update (for C++
     *               people: practically a this pointer).
     * \param wnd    The window that sent the event
     * \param type   The event type.
     * \param event  The window event that occoured.
     */
    void (* window_event_callback )( sgui_widget* widget, sgui_window* wnd,
                                     int type, sgui_event* event );
};



#endif /* SGUI_WIDGET_INTERNAL_H */


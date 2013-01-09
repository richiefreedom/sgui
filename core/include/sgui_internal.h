/*
 * sgui_internal.h
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
#ifndef SGUI_INTERNAL_H
#define SGUI_INTERNAL_H



#include "sgui_predef.h"
#include "sgui_rect.h"
#include "sgui_window.h"



#define SGUI_CANVAS_STACK_DEPTH 10

struct sgui_canvas
{
    int allow_clear;
    int ox, oy;                     /**< \brief current offset */

    unsigned int width, height;     /**< \brief Size of the canvas */

    sgui_rect sc;                   /**< \brief current scissor rect */

    sgui_rect sc_stack[ SGUI_CANVAS_STACK_DEPTH ];
    unsigned int scissor_stack_pointer;

    int offset_stack_x[ SGUI_CANVAS_STACK_DEPTH ];
    int offset_stack_y[ SGUI_CANVAS_STACK_DEPTH ];
    unsigned int offset_stack_pointer;

    int began;

    unsigned char bg_color[3];      /**< \brief RGB8 background color */


    unsigned char* buffer;
    unsigned int buffer_x, buffer_y, buffer_w, buffer_h;


    /**
     * \brief Gets called by sgui_canvas_begin to load a the region to redraw
     *        from the display servers memory into the scratch_buffer
     *
     * \param canvas A pointer to the canvas (for C++ people: practically a
     *               this pointer).
     * \param r      The rectangle to load (already clamped to the canvas)
     */
    void(* download )( sgui_canvas* canvas, sgui_rect* r );

    /**
     * \brief Gets called by sgui_canvas_end to upload the modified region
     *        to the display server.
     *
     * \param canvas A pointer to the canvas (for C++ people: practically a
     *               this pointer).
     */
    void(* upload )( sgui_canvas* canvas );

    /**
     * \brief Clear a portion of a canvas to the background color
     *
     * \param canvas A pointer to the canvas (for C++ people: practically a
     *               this pointer).
     * \param r      The region to clear, with ofset applied and cliped with
     *               the scissor rect
     */
    void(* clear )( sgui_canvas* canvas, sgui_rect* r );
};

struct sgui_widget
{
    sgui_rect area;         /**< \brief The area occupied by a widget */

    int visible;            /**< \brief zero if the widget should not be
                                        rendered */

    /** \brief The widget manager responsible for that widget */
    sgui_widget_manager* mgr;

    /**
     * \brief The next widget on the same level (linked list)
     *
     * Used by widget manager. Do not alter in widget code!
     */
    sgui_widget* next;

    /**
     * \brief Callback that is called to draw a widget
     *
     * \param widget A pointer to the widget to draw (for C++
     *               people: practically a this pointer).
     * \param cv     A pointer to a canvas to draw to.
     */
    void (* draw_callback )( sgui_widget* widget, sgui_canvas* cv );

    /**
     * \brief Callback that is called to inject window events
     *
     * \param widget A pointer to the widget to update (for C++
     *               people: practically a this pointer).
     * \param type   The event type.
     * \param event  The window event that occoured.
     */
    void (* window_event_callback )( sgui_widget* widget, int type,
                                     sgui_event* event );
};

struct sgui_window
{
    sgui_widget_manager* mgr;   /**< \brief pointer to a widget manager */
    sgui_canvas* back_buffer;   /**< \brief pointer to a canvas */

    sgui_window_callback event_fun; /**< \brief the window event callback */

    int x, y;                   /**< \brief position of the window */
    unsigned int w, h;          /**< \brief the size of the window */

    int visible;                /**< \brief Window visibility */
};



#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialise a widget structure
 *
 * \param widget A pointer to the widget structure
 * \param x      The horizontal component of the widgets position
 * \param y      The vertical component of the widgets position
 * \param width  The width of the widget
 * \param height The height of the widget
 */
SGUI_DLL void sgui_internal_widget_init( sgui_widget* widget, int x, int y,
                                         unsigned int width,
                                         unsigned int height );

/**
 * \brief Initialise a canvas structure
 *
 * \param cv     A pointer to the canvas structure
 * \param width  The width of the canvas
 * \param height The height of the canvas
 */
SGUI_DLL void sgui_internal_canvas_init( sgui_canvas* cv, unsigned int width,
                                         unsigned int height );

/**
 * \brief Propagate a window event
 *
 * \param wnd   The window that trigered the event
 * \param event The event that got triggered
 * \param e     A pointer ot a struct with additional information for an event
 */
SGUI_DLL void sgui_internal_window_fire_event( sgui_window* wnd, int event,
                                               sgui_event* e );

#ifdef __cplusplus
}
#endif


#endif /* SGUI_WIDGET_INTERNAL_H */

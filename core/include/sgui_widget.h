/*
 * sgui_widget.h
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
 * \file sgui_widget.h
 *
 * \brief Contains the declaration of the abstract widget datatype.
 */
#ifndef SGUI_WIDGET_H
#define SGUI_WIDGET_H



#include "sgui_predef.h"
#include "sgui_rect.h"



/**
 * \enum SGUI_WIDGET_STATE_CHANGE
 *
 * \brief Flags for the widget state change callback
 */
typedef enum
{
    SGUI_WIDGET_POSITION_CHANGED     = 0x01,
    SGUI_WIDGET_VISIBILLITY_CHANGED  = 0x02,
    SGUI_WIDGET_PARENT_CHANGED       = 0x04,
    SGUI_WIDGET_CHILD_ADDED          = 0x08,
    SGUI_WIDGET_CHILD_REMOVED        = 0x10,
    SGUI_WIDGET_CANVAS_CHANGED       = 0x20
}
SGUI_WIDGET_STATE_CHANGE_FLAG;

/**
 * \enum SGUI_WIDGET_FLAG
 *
 * \brief Generic widget flags
 */
typedef enum
{
    SGUI_FOCUS_ACCEPT   = 0x01,    /**< \brief the widget accepts focus */
    SGUI_FOCUS_DRAW     = 0x02,    /**< \brief draw focus box */
    SGUI_FOCUS_DROP_ESC = 0x04,    /**< \brief drop focus on ESC-key */
    SGUI_FOCUS_DROP_TAB = 0x08,    /**< \brief drop focus on TAB-key */
    SGUI_WIDGET_VISIBLE = 0x10     /**< \brief widget is visible */
}
SGUI_WIDGET_FLAG;



/**
 * \struct sgui_widget
 *
 * \brief The common base for all GUI controls
 */
struct sgui_widget
{
    sgui_rect area;  /**< \brief The area occupied by a widget */

    int flags;  /**< \brief A combination of \ref SGUI_WIDGET_FLAG flags */

    /** \brief The canvas that the widget is attached to */
    sgui_canvas* canvas;

    /** \brief The next widget on the same level (linked list) */
    sgui_widget* next;

    /** \brief A pointer to the first widget in the children list */
    sgui_widget* children;

    sgui_widget* parent;    /**< \brief A pointer to the parent widget */

    /** \copydoc sgui_widget_destroy */
    void (* destroy )( sgui_widget* widget );

    /**
     * \brief Draw a widget
     *
     * This is called to draw a widget onto a canvas.
     *
     * \note This may be NULL if not implemented.
     *
     * \param widget The widget to update.
     */
    void (* draw )( sgui_widget* widget );

    /**
     * \brief Callback that is called to inject window events
     *
     * \param widget A pointer to the widget to update.
     * \param event  The window event that occoured.
     */
    void (* window_event )( sgui_widget* widget, const sgui_event* event );

    /**
     * \brief Callback that is called when the internal state of a widget
     *        changes(e.g. position, visibility, etc...)
     * 
     * \param widget A pointer to the widget that changed
     * \param change A combination of \ref SGUI_WIDGET_STATE_CHANGE_FLAG flags
     *               that indicate what changed
     */
    void (* state_change_event )( sgui_widget* widget, int change );
};



#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Used by widget implementations to initialise the widget base struct
 *
 * \memberof sgui_widget
 *
 * \param widget A pointer to the widget structure
 * \param x      The horizontal component of the widgets position
 * \param y      The vertical component of the widgets position
 * \param width  The width of the widget
 * \param height The height of the widget
 */
SGUI_DLL void sgui_widget_init( sgui_widget* widget, int x, int y,
                                unsigned int width, unsigned int height );

/**
 * \brief Destroy a widget, freeing up all its resources
 *
 * \memberof sgui_widget
 *
 * \param widget A pointer to the widget to destroy
 */
static SGUI_INLINE void sgui_widget_destroy( sgui_widget* widget )
{
    widget->destroy( widget );
}

/**
 * \brief Destroy the children of a widget
 *
 * \memberof sgui_widget
 *
 * \param widget A pointer to a widget
 */
SGUI_DLL void sgui_widget_destroy_children( sgui_widget* widget );

/**
 * \brief Recursively destroy ALL children of a widget at all layers below
 *
 * \memberof sgui_widget
 *
 * \param widget A pointer to a widget
 */
SGUI_DLL void sgui_widget_destroy_all_children( sgui_widget* widget );

/**
 * \brief Change the position of a widget
 *
 * \memberof sgui_widget
 *
 * \param w The widget to reposition
 * \param x The x component of the position
 * \param y The y component of the position
 */
SGUI_DLL void sgui_widget_set_position( sgui_widget* w, int x, int y );

/**
 * \brief Get the position of a widget
 *
 * \memberof sgui_widget
 *
 * \param w The widget to get the position from
 * \param x Returns the x component of the position
 * \param y Returns the y component of the position
 */
static SGUI_INLINE void sgui_widget_get_position( const sgui_widget* w,
                                                  int* x, int* y )
{
    *x = w->area.left;
    *y = w->area.top;
}

/**
 * \brief Get the absolute position of a widget (i.e. not parent relative
 *        but relative to the window)
 *
 * \memberof sgui_widget
 *
 * \param w The widget to get the position from
 * \param x Returns the x component of the position
 * \param y Returns the y component of the position
 */
SGUI_DLL void sgui_widget_get_absolute_position( const sgui_widget* w,
                                                 int* x, int* y );

/**
 * \brief Get the size of a widget
 *
 * \memberof sgui_widget
 *
 * \param w      The widget to get the size of
 * \param width  Returns the width of the widget
 * \param height Returns the height of the widget
 */
SGUI_DLL void sgui_widget_get_size( const sgui_widget* w,
                                    unsigned int* width,
                                    unsigned int* height );

/**
 * \brief Returns non-zero if the given widget is visible and all its parents
 *        are visible too
 *
 * \memberof sgui_widget
 */
SGUI_DLL int sgui_widget_is_absolute_visible( const sgui_widget* w );

/**
 * \brief Set whether a given widget should be rendered or not
 *
 * \memberof sgui_widget
 *
 * \param w       The widget
 * \param visible Non-zero to allow rendering of the widget, zero to prohibit.
 */
SGUI_DLL void sgui_widget_set_visible( sgui_widget* w, int visible );

/**
 * \brief Get the bounding box of a widget in absolute coordinates (i.e. not
 *        parent relative but relative to the window)
 *
 * \memberof sgui_widget
 *
 * \param w The widget to get the position from
 * \param r Returns the rect
 */
SGUI_DLL void sgui_widget_get_absolute_rect( const sgui_widget* w,
                                             sgui_rect* r );

/**
 * \brief Send a parent widget or window event to a widget
 *
 * \memberof sgui_widget
 *
 * This is called inside the sgui_window_update function to make widgets
 * interact with user input or by parent widgets to send things like tab group
 * deselect notifications.
 *
 * \param widget    The widget to send the event to
 * \param event     The event to send
 * \param propagate Nonzero if the event should be propagate to the widgets
 *                  children, zero if it should not
 */
SGUI_DLL void sgui_widget_send_event( sgui_widget* widget,
                                      const sgui_event* event,
                                      int propagate );

/**
 * \brief Add a widget as child widget to an other widget
 *
 * \memberof sgui_widget
 *
 * \param parent The parent to add the child widget to
 * \param child  The child widget to add to the parent
 */
SGUI_DLL void sgui_widget_add_child( sgui_widget* parent, sgui_widget* child );

/**
 * \brief Remove a widget from the widget hirarchy
 *
 * \memberof sgui_widget
 *
 * This function removes a widget from its parent and sets its canvas pointer
 * to NULL, as well as all the canvas pointers of all the children of the
 * given widget.
 *
 * \param widget The widget to remove from its parent
 */
SGUI_DLL void sgui_widget_remove_from_parent( sgui_widget* widget );

/**
 * \brief Get a pointer to a child widget from a point within a widget
 *
 * \memberof sgui_widget
 *
 * The function first tests if the given point is inside the given widget.
 * If it is, it continues to test against all children of the given widget
 * and, when it finds a match, transforms the point to child local coordinates
 * and recursively continues with that child widget, until the least child at
 * the bottom of the tree is found that has the given point inside it.
 *
 * If at any point the function runs out of children (i.e. the point does not
 * lie inside any child), the last widget above in the hirarchy is returned.
 *
 * If the point does not even lie in the given widget to begin with, NULL is
 * returned.
 *
 * Also, widgets are skiped if they are not flagged visible.
 *
 * \param widget The widget to test against
 * \param x The distance from the left side of the widget
 * \param y The distance from the top side of the widget
 *
 * \return A pointer to a widget or NULL if the point does not lie inside any
 *         widget.
 */
SGUI_DLL sgui_widget* sgui_widget_get_child_from_point(
                                            const sgui_widget* widget,
                                            int x, int y );

/**
 * \brief Assuming that the given widget has focus, find the next widget
 *        that will get focus when the tab key gets pressed
 *
 * \memberof sgui_widget
 *
 * \param w A pointer to a widget that is assumed to have focus
 *
 * \return A pointer to the next widget in the tab order
 */
SGUI_DLL sgui_widget* sgui_widget_find_next_focus( const sgui_widget* w );

/**
 * \brief Recursively draw a widget and all its children
 *
 * \memberof sgui_widget
 *
 * This function calls the draw function on the given widget (if not NULL)
 * and then recursively calls the draw functions on all children of the
 * given widget. Drawing is done to the canvas set in the widget. The clipping
 * rect and offset of the canvas are adjusted and restored to the bounds of
 * the widget being drawn. A top level rect can be specified to further
 * restrict drawing. If any widget encountered is flaged as not visible, the
 * widget and the entire subtree are skipped. If the starting widget is
 * visible, it is assumed that all parents are visible as well. If the given
 * focus widget is encountered on the way and it has the SGUI_FOCUS_DRAW flag
 * set, a focus box is drawn around it (i.e. set it to NULL to surpress
 * drawing of the focus box).
 *
 * \param w      A pointer to a widget to draw
 * \param bounds If not NULL, a bounding rectangle to clip all drawing
 *               against
 * \param focus  If this widget is encountered along the way and it wishes
 *               wishes a focus box to be drawn, a focus box is drawn
 *               around it
 */
SGUI_DLL void sgui_widget_draw( sgui_widget* w, sgui_rect* bounds,
                                sgui_widget* focus );

#ifdef __cplusplus
}
#endif

#endif /* SGUI_WIDGET_H */


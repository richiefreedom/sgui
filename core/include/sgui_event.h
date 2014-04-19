/*
 * sgui_event.h
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
#ifndef SGUI_EVENT_H
#define SGUI_EVENT_H



#include "sgui_keycodes.h"
#include "sgui_predef.h"
#include "sgui_rect.h"



#define SGUI_MOUSE_BUTTON_LEFT   0
#define SGUI_MOUSE_BUTTON_MIDDLE 1
#define SGUI_MOUSE_BUTTON_RIGHT  2


/**************** window events ****************/
#define SGUI_USER_CLOSED_EVENT          0x0000
#define SGUI_API_INVISIBLE_EVENT        0x0001
#define SGUI_API_DESTROY_EVENT          0x0002
#define SGUI_SIZE_CHANGE_EVENT          0x0003

#define SGUI_MOUSE_MOVE_EVENT           0x0004
#define SGUI_MOUSE_PRESS_EVENT          0x0005
#define SGUI_MOUSE_RELEASE_EVENT        0x0006
#define SGUI_MOUSE_WHEEL_EVENT          0x0007

#define SGUI_KEY_PRESSED_EVENT          0x0008
#define SGUI_KEY_RELEASED_EVENT         0x0009
#define SGUI_CHAR_EVENT                 0x000A

#define SGUI_EXPOSE_EVENT               0x000B

#define SGUI_MAX_WINDOW_EVENT           0x000F

/**************** canvas events ****************/
#define SGUI_MOUSE_ENTER_EVENT          0x0010
#define SGUI_MOUSE_LEAVE_EVENT          0x0011
#define SGUI_FOCUS_EVENT                0x0012
#define SGUI_FOCUS_LOSE_EVENT           0x0013

#define SGUI_MAX_CANVAS_EVENT           0x001F

/**************** button events ****************/
#define SGUI_BUTTON_IN_EVENT            0x0020
#define SGUI_BUTTON_OUT_EVENT           0x0021

/*************** edit box events ***************/
/* text got modified */
#define SGUI_EDIT_BOX_TEXT_CHANGED      0x0030

/* text got modified, user pressed enter */
#define SGUI_EDIT_BOX_TEXT_ENTERED      0x0031

/***************** tab events ******************/
/* the parent tab of a widget got deselected */
#define SGUI_TAB_DESELECTED             0x0040

/* the parent tab of a widget got selected */
#define SGUI_TAB_SELECTED               0x0041

/************** icon view events ***************/
/* an icon got double clicked */
#define SGUI_ICON_SELECTED              0x0050

/* delete got pressed on an icon */
#define SGUI_ICON_DELETE                0x0051

/* the copy shortcut got pressed on an icon */
#define SGUI_ICON_COPY                  0x0052

/* the cut shortcut got pressed on an icon */
#define SGUI_ICON_CUT                   0x0053

/* the paste shortcut got pressed on an icon */
#define SGUI_ICON_PASTE                 0x0054



struct sgui_event
{
    union
    {
        /**
         * \brief Unsigned int vector. Used by SGUI_SIZE_CHANGE_EVENT
         */
        struct { unsigned int x, y; } ui2;

        /**
         * \brief Int vector. Used by SGUI_MOUSE_MOVE_EVENT
         */
        struct { int x, y; } i2;

        /**
         * \brief Int vector. Used by SGUI_MOUSE_PRESS_EVENT with z being a
         *        mouse button identifyer.
         */
        struct { int x, y, z; } i3;

        /**
         * \brief Int parameter
         *
         * Used by SGUI_MOUSE_WHEEL_EVENT as direction (1=up, -1=donw).
         * Used by SGUI_KEY_PRESSED_EVENT and SGUI_KEY_RELEASED_EVENT as
         * keycode.
         */
        int i;

        /**
         * \brief UTF8 string fraction
         *
         * Used by SGUI_CHAR_EVENT as translated utf8 string
         */
        char utf8[8];

        /**
         * \brief Rect paramter
         *
         * Used by SGUI_EXPOSE_EVENT as expose rectangle
         */
        sgui_rect rect;
    }
    arg;

    /** \brief A pointer to the widget that caused the event */
    sgui_widget* widget;

    /** \brief A pointer to the window that caused the event */
    sgui_window* window;

    /** \brief Event type identifyer */
    int type;
};




#define SGUI_FROM_EVENT 0x10
#define SGUI_EVENT 0x10
#define SGUI_WIDGET 0x11
#define SGUI_WINDOW 0x12
#define SGUI_TYPE 0x13
#define SGUI_I 0x14
#define SGUI_I2_X 0x15
#define SGUI_I2_Y 0x16
#define SGUI_I2_XY 0x17
#define SGUI_I2_YX 0x18
#define SGUI_I3_X 0x19
#define SGUI_I3_Y 0x1A
#define SGUI_I3_Z 0x1B
#define SGUI_I3_XY 0x1C
#define SGUI_I3_XZ 0x1D
#define SGUI_I3_YX 0x1E
#define SGUI_I3_YZ 0x1F
#define SGUI_I3_ZX 0x20
#define SGUI_I3_ZY 0x21
#define SGUI_I3_XYZ 0x22
#define SGUI_I3_XZY 0x23
#define SGUI_I3_YXZ 0x24
#define SGUI_I3_YZX 0x25
#define SGUI_I3_ZXY 0x26
#define SGUI_I3_ZYX 0x27
#define SGUI_UI2_X 0x28
#define SGUI_UI2_Y 0x29
#define SGUI_UI2_XY 0x2A
#define SGUI_UI2_YX 0x2B
#define SGUI_UTF8 0x2C
#define SGUI_RECT 0x2D

#define SGUI_VOID 0x00
#define SGUI_CHAR 0x01
#define SGUI_SHORT 0x02
#define SGUI_INT 0x03
#define SGUI_INT2 0x04
#define SGUI_INT3 0x06
#define SGUI_LONG 0x07
#define SGUI_POINTER 0x08

#ifndef SGUI_NO_FLOAT
    #define SGUI_FLOAT 0x0A
    #define SGUI_DOUBLE 0x0B
#endif



/**
 * \brief A generic function pointer type
 *
 * \param object A pointer to the object instance ("this pointer")
 * \param ...    Possible arguments to the function
 */
typedef void (* sgui_function )( void* object, ... );



#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Connect an event with a callback
 *
 * To supply arguments to a callback function, simply pass a datatype
 * identifyer after the callback (e.g. SGUI_CHAR for char, SGUI_INT for int
 * or SGUI_INT2 for two int arguments), followed by the arguments to pass to
 * the callback, or SGUI_VOID for no arguments.
 * If the argument should be an field from the event structure, pass
 * SGUI_FROM_EVENT instead of a datatype, followed by an identifyer for the
 * event field (e.g. SGUI_WIDGET or SGUI_UTF8).
 *
 * When the callback is called, the receiver object is passed as first
 * argument, followed by the supplied arguments.
 *
 * \param sender    A pointer to the sender object, or NULL for any
 * \param eventtype The event identifyer to listen to
 * \param iswidget  Non-zero if the sender is a widget, zero if it is a window
 * \param ...       A pointer to a callback function, followed by a pointer to
 *                  the receiver object and an optional argument
 */
SGUI_DLL void sgui_event_connect( void* sender, int eventtype, int iswidget,
                                  ... );

/**
 * \brief Disconnect an event from a callback
 *
 * Given a sender, event type, receiver and callback, this function runs
 * through the internal list of event connections and disconnects all matching
 * connections.
 *
 * \param sender    A pointer to the sender object
 * \param eventtype The event identifyer to listen to
 * \param callback  A pointer to a callback function
 * \param receiver  A pointer to the receiver object
 */
SGUI_DLL void sgui_event_disconnect( void* sender, int eventtype,
                                     sgui_function callback, void* receiver );

/**
 * \brief Post an event to the event queue
 *
 * \param event A pointer to an event structure
 */
SGUI_DLL void sgui_event_post( const sgui_event* event );

#ifdef __cplusplus
}
#endif

#endif /* SGUI_EVENT_H */


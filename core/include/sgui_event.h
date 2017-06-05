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

/**
 * \file sgui_event.h
 *
 * \brief Contains the declarations and enumerations for the event
 *        processing subsystem.
 */
#ifndef SGUI_EVENT_H
#define SGUI_EVENT_H



#include "sgui_keycodes.h"
#include "sgui_predef.h"
#include "sgui_rect.h"



/**
 * \enum SGUI_MOUSE_BUTTON
 *
 * \brief Mouse button identifyer
 */
typedef enum
{
    SGUI_MOUSE_BUTTON_LEFT   = 0,
    SGUI_MOUSE_BUTTON_MIDDLE = 1,
    SGUI_MOUSE_BUTTON_RIGHT  = 2
}
SGUI_MOUSE_BUTTON;

/**
 * \enum SGUI_EVENT_TYPE
 *
 * \brief Event type identifier
 */
typedef enum
{
    /** \brief Generated by sgui_window when the user closes it */
    SGUI_USER_CLOSED_EVENT          = 0x0000,

    /** \brief Generated by sgui_window when set invisible via the API */
    SGUI_API_INVISIBLE_EVENT        = 0x0001,

    /** \brief Generated by sgui_window. New size is in ui2 argument */
    SGUI_SIZE_CHANGE_EVENT          = 0x0003,

    /** \brief Generated by sgui_window. Cursor position is in i2 argument */
    SGUI_MOUSE_MOVE_EVENT           = 0x0004,

    /**
     * \brief Generated by sgui_window. Cursor position is in i3.xy argument,
     *        button is in i3.z argument (an \ref SGUI_MOUSE_BUTTON value)
     */
    SGUI_MOUSE_PRESS_EVENT          = 0x0005,

    /**
     * \brief Generated by sgui_window. Cursor position is in i3.xy argument,
     *        button is in i3.z argument (an \ref SGUI_MOUSE_BUTTON value)
     */
    SGUI_MOUSE_RELEASE_EVENT        = 0x0006,

    /**
     * \brief Generated by sgui_window. int argument is positive for
     *        up and negative for down
     */
    SGUI_MOUSE_WHEEL_EVENT          = 0x0007,

    /**
     * \brief Generated by sgui_window.
     *        int argument is \ref SGUI_KEYCODE value
     */
    SGUI_KEY_PRESSED_EVENT          = 0x0008,

    /**
     * \brief Generated by sgui_window.
     *        int argument is \ref SGUI_KEYCODE value
     */
    SGUI_KEY_RELEASED_EVENT         = 0x0009,

    /**
     * \brief Generated by sgui_window when a character sequence is
     *        decomposed to a character. utf8 argument is character sequence.
     */
    SGUI_CHAR_EVENT                 = 0x000A,

    /** \brief Generated by sgui_window when the window needs to be redrawn */
    SGUI_EXPOSE_EVENT               = 0x000B,

    /** \brief Generated by sgui_window when a Direct3D 9 device is lost */
    SGUI_D3D9_DEVICE_LOST           = 0x000C,

    /**
     * \brief Generated by sgui_canvas when the mouse cursor enters a widget
     *
     * This event is directly passed to the widget and not posted to
     * the global event queue.
     */
    SGUI_MOUSE_ENTER_EVENT          = 0x0010,

    /**
     * \brief Generated by sgui_canvas when the mouse cursor leaves a widget
     *
     * This event is directly passed to the widget and not posted to
     * the global event queue.
     */
    SGUI_MOUSE_LEAVE_EVENT          = 0x0011,

    /**
     * \brief Generated by sgui_window when a window loses keyboard focus or
     *        by sgui_canvas when a widget loses keyboard focus.
     *
     * When the canvas generates this event, it passes it directly to the
     * widget and does not post it to the global event queue.
     */
    SGUI_FOCUS_EVENT                = 0x0012,

    /**
     * \brief Generated by sgui_window when a window gets keyboard focus or
     *        by sgui_canvas when a widget gets keyboard focus.
     *
     * When the canvas generates this event, it passes it directly to the
     * widget and does not post it to the global event queue.
     */
    SGUI_FOCUS_LOSE_EVENT           = 0x0013,

    /** \brief Generated by sgui_window when a double click occours */
    SGUI_DOUBLE_CLICK_EVENT         = 0x0014,

    /**
     * \brief Generated by an sgui_button if it is pressed inwards
     *
     * A toggle button generates this when being toggled inwards. A check box
     * or radio button generates this when being selected. A normal button
     * generates this when being clicked.
     */
    SGUI_BUTTON_IN_EVENT            = 0x0020,

    /**
     * \brief Generated by an sgui_button if it is reset
     *
     * A toggle button generates this when being toggled outwards. A check box
     * or radio button generates this when being deselected. A normal button
     * generates this when being released.
     */
    SGUI_BUTTON_OUT_EVENT           = 0x0021,

    /** \brief Generated by an sgui_edit_box after the text got modified */
    SGUI_EDIT_BOX_TEXT_CHANGED      = 0x0030,

    /** \brief Generated by an sgui_edit_box when the enter key is pressed */
    SGUI_EDIT_BOX_TEXT_ENTERED      = 0x0031,

    /**
     * \brief Generated by an sgui_numeric_edit when the
     *        value changes (int argument of event)
     */
    SGUI_EDIT_VALUE_CHANGED         = 0x0032,

    /** \brief Generated by an sgui_tab if it got deselected */
    SGUI_TAB_DESELECTED             = 0x0040,

    /** \brief Generated by an sgui_tab if it got selected */
    SGUI_TAB_SELECTED               = 0x0041,

    /**
     * \brief Generated by an sgui_icon_view after an icon got double clicked
     *
     * The source of the event is set to the sgui_item asociated
     * with the icon.
     */
    SGUI_ICON_SELECTED_EVENT        = 0x0050,

    /**
     * \brief Generated by an sgui_icon_view after the delete key got
     *        pressed on an icon
     *
     * The source of the event is set to the sgui_item asociated
     * with the icon.
     */
    SGUI_ICON_DELETE_EVENT          = 0x0051,

    /**
     * \brief Generated by an sgui_icon_view after the copy key composition
     *        got pressed on an icon
     *
     * The source of the event is set to the sgui_item asociated
     * with the icon.
     */
    SGUI_ICON_COPY_EVENT            = 0x0052,

    /**
     * \brief Generated by an sgui_icon_view after the cut key composition
     *        got pressed on an icon
     *
     * The source of the event is set to the sgui_item asociated
     * with the icon.
     */
    SGUI_ICON_CUT_EVENT             = 0x0053,

    /**
     * \brief Generated by an sgui_icon_view after the paste key composition
     *        got pressed
     */
    SGUI_ICON_PASTE_EVENT           = 0x0054,

    /**
     * \brief Generated by an sgui_slider after its value changed
     *        (passed as int argument)
     */
    SGUI_SLIDER_CHANGED_EVENT       = 0x0060,

    /**
     * \brief Generated by an sgui_color_picker when the color changed
     *
     * The color argument contains an RGBA color.
     */
    SGUI_RGBA_CHANGED_EVENT         = 0x0070,

    /**
     * \brief Generated by an sgui_color_picker when the color changed
     *
     * The color argument contains a HSVA color.
     */
    SGUI_HSVA_CHANGED_EVENT         = 0x0071,

    /** \brief Generated by sgui_dialog if it got rejected */
    SGUI_DIALOG_REJECTED            = 0x0100,

    /**
     * \brief Generated by sgui_message_box when the first button gets pressed
     */
    SGUI_MESSAGE_BOX_BUTTON1_EVENT  = 0x0101,

    /**
     * \brief Generated by sgui_message_box when the second
     *        button gets pressed
     */
    SGUI_MESSAGE_BOX_BUTTON2_EVENT  = 0x0102,

    /**
     * \brief Generated by sgui_message_box when the third
     *        button gets pressed
     */
    SGUI_MESSAGE_BOX_BUTTON3_EVENT  = 0x0103,

    /**
     * \brief Generated by an sgui_color_dialog when it gets accepted
     *
     * The color argument contains the last selected RGBA color.
     */
    SGUI_COLOR_SELECTED_RGBA_EVENT  = 0x0110,

    /**
     * \brief Generated by an sgui_color_dialog when it gets accepted
     *
     * The color argument contains the last selected HSVA color.
     */
    SGUI_COLOR_SELECTED_HSVA_EVENT  = 0x0111
}
SGUI_EVENT_TYPE;



/**
 * \struct sgui_event
 *
 * \brief Encapsulates an event generated by a window, canvas or widget
 */
struct sgui_event
{
    /** \brief A variant argument */
    union
    {
        /**
         * \brief Unsigned int vector. Used by SGUI_SIZE_CHANGE_EVENT
         */
        struct { unsigned int x, y; } ui2;

        /**
         * \brief Int vector. Used by SGUI_MOUSE_MOVE_EVENT and
         *        SGUI_DOUBLE_CLICK_EVENT
         */
        struct { int x, y; } i2;

        /**
         * \brief Int vector. Used by SGUI_MOUSE_PRESS_EVENT and
         *        SGUI_MOUSE_RELEASE_EVENT with z being a mouse button
         *        identifyer.
         *
         * \see SGUI_MOUSE_BUTTON
         */
        struct { int x, y, z; } i3;

        /**
         * \brief Int parameter
         *
         * Used by SGUI_MOUSE_WHEEL_EVENT as direction (1=up, -1=donw).
         * Used by SGUI_KEY_PRESSED_EVENT and SGUI_KEY_RELEASED_EVENT as
         * keycode. Used by SGUI_EDIT_VALUE_CHANGED for the new value.
         *
         * \see SGUI_KEYCODE
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

        /**
         * \brief Color value
         *
         * Used by SGUI_RGBA_CHANGED_EVENT, SGUI_HSVA_CHANGED_EVENT,
         * SGUI_COLOR_SELECTED_RGBA_EVENT and SGUI_COLOR_SELECTED_HSVA_EVENT.
         */
        unsigned char color[4];
    }
    arg;

    /** \brief Holds one of the possible sources of an event */
    union
    {
        /** \brief A pointer to the widget that caused the event */
        sgui_widget* widget;

        /** \brief A pointer to the window that caused the event */
        sgui_window* window;

        /** \brief A source other than a widget or a window */
        void* other;
    }
    src;

    /** \brief Event type identifyer (an \ref SGUI_EVENT_TYPE value) */
    int type;
};



typedef enum
{
    SGUI_EVENT = 0x10,  /**< \brief function( obj, &event ) */
    SGUI_WIDGET = 0x11, /**< \brief function( obj, src.widget ) */
    SGUI_WINDOW = 0x12, /**< \brief function( obj, src.window ) */
    SGUI_TYPE = 0x13,   /**< \brief function( obj, type ) */
    SGUI_I = 0x14,      /**< \brief function( obj, i ) */
    SGUI_I2_X = 0x15,   /**< \brief function( obj, i2.x ) */
    SGUI_I2_Y = 0x16,   /**< \brief function( obj, i2.y ) */
    SGUI_I2_XY = 0x17,  /**< \brief function( obj, i2.x, i2.y ) */
    SGUI_I2_YX = 0x18,  /**< \brief function( obj, i2.y, i2.x ) */
    SGUI_I3_X = 0x19,   /**< \brief function( obj, i3.x ) */
    SGUI_I3_Y = 0x1A,   /**< \brief function( obj, i3.y ) */
    SGUI_I3_Z = 0x1B,   /**< \brief function( obj, i3.z ) */
    SGUI_I3_XY = 0x1C,  /**< \brief function( obj, i3.x, i3.y ) */
    SGUI_I3_XZ = 0x1D,  /**< \brief function( obj, i3.x, i3.z ) */
    SGUI_I3_YX = 0x1E,  /**< \brief function( obj, i3.y, i3.x ) */
    SGUI_I3_YZ = 0x1F,  /**< \brief function( obj, i3.y, i3.z ) */
    SGUI_I3_ZX = 0x20,  /**< \brief function( obj, i3.z, i3.x ) */
    SGUI_I3_ZY = 0x21,  /**< \brief function( obj, i3.z, i3.y ) */
    SGUI_I3_XYZ = 0x22, /**< \brief function( obj, i3.x, i3.y, i3.z ) */
    SGUI_I3_XZY = 0x23, /**< \brief function( obj, i3.x, i3.z, i3.y ) */
    SGUI_I3_YXZ = 0x24, /**< \brief function( obj, i3.y, i3.x, i3.z ) */
    SGUI_I3_YZX = 0x25, /**< \brief function( obj, i3.x, i3.z, i3.x ) */
    SGUI_I3_ZXY = 0x26, /**< \brief function( obj, i3.z, i3.x, i3.y ) */
    SGUI_I3_ZYX = 0x27, /**< \brief function( obj, i3.z, i3.y, i3.x ) */
    SGUI_UI2_X = 0x28,  /**< \brief function( obj, ui2.x ) */
    SGUI_UI2_Y = 0x29,  /**< \brief function( obj, ui2.y ) */
    SGUI_UI2_XY = 0x2A, /**< \brief function( obj, ui2.x, ui2.y ) */
    SGUI_UI2_YX = 0x2B, /**< \brief function( obj, ui2.y, ui2.x ) */
    SGUI_UTF8 = 0x2C,   /**< \brief function( obj, arg.utf8 ) */
    SGUI_RECT = 0x2D,   /**< \brief function( obj, &arg.rect ) */
    SGUI_COLOR = 0x2E   /**< \brief function( obj, arg.color ) */
}
SGUI_EVENT_ARG;

typedef enum
{
    /** \brief No further arguments are passed to the callback */
    SGUI_VOID = 0x00,

    /** \brief A char argument is passed to the callback */
    SGUI_CHAR = 0x01,

    /** \brief A short argument is passed to the callback */
    SGUI_SHORT = 0x02,

    /** \brief An int argument is passed to the callback */
    SGUI_INT = 0x03,

    /** \brief Two int arguments are passed to the callback */
    SGUI_INT2 = 0x04,

    /** \brief Three int arguments are passed to the callback */
    SGUI_INT3 = 0x06,

    /** \brief A long argument is passed to the callback */
    SGUI_LONG = 0x07,

    /** \brief A pointer argument is passed to the callback */
    SGUI_POINTER = 0x08,

    /** \brief A float argument is passed to the callback */
    SGUI_FLOAT = 0x0A,

    /** \brief A double argument is passed to the callback */
    SGUI_DOUBLE = 0x0B,

    /** \brief A value from the event structure is passed to the callback */
    SGUI_FROM_EVENT = 0x10
}
SGUI_ARG_TYPE;



/**
 * \brief A generic function pointer type
 *
 * \param object A pointer to the object instance ("this pointer")
 * \param ...    Possible arguments to the function
 */
typedef void (* sgui_function )( void* object, ... );


/**
 * \struct sgui_event_queue
 *
 * \brief Manages a queue of events and event to event-handler mappings
 */
struct sgui_event_queue {
	/** \brief Destroy an event queue and free all allocated memory */
	void (*destroy)(sgui_event_queue *queue);
};


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Create an event queue
 *
 * \memberof sgui_event_queue
 *
 * \return A pointer to an \ref sgui_event_queue on success, NULL on failure
 */
sgui_event_queue *sgui_event_queue_create(void);

/**
 * \brief Connect an event with a callback
 *
 * \memberof sgui_event_queue
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
 * \param ev        The event queue in which to store the mapping
 * \param sender    A pointer to the sender object, or NULL for any
 * \param eventtype The event identifyer to listen to
 * \param ...       A pointer to a callback function, followed by a pointer to
 *                  the receiver object and an \ref SGUI_ARG_TYPE value
 *                  specifying the type of argument. If it is not SGUI_VOID,
 *                  one or more immediate value folow. If it is
 *                  SGUI_FROM_EVENT, an SGUI_EVENT_ARG value follows.
 *
 * \return Non-zero on success, zero if out of memory
 */
SGUI_DLL int sgui_event_connect(sgui_event_queue *ev,
				void *sender, int eventtype, ...);

/**
 * \brief Disconnect an event from a callback
 *
 * \memberof sgui_event_queue
 *
 * Given a sender, event type, receiver and callback, this function runs
 * through the internal list of event connections and disconnects all matching
 * connections.
 *
 * \param ev        The event queue that this mapping was created for
 * \param sender    A pointer to the sender object
 * \param eventtype The event identifyer to listen to
 * \param callback  A pointer to a callback function
 * \param receiver  A pointer to the receiver object
 */
SGUI_DLL void sgui_event_disconnect(sgui_event_queue *ev, void *sender,
					int eventtype, sgui_function callback,
					void *receiver);

/**
 * \brief Post an event to an \ref sgui_event_queue
 *
 * \memberof sgui_event_queue
 *
 * \param ev The event queue to post the event to
 * \param event A pointer to an event structure
 *
 * \return Non-zero on success, zero if out of memory
 */
SGUI_DLL int sgui_event_post(sgui_event_queue *ev, const sgui_event *event);

/**
 * \brief Get the number of events still waiting for processing
 *
 * \memberof sgui_event_queue
 *
 * \param ev The event queue to query
 *
 * \return Number of unprocessed events in the queue
 */
SGUI_DLL unsigned int sgui_event_queued(sgui_event_queue *ev);

/**
 * \brief Process the queued events and forward them to handlers
 *
 * \memberof sgui_event_queue
 *
 * \param ev A pointer to an event queue
 */
SGUI_DLL void sgui_event_process(sgui_event_queue *ev);

#ifdef __cplusplus
}
#endif

#endif /* SGUI_EVENT_H */


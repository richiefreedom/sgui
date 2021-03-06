/*
 * sgui_context.h
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
 * \file sgui_context.h
 *
 * \brief Contains the declarations of the abstract rendering
 *        context structure
 */
#ifndef SGUI_CONTEXT_H
#define SGUI_CONTEXT_H



#include "sgui_predef.h"



/**
 * \interface sgui_context
 *
 * \brief An abstract hardware accelerated rendering context
 *
 * \image html gldemo.png "A window with an OpenGL&reg; context"
 */
struct sgui_context
{
    /**
     * \brief Destroy a rendering context
     *
     * \param ctx A pointer to a context object
     */
    void (* destroy )( sgui_context* ctx );

    /**
     * \brief Create a rendering context that shares resources with an
     *        existing context
     *
     * \param ctx A pointer to a context for which to create an offscreen,
     *            resource sharing context
     *
     * \return A pointer to a rendering context object
     */
    sgui_context* (* create_share )( sgui_context* ctx );

    /**
     * \brief Make a rendering context current
     *
     * \note Can be NULL if not implemented
     *
     * Some rendering systems have the concept of a "context", that has to be
     * made current in order to be used (e.g. OpenGL). A context can only be
     * current in exactely one thread at any time and one thread may only have
     * exactely one context made current at any time.
     *
     * If the underlying rendering system has the concept of making contexts
     * current, this function binds the context to the calling thread and the
     * given window.
     *
     * \param ctx A pointer to a context object
     * \param wnd A pointer to a rendering window to bind the context to
     */
    void (* make_current )( sgui_context* ctx, sgui_window* wnd );

    /**
     * \brief Release a context, assuming it is current in the calling thread
     *
     * \node Can be NULL if not implemented
     *
     * \param ctx A pointer to a context previously made current in
     *            the calling thread
     */
    void (* release_current )( sgui_context* ctx );

    /**
     * \brief Load an extension function pointer from a rendering context
     *
     * \note Can be NULL if not implemented
     *
     * \param ctx  A pointer to a context object
     * \param name The name of the function
     *
     * \return Either a pointer to the function or NULL on failure
     */
    sgui_funptr (* load )( sgui_context* ctx, const char* name );

    /**
     * \brief Get a pointer to the internally used data structure
     *
     * For OpenGL contexts, this returns a pointer to the platform dependend
     * context handle. For Direct3D contexts, this returns a pointer to the
     * underlying device object.
     *
     * \param ctx A pointer to a context object
     *
     * \return A pointer to an internal object or NULL if the context is NULL
     */
    void* (* get_internal )( sgui_context* ctx );
};

#endif /* SGUI_CONTEXT_H */


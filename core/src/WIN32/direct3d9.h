/*
 * direct3d9.h
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
#ifndef SGUI_DIRECT3D9_H
#define SGUI_DIRECT3D9_H

#include "sgui_context.h"
#include "sgui_internal.h"

#ifndef SGUI_NO_D3D9
    #include "sgui_d3d9.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* create a Direct3D 9 context context */
sgui_context* d3d9_context_create( sgui_window* wnd,
                                   const sgui_window_description* desc );

void send_event_if_d3d9_lost( sgui_window* wnd );

#ifdef __cplusplus
}
#endif

#endif /* SGUI_DIRECT3D9_H */


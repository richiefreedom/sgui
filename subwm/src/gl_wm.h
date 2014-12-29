/*
 * gl_wm.h
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
#ifndef GL_WM_H
#define GL_WM_H



#include "sgui_subwm_predef.h"
#include "sgui_ctx_window.h"
#include "sgui_internal.h"
#include "sgui_ctx_wm.h"
#include "sgui_event.h"
#include "sgui_skin.h"



#ifndef SGUI_NO_OPENGL
#ifdef SGUI_WINDOWS
    #include <windows.h>
#endif

#ifdef SGUI_OS_X
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#ifndef GL_VERSION_1_5
    typedef ptrdiff_t GLsizeiptr;
    typedef ptrdiff_t GLintptr;
#endif

#ifndef GL_VERSION_2_0
    typedef char GLchar;
#endif

#undef GLAPIENTRY
#undef GL_FRAGMENT_SHADER
#undef GL_VERTEX_SHADER
#undef GL_ARRAY_BUFFER
#undef GL_ELEMENT_ARRAY_BUFFER
#undef GL_STATIC_DRAW
#undef GL_STREAM_DRAW
#undef GL_TEXTURE0
#undef GL_ACTIVE_TEXTURE
#undef GL_CURRENT_PROGRAM

#ifdef APIENTRY
    #define GLAPIENTRY APIENTRY
#endif
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_STREAM_DRAW 0x88E0
#define GL_TEXTURE0 0x84C0
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_CURRENT_PROGRAM 0x8B8D


#define GLWM_CORE_MAX_WINDOWS 10



typedef GLuint (GLAPIENTRY * GLCREATESHADERPROC) (GLenum type);
typedef void (GLAPIENTRY * GLLINKPROGRAMPROC) (GLuint program);
typedef GLuint (GLAPIENTRY * GLCREATEPROGRAMPROC) (void);
typedef void (GLAPIENTRY * GLCOMPILESHADERPROC) (GLuint shader);
typedef void (GLAPIENTRY * GLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (GLAPIENTRY * GLSHADERSOURCEPROC) (GLuint shader,
                                                GLsizei count,
                                                const GLchar** string,
                                                const GLint* length);
typedef void (GLAPIENTRY * GLATTACHSHADERPROC) (GLuint program,
                                                GLuint shader);
typedef void (GLAPIENTRY * GLGENVERTEXARRAYSPROC) (GLsizei n,
                                                   GLuint* arrays);
typedef void (GLAPIENTRY * GLGETPROGRAMINFOLOGPROC) (GLuint program,
                                                     GLsizei bufSize,
                                                     GLsizei* length,
                                                     GLchar* infoLog);
typedef void (GLAPIENTRY * GLGETSHADERINFOLOGPROC) (GLuint shader,
                                                    GLsizei bufSize,
                                                    GLsizei* length,
                                                    GLchar* infoLog);

typedef void (GLAPIENTRY * GLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (GLAPIENTRY * GLBINDBUFFERPROC) (GLenum target, GLuint buffer);

typedef void (GLAPIENTRY * GLBINDFRAGDATALOCATIONPROC) (GLuint program,
                                                        GLuint colorNumber,
                                                        const GLchar* name);
typedef void (GLAPIENTRY * GLBINDATTRIBLOCATIONPROC) (GLuint program,
                                                      GLuint index,
                                                      const GLchar* name);
typedef void (GLAPIENTRY * GLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (GLAPIENTRY * GLVERTEXATTRIBPOINTERPROC) (GLuint index,
                                                       GLint size,
                                                       GLenum type,
                                                       GLboolean normalized,
                                                       GLsizei stride,
                                                       const void* pointer);
typedef GLint (GLAPIENTRY * GLGETUNIFORMLOCATIONPROC) (GLuint program,
                                                       const GLchar* name);
typedef void (GLAPIENTRY * GLBUFFERDATAPROC) (GLenum target, GLsizeiptr size,
                                              const void* data, GLenum usage);
typedef void (GLAPIENTRY * GLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (GLAPIENTRY * GLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (GLAPIENTRY * GLDRAWELEMENTSBASEVERTEXPROC) (GLenum mode,
                                                          GLsizei count,
                                                          GLenum type,
                                                          const void *indices,
                                                          GLint basevertex);
typedef void (GLAPIENTRY * GLBUFFERSUBDATAPROC) (GLenum target,
                                                 GLintptr offset,
                                                 GLsizeiptr size,
                                                 const void* data);
typedef void (GLAPIENTRY * GLUSEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * GLACTIVETEXTUREPROC) (GLenum texture);
typedef void (GLAPIENTRY * GLDELETEBUFFERSPROC) (GLsizei n,
                                                 const GLuint* buffers);
typedef void (GLAPIENTRY * GLDELETESHADERPROC) (GLuint shader);
typedef void (GLAPIENTRY * GLDELETEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * GLDELETEVERTEXARRAYSPROC) (GLsizei n,
                                                      const GLuint* arrays);
typedef void (GLAPIENTRY * GLUNIFORMMATRIX4FVPROC) (GLint location,
                                                    GLsizei count,
                                                    GLboolean transpose,
                                                    const GLfloat* value);




typedef struct
{
    GLUNIFORM1FPROC Uniform1f;
    GLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
    GLBINDVERTEXARRAYPROC BindVertexArray;
    GLBUFFERSUBDATAPROC BufferSubData;
    GLDRAWELEMENTSBASEVERTEXPROC DrawElementsBaseVertex;
    GLUSEPROGRAMPROC UseProgram;
    GLACTIVETEXTUREPROC ActiveTexture;
}
sgui_gl_functions;

typedef struct
{
    sgui_ctx_wm super;

    GLuint wndtex;
}
sgui_gl_wm;

typedef struct
{
    sgui_gl_wm super;

    GLint u_alpha;      /**< \brief Window transparency uniform location */
    GLint u_mvp;        /**< \brief Scaling matrix uniform location */

    GLuint vao;         /**< \brief VAO with window VBO and IBO bound */
    GLuint buffers[2];  /**< \brief Buffer objects */

    GLuint vsh;         /**< \brief vertex shader for drawing windows */
    GLuint fsh;         /**< \brief fragment shader for drawing windows */
    GLuint prog;        /**< \brief shader program  for drawing windows */

    sgui_gl_functions gl;
}
sgui_gl_core_wm;

typedef struct
{
    GLint view[4];      /**< \brief Viewport data */
    GLint tex[2];       /**< \brief Texture bindings */
    GLint activetex;    /**< \brief The active texture unit */
    GLint program;      /**< \brief Active shader program */
    GLint blending;     /**< \brief Is blending enabled? */
    GLint depth_test;   /**< \brief Is depth test enabled? */
    GLint depth_write;  /**< \brief Is depth write enabled? */
    GLint blend_src;    /**< \brief Source blend factor */
    GLint blend_dst;    /**< \brief Destination blend factor */
}
gl_state;



#ifdef __cplusplus
extern "C" {
#endif

sgui_ctx_wm* gl_wm_create( sgui_window* wnd );

sgui_ctx_wm* gl_wm_create_core( sgui_window* wnd );

#ifdef __cplusplus
}
#endif

#endif /* SGUI_NO_OPENGL */

#endif /* GL_WM_H */


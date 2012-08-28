/*
 * sgui_canvas.h
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
#ifndef SGUI_CANVAS_H
#define SGUI_CANVAS_H



#ifdef __cplusplus
extern "C"
{
#endif



typedef enum
{
    SCF_RGB8 = 0,
    SCF_RGBA8,

    SCF_BGR8,
    SCF_BGRA8
}
SGUI_COLOR_FORMAT;



typedef struct sgui_canvas sgui_canvas;
typedef struct sgui_font   sgui_font;



/**
 * \brief Initialise the font rasterisation subsystem
 *
 * \note Call this before any other sgui_font_* function!
 *
 * \return non-zero on success, zero on failure.
 */
int sgui_font_init( void );

/**
 * \brief Uninitialise the font rasterisation subsystem
 *
 * \note Call this after you're done with font rasterisation and do not call
 *       any other sgui_font_* function afterwards!
 */
void sgui_font_deinit( void );

/**
 * \brief Load a font face from a file using STDIO
 *
 * \param filename The path to the font file
 *
 * \returns A font object
 */
sgui_font* sgui_font_load_from_file( const char* filename );

/**
 * \brief Load a font from a memory buffer
 *
 * This is usefull when loading fonts from other resources than the standard
 * file system (e.g. custom virtual filesystems like physicsFS et al.)
 *
 * \note This function takes ownership of the given buffer. Do NOT delete it
 *       on your own afterwards.
 *
 * \param buffer     Pointer to a buffer containing the loaded font file data
 * \param buffersize The size of the buffer in bytes
 *
 * \returns A font object
 */
sgui_font* sgui_font_load_from_mem( void* buffer, unsigned int buffersize );

/** \brief Destroy a font object */
void sgui_font_destroy( sgui_font* font );

/**
 * \brief Get the with of a single line of text, in a single font face,
 *        in pixels
 *
 * \param font_face   The font face to use
 * \param font_height The height of the font in pixels
 * \param text        The UTF8 text to determine the rendered width of
 * \param length      The number of bytes to read
 */
unsigned int sgui_font_get_text_extents_plain( sgui_font* font_face,
                                               unsigned int font_height,
                                               const unsigned char* text,
                                               unsigned int length );

/**
 * \brief Get the with and height of a multi line text that uses html like
 *        tags to determine color and font face
 *
 * \see sgui_canvas_draw_text
 *
 * \param font_norm   The font face to use for normal text
 * \param font_bold   The font face to use for bold text
 * \param font_ital   The font face to use for italic text
 * \param font_boit   The font face to use for bold and italic text
 * \param font_height The height of the font in pixels
 * \param text        The UTF8 text to determine the rendered size of
 * \param width       Returns the width of the rendered text
 * \param height      Returns the height of the rendererd text
 */
void sgui_font_get_text_extents( sgui_font* font_norm, sgui_font* font_bold,
                                 sgui_font* font_ital, sgui_font* font_boit,
                                 unsigned int font_height,
                                 const unsigned char* text,
                                 unsigned int* width, unsigned int* height );






/**
 * \brief Create a canvas object
 *
 * \param width  The widht of the canvas
 * \param height The height of the canvas
 * \param format The color format of the canvas
 *
 * \return A pointer to a new canvas object
 */
sgui_canvas* sgui_canvas_create( unsigned int width, unsigned int height,
                                 SGUI_COLOR_FORMAT format );

/**
 * \brief Create a canvas object that uses a memory buffer, ownership of the
 *        buffer remains with the caller
 *
 * \param buffer The buffer to use
 * \param width  The widht of the canvas
 * \param height The height of the canvas
 * \param format The color format of the buffer
 *
 * \return A pointer to a new canvas object
 */
sgui_canvas* sgui_canvas_create_use_buffer( void* buffer,
                                            unsigned int width,
                                            unsigned int height,
                                            SGUI_COLOR_FORMAT format );

/** \brief Destroy a canvas object */
void sgui_canvas_destroy( sgui_canvas* canvas );

/**
 * \brief Get the size of a canvas
 *
 * \param canvas The canvas to report the size of
 * \param width  Returns the width of the canvas
 * \param height Returns the height of the canvas
 */
void sgui_canvas_get_size( sgui_canvas* canvas, unsigned int* width,
                           unsigned int* height );

/**
 * \brief Resize a canvas that does not use an external buffer, losing all
 *        previous contents of the canvas.
 *
 * \param width  The new width of the canvas
 * \param height The new height of the canvas
 */
void sgui_canvas_resize( sgui_canvas* canvas, unsigned int width,
                         unsigned int height );



/**
 * \brief Set the scissor rect of a canvas
 *
 * Rendering to a canvas is only allowed inside the scissor rect, everything
 * outside will be clipped.
 *
 * To reset the scissor rect to default, simply set all parameters to zero.
 *
 * \param x      The distance from the left of the scissor rect to the
 *               left of the canvas.
 * \param y      The distance from the top of the scissor rect to the
 *               top of the canvas.
 * \param width  The width of the scissor rect.
 * \param height The height of the scissor rect.
 */
void sgui_canvas_set_scissor_rect( sgui_canvas* canvas, int x, int y,
                                   unsigned int width, unsigned int height );

/**
 * \brief Blit an image onto a canvas
 *
 * \param x      Distance from the left of the canvas to the left of the image
 * \param y      Distance from the top of the canvas to the top of the image
 * \param width  Width of the image
 * \param height Width of the image
 * \param format Color format of the image
 * \param data   Pointer to the image data
 */
void sgui_canvas_blit( sgui_canvas* canvas, int x, int y, unsigned int width,
                       unsigned int height, SGUI_COLOR_FORMAT format,
                       const void* data );

/**
 * \brief Blend an image onto a canvas
 *
 * \param x      Distance from the left of the canvas to the left of the image
 * \param y      Distance from the top of the canvas to the top of the image
 * \param width  Width of the image
 * \param height Width of the image
 * \param format Color format of the image (must be a format with an alpha
 *               channel)
 * \param data   Pointer to the image data
 */
void sgui_canvas_blend( sgui_canvas* canvas, int x, int y, unsigned int width,
                        unsigned int height, SGUI_COLOR_FORMAT format,
                        const void* data );

/**
 * \brief Draw a rectangle onto a canvas
 *
 * \param x      Distance from the left of the canvas to the left of the box
 * \param y      Distance from the top of the canvas to the top of the box
 * \param width  The width of the box
 * \param height The height of the box
 * \param color  The color to draw the box in
 * \param format The color format stored in the color array
 */
void sgui_canvas_draw_box( sgui_canvas* canvas, int x, int y,
                           unsigned int width, unsigned int height,
                           unsigned char* color, SGUI_COLOR_FORMAT format );

/**
 * \brief Draw a line onto a canvas
 *
 * \param x          Distance from the left of the canvas to the start of the
 *                   line.
 * \param y          Distance from the top of the canvas to the start of the
 *                   line.
 * \param length     The length of the line in the desired direction
 * \param horizontal Non-zero to draw a horizontal line, zero to draw a
 *                   vertical line.
 * \param color      The color to draw the line in
 * \param format     The color format stored in the color array
 */
void sgui_canvas_draw_line( sgui_canvas* canvas, int x, int y,
                            unsigned int length, int horizontal,
                            unsigned char* color, SGUI_COLOR_FORMAT format );

/**
 * \brief Render one line of text in a single font face
 *
 * \param x         Distance from the left of the canvas to the left of the
 *                  text.
 * \param y         Distance from the top of the canvas to the top of the
 *                  text.
 * \param font_face The font face to use.
 * \param height    The height of the text in pixels.
 * \param color     The color to draw the text in.
 * \param format    The format of the given color.
 * \param text      The text to draw.
 * \param length    The number of bytes to read from the text.
 */
void sgui_canvas_draw_text_plain( sgui_canvas* canvas, int x, int y,
                                  sgui_font* font_face, unsigned int height,
                                  unsigned char* color,
                                  SGUI_COLOR_FORMAT format,
                                  const unsigned char* text,
                                  unsigned int length );

/**
 * \brief Render a multi line text that uses html like tags to switch color
 *        or font_face
 *
 * \param x           Distance from the left of the text to the left of the
 *                    canvas.
 * \param y           Distance from the top of the text to the top of the
 *                    canvas.
 * \param font_norm   The font face to use for normal text.
 * \param font_bold   The font face to use for bold text.
 * \param font_ital   The font face to use for italic text.
 * \param font_boit   The font face to use for bold and italic text.
 * \param font_height The height of the font faces in pixels
 * \param color       The default color to use.
 * \param format      The format of the given default color
 * \param text        The UTF8 text to print. The LF ('\n') character can be
 *                    used for line wraps, the \<b\> \</b\> and \<i\> \</i\>
 *                    for writing text bold or italic. A \<color="#RRGGBB"\>
 *                    tag can be used to switch text color, where the value
 *                    "default" for color switches back to default color.
 */
void sgui_canvas_draw_text( sgui_canvas* canvas, int x, int y,
                            sgui_font* font_norm, sgui_font* font_bold,
                            sgui_font* font_ital, sgui_font* font_boit,
                            unsigned int font_height, unsigned char* color,
                            SGUI_COLOR_FORMAT format,
                            const unsigned char* text );

/** \brief Get a pointer to the raw data buffer of a canvas */
void* sgui_canvas_get_raw_data( sgui_canvas* canvas );

/**
 * \brief Set a buffer for a canvas to render to
 *
 * \param format The color format to use
 * \param width  The width of the canvas
 * \param height The height of the canvas
 * \param data   A pointer to the buffer to use
 */
void sgui_canvas_set_raw_data( sgui_canvas* canvas, SGUI_COLOR_FORMAT format,
                               unsigned int width, unsigned int height,
                               void* data );



#ifdef __cplusplus
}
#endif

#endif /* SGUI_CANVAS_H */

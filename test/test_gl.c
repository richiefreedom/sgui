#include "sgui.h"
#include "sgui_opengl.h"

#include <GL/gl.h>
#include <stdio.h>


sgui_canvas* cv;
sgui_widget_manager* mgr;


/* translates window to canvas coordinates */
void translate_pos( int* x, int* y )
{
    *x -= 800 * 0.2f;
    *y -= 600 * 0.1f;

    *x = (256*(*x)) / (800.0f * 0.6f);
    *y = (256*(*y)) / (600.0f * 0.8f);
}

void sgui_window_fun( sgui_window* wnd, int type, sgui_event* event )
{
    sgui_event e;
    (void)wnd;

    if( event )
        e = *event;

    if( (type == SGUI_MOUSE_RELEASE_EVENT) ||
        (type == SGUI_MOUSE_PRESS_EVENT) )
    {
        translate_pos( &e.mouse_press.x, &e.mouse_press.y );
    }
    else if( type == SGUI_MOUSE_MOVE_EVENT )
    {
        translate_pos( &e.mouse_move.x, &e.mouse_move.y );
    }

    sgui_widget_manager_send_window_event( mgr, type, &e );
}

int main( void )
{
    sgui_window* wnd;
    GLuint tex;
    unsigned char color[4];
    sgui_widget *pbar, *c0, *c1, *c2, *butt;
    sgui_font *font, *font_bold, *font_ital, *font_boit;

    /* create window */
    sgui_init( );

    wnd = sgui_window_create( 800, 600, 0, SGUI_OPENGL_COMPAT );

    sgui_window_set_title( wnd, "sgui OpenGL test" );
    sgui_window_on_event( wnd, sgui_window_fun );
    sgui_window_set_visible( wnd, 1 );

    sgui_window_make_current( wnd );

    glViewport( 0, 0, 800, 600 );

    font     =sgui_font_load("../../font/SourceSansPro-Regular.ttf",16);
    font_bold=sgui_font_load("../../font/SourceSansPro-Semibold.ttf",16);
    font_ital=sgui_font_load("../../font/SourceSansPro-It.ttf",16);
    font_boit=sgui_font_load("../../font/SourceSansPro-SemiboldIt.ttf",16);

    sgui_skin_set_default_font( font, font_bold, font_ital, font_boit );

    /* create canvas and screen */
    cv = sgui_opengl_canvas_create( 256, 256 );
    mgr = sgui_widget_manager_create( );

    sgui_skin_get_window_background_color( color );
    sgui_canvas_set_background_color( cv, color );
    sgui_canvas_clear( cv, NULL );

    pbar = sgui_progress_bar_create( 10, 10, SGUI_PROGRESS_BAR_STIPPLED, 0,
                                     50, 200 );

    c0 = sgui_checkbox_create( 10,  50, "Checkbox 1" );
    c1 = sgui_checkbox_create( 10,  75, "Checkbox 2" );
    c2 = sgui_checkbox_create( 10, 100, "Checkbox 3" );

    butt = sgui_button_create( 10, 150, 100, 30, "Button" );

    sgui_widget_manager_add_widget( mgr, pbar );
    sgui_widget_manager_add_widget( mgr, butt );
    sgui_widget_manager_add_widget( mgr, c0 );
    sgui_widget_manager_add_widget( mgr, c1 );
    sgui_widget_manager_add_widget( mgr, c2 );

    /* bind Canvas texture */
    tex = sgui_opengl_canvas_get_texture( cv );

    glBindTexture( GL_TEXTURE_2D, tex );
    glEnable( GL_TEXTURE_2D );

    /* print OpenGL info */
    printf( "%s\n%s\nOpenGL %s\n",
            glGetString( GL_VENDOR ),
            glGetString( GL_RENDERER ),
            glGetString( GL_VERSION ) );

    /* main loop */
    while( sgui_main_loop_step( ) )
    {
        sgui_widget_manager_draw( mgr, cv );
        sgui_widget_manager_clear_dirty_rects( mgr );

        glClear( GL_COLOR_BUFFER_BIT );

        glBegin( GL_QUADS );
        glVertex2f( -0.6f,  0.8f ); glTexCoord2f( 1.0f, 0.0f );
        glVertex2f(  0.6f,  0.8f ); glTexCoord2f( 1.0f, 1.0f );
        glVertex2f(  0.6f, -0.8f ); glTexCoord2f( 0.0f, 1.0f );
        glVertex2f( -0.6f, -0.8f ); glTexCoord2f( 0.0f, 0.0f );
        glEnd( );

        sgui_window_swap_buffers( wnd );
    }

    /* cleanup */
    sgui_canvas_destroy( cv );
    sgui_window_make_current( NULL );
    sgui_window_destroy( wnd );
    sgui_widget_manager_destroy( mgr );
    sgui_button_destroy( butt );
    sgui_button_destroy( c0 );
    sgui_button_destroy( c1 );
    sgui_button_destroy( c2 );
    sgui_progress_bar_destroy( pbar );
    sgui_font_destroy( font );
    sgui_font_destroy( font_bold );
    sgui_font_destroy( font_ital );
    sgui_font_destroy( font_boit );

    sgui_deinit( );

    return 0;
}


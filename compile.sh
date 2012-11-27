#!/bin/sh

# Flags for the C compiler
CFLAGS="-ansi -pedantic -Werror -Wall -Wextra -Wshadow -Wwrite-strings
        -I./include -ggdb"

# Compile a list of files. $1: additional compiler flags, $2: list of files,
#                          $3: object directory, $4: object prefix
#
# Only compiles files if the coresponding object file does not exist or is
# "older" (lower modification time stamp) than the source file
compile_files( )
{
    for f in $2
    do
        obj="build/obj/$3/$4$(basename "$f" .c).o"
        objtime="0"
        srctime=$(stat -c %Y $f)

        # If the object file exists, get the last modification timestamp
        if [ -e $obj ]; then
            objtime=$(stat -c %Y $obj)
        fi

        # compile if object file is older than source code file
        if [ $objtime -lt $srctime ]; then
            echo -e "\e[32m$f\e[0m"
            gcc $CFLAGS $1 -c $f -o $obj

            # On error, exit
            if [ $? -ne 0 ]; then
                exit 1
            fi
        fi
    done
}

# Create a static library from object files. $1: object dir, $2: library name
#
# Simply glues all object files (*.o) within a source directory together to a
# single static library.
create_library( )
{
    echo -e "\e[31m***** creating static library $2 *****\e[0m"
    cur=$(pwd)              # store working directory
    cd build/obj/$1         # go to object file directory
    ar rcs $2 $(ls *.o)     # "ar" all *.o files to an archive
    ranlib $2               # run "ranlib" on the resulting archive
    cd $cur                 # go back to stored working directory
}

############################# source code files #############################

# Common source code files
SOURCE_WIDGETS="src/widgets/progress_bar.c src/widgets/static_text.c
                src/widgets/button.c src/widgets/image.c
                src/widgets/edit_box.c src/widgets/frame.c
                src/widgets/scroll_bar.c src/widgets/group_box.c
                src/widgets/tab.c"

SOURCE_OPENGL="src/OpenGL/canvas_gl_tex.c"

SOURCE_COMMON="src/widget.c src/font.c src/rect.c src/widget_manager.c
               src/skin.c src/filesystem.c src/canvas.c src/window.c
               src/screen.c $SOURCE_WIDGETS $SOURCE_OPENGL"

# Platform specific stuff
SOURCE_X11="src/X11/window.c src/X11/keycode_translate.c src/X11/canvas.c
            src/X11/platform.c src/X11/opengl.c"

INCLUDE_X11="-I/usr/include -I/usr/include/freetype2"
LIBS_X11="-lX11 -lfreetype -lGL"

# Test application sources
SOURCE_TEST="test/test.c test/test_gl.c"

############################# Do the compilation #############################

# compile test apps. $1: additional flags, $2: obj dir, $3: app postfix
compile_tests( )
{
    echo -e "\e[31m***** compiling test and demo programs *****\e[0m"

    compile_files "$1" "$SOURCE_TEST" "$2/test"

    list=$(ls build/obj/$2/test/*.o)

    for f in $list
    do
        name=$(basename $f ".o")

        echo -e "\e[31m***** linking $name$3 *****\e[0m"
        gcc $1 $f -Lbuild/obj/$2 -lsgui -o build/$name$3

        # On error, exit
        if [ $? -ne 0 ]; then
            exit 1
        fi
    done
}

# Do it
if [ ! -f ./compile.sh ]; then
    echo -e "\e[01;31mERROR\e[0m script must be run from it's directory!"
else
    mkdir -p "build/obj/unix32/test"
    mkdir -p "build/obj/unix64/test"

    ######### unix32/X11 #########
    echo -e "\e[0m * Compiling for x86, UNIX like system"

    compile_files "-m32 $INCLUDE_X11" "$SOURCE_COMMON" "unix32"
    compile_files "-m32 $INCLUDE_X11" "$SOURCE_X11"    "unix32" "x11_"

    create_library "unix32" "libsgui.a"

    compile_tests "-m32 $LIBS_X11" "unix32" "_unix32"

    ######### unix64/X11 #########
    echo -e "\e[0m * Compiling for x86_64, UNIX like system"

    compile_files "-m64 $INCLUDE_X11" "$SOURCE_COMMON" "unix64"
    compile_files "-m64 $INCLUDE_X11" "$SOURCE_X11"    "unix64" "x11_"

    create_library "unix64" "libsgui.a"

    compile_tests "-m64 $LIBS_X11" "unix64" "_unix64"
fi


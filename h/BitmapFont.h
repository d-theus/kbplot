#pragma once

#ifndef BITMAP_FONT_H
#define BITMAP_FONT_H
#include <stdio.h>			// Header File For Standard Input/Output
#include <stdarg.h>			// Header File For Variable Argument Routines
#include "gl_helper.h"

class BitmapFont
{
public:
    BitmapFont(HDC hDC = NULL);
    ~BitmapFont(void);

    GLvoid BuildFont(size_t);				// Build Our Bitmap Font
    GLvoid KillFont(GLvoid);				// Delete The Font List
    GLvoid Print(GLfloat x, GLfloat y, const char *fmt, ...);	// Custom GL "Print" Routine

private:
    // FONT
    GLuint	base_;				// Base Display List For The Font Set
    HDC hDC_;
};

#endif//BITMAP_FONT_H

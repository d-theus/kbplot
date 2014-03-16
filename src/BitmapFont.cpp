#include "BitmapFont.h"


BitmapFont::BitmapFont(HDC hDC) : hDC_(hDC)
{
}


BitmapFont::~BitmapFont(void)
{
	KillFont();
}

// настройка шрифта и указание списка символов для вывода текста с помощью win api
GLvoid BitmapFont::BuildFont(size_t size)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base_ = glGenLists(256);								// Storage For 96 Characters

	font = CreateFont(	-(int)size,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_REGULAR,						// Font Weight
						FALSE,							// Italic-курсив
						FALSE,							// Underline
						FALSE,							// Strikeout
						RUSSIAN_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						L"Courier New");				// Font Name

	oldfont = (HFONT)SelectObject(hDC_, font);           // Selects The Font We Want
	wglUseFontBitmapsA(hDC_, 0, 256, base_);			 // Builds 96 Characters Starting At Character 32
	SelectObject(hDC_, oldfont);						// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

GLvoid BitmapFont::KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base_, 256);							// Delete All 96 Characters
}

GLvoid BitmapFont::Print(GLfloat x, GLfloat y, const char *fmt, ...)					// Custom GL "Print" Routine
{
	glRasterPos2f(x, y);

	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)						// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
		vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base_);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}


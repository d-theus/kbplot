// Подключаем algorithm (для swap)
#include <algorithm>

// Подключаем WinAPI, при этом дружим Gdiplus и C++Builder
#define STRICT
#include <windows.h>

// Подключаем OpenGL
#include <GL/GL.h>

// Подключаем Gdiplus:
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


// Объявление основных функций
static void ReadGLBuff(const size_t width, const size_t height, BYTE *rgbBuff);
static void SaveBuffToFile(const wchar_t* filename, const size_t width, const size_t height, const BYTE *rgbBuff);

/*
Входная функция.
*/
void SaveScreenShot(const wchar_t* filename, const size_t width, const size_t height)
{
    BYTE *buff = new BYTE[width * height * 3];
    ReadGLBuff(width, height, buff);
    SaveBuffToFile(filename, width, height, buff);
    delete[] buff;
}

/*
Функция чтения буффера кадра.
*/
static void ReadGLBuff(const size_t width, const size_t height, BYTE *rgbBuff)
{
    // Будем читать из заднего буффера
    glReadBuffer(GL_BACK);

    // Не выравниваем длину прочитаной строки
    // (выравниваем по 1 байту)
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    // Читаем буффер
    glReadPixels(0, 0, (GLsizei) width, (GLsizei) height, GL_RGB, GL_UNSIGNED_BYTE, rgbBuff);
}

/*
Функция сохранения в файл.
*/
static void SaveBuffToFile(const wchar_t* filename, const size_t width, const size_t height, const BYTE *rgbBuff)
{
    // Объявление вспомогательных функций
    size_t alignUp(const size_t alignment, const size_t n);
    int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);


    // Размер в байтах одной строки в Bitmap-е
    const size_t bmpStride = alignUp(4, width * 3);

    // Буффер для создания Bitmap-а
    BYTE *bmpBuff = new BYTE[bmpStride * height];

    // Заполняем буффер для создания Bitmap-а:
    // конфертируем RGB в BGR и инвертируем порядок строк.
    for(size_t y = 0; y < height; y++)
    {
        BYTE *bmpPtr = bmpBuff + (height - y) * bmpStride;
        const BYTE *rgbPtr = rgbBuff + y * width * 3;

        // Копируем строку
        memcpy(bmpPtr, rgbPtr, width * 3);

        // Конфертируем RGB в BGR
        for(size_t x = 0; x < width; x++)
        {
            std::swap(bmpPtr[0], bmpPtr[2]);
            bmpPtr += 3;
        }
    }

    // Инициализируем GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Получаем CLSID кодировщика PNG
    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);

    // Создаём Bitmap из подготовленного буффера и сохраняем его в файл
    Bitmap((INT) width, (INT) height, (INT) bmpStride, PixelFormat24bppRGB, bmpBuff)
    .Save(filename, &pngClsid, NULL);

    // Выключаем GDI+
    GdiplusShutdown(gdiplusToken);

    delete[] bmpBuff;
}


/*
Вспомогательная функция получения CLSID кодировщика

Из http://msdn.microsoft.com/en-us/library/ms533843(v=vs.85).aspx
*/
static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if(size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}


/*
Вспомогательные функции выравнивания
*/
static size_t alignDown(const size_t alignment, const size_t n)
{
    return n & ~(alignment - 1); // alignment - 1: 0b11, 0b111, ...
}

static size_t alignUp(const size_t alignment, const size_t n)
{
    return alignDown(alignment, n + alignment - 1);
}

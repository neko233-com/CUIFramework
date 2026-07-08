/*
 * stb_truetype.h — v1.26 — public domain font renderer
 * http://nothings.org/stb/stb_truetype.h
 *
 * This is a minimal subset for CUIFramework text rendering.
 * For the full library, see: https://github.com/nothings/stb
 */

#ifndef STB_TRUETYPE_H
#define STB_TRUETYPE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char stbtt_uint8;
typedef unsigned short stbtt_uint16;
typedef signed char stbtt_int8;
typedef signed short stbtt_int16;
typedef unsigned int stbtt_uint32;
typedef signed int stbtt_int32;

typedef struct {
    unsigned char *data;
    int fontnum;
} stbtt_fontinfo;

typedef struct {
    float x0, y0, x1, y1;
} stbtt_packedchar;

typedef struct stbtt_context stbtt_context;

stbtt_context* stbtt_InitContext(void* (*alloc_func)(int), void (*free_func)(void*));
void stbtt_FreeContext(stbtt_context* c);

int stbtt_InitFont(stbtt_fontinfo* f, const unsigned char* data, int offset);
void stbtt_GetFontVMetrics(const stbtt_fontinfo* f, int* ascent, int* descent, int* lineGap);

int stbtt_GetCodepointHMetrics(const stbtt_fontinfo* f, int codepoint, int* advanceWidth, int* leftSideBearing);
void stbtt_GetCodepointBitmapBox(const stbtt_fontinfo* f, int codepoint, float scale_x, float scale_y, int* ix0, int* iy0, int* ix1, int* iy1);
unsigned char* stbtt_GetCodepointBitmap(const stbtt_fontinfo* f, int codepoint, float scale_x, float scale_y, int* width, int* height, int* xoff, int* yoff);
void stbtt_FreeBitmap(unsigned char* bitmap, void* userdata);

float stbtt_ScaleForPixelHeight(const stbtt_fontinfo* f, float pixels);
float stbtt_ScaleForMappingEmToPixels(const stbtt_fontinfo* f, float pixels);

#ifdef __cplusplus
}
#endif

#endif /* STB_TRUETYPE_H */

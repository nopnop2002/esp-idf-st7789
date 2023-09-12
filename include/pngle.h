/*-
 * MIT License
 *
 * Copyright (c) 2019 kikuchan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __PNGLE_H__
#define __PNGLE_H__

#include <stdint.h>
#include <stdbool.h>
#include "miniz.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifdef PNGLE_DEBUG
#define debug_printf(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug_printf(...) ((void)0)
#endif

//#define PNGLE_NO_GAMMA_CORRECTION

#if 0
typedef struct __attribute__((__packed__)) {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_png;
#endif

//rgb565 format
typedef uint16_t pixel_png;

typedef enum {
    PNGLE_STATE_ERROR = -2,
    PNGLE_STATE_EOF = -1,
    PNGLE_STATE_INITIAL = 0,

    PNGLE_STATE_FIND_CHUNK_HEADER,
    PNGLE_STATE_HANDLE_CHUNK,
    PNGLE_STATE_CRC,
} pngle_state_t;


typedef enum {
// Supported chunks
//   Filter chunk names by following command to (re)generate hex constants;
//     % perl -ne 'chomp; s/.*\s*\/\/\s*//; print "\tPNGLE_CHUNK_$_ = 0x" . unpack("H*") . "UL, // $_\n";'
    PNGLE_CHUNK_IHDR = 0x49484452UL, // IHDR
    PNGLE_CHUNK_PLTE = 0x504c5445UL, // PLTE
    PNGLE_CHUNK_IDAT = 0x49444154UL, // IDAT
    PNGLE_CHUNK_IEND = 0x49454e44UL, // IEND
    PNGLE_CHUNK_tRNS = 0x74524e53UL, // tRNS
    PNGLE_CHUNK_gAMA = 0x67414d41UL, // gAMA
} pngle_chunk_t;

typedef struct _pngle_ihdr_t {
	uint32_t width;
	uint32_t height;
	uint8_t depth;
	uint8_t color_type;
	uint8_t compression;
	uint8_t filter;
	uint8_t interlace;
} pngle_ihdr_t;

typedef unsigned int	UINT;

typedef struct pngle pngle_t;

typedef void (*pngle_init_callback_t)(pngle_t *pngle, uint32_t w, uint32_t h);
typedef void (*pngle_draw_callback_t)(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4]);
typedef void (*pngle_done_callback_t)(pngle_t *pngle);

struct pngle {
    pngle_ihdr_t hdr;

    uint_fast8_t channels; // 0 indicates IHDR hasn't been processed yet

    // PLTE chunk
    size_t n_palettes;
    uint8_t *palette;

    // tRNS chunk
    size_t n_trans_palettes;
    uint8_t *trans_palette;

    // parser state (reset on every chunk header)
    pngle_state_t state;
    uint32_t chunk_type;
    uint32_t chunk_remain;
    mz_ulong crc32;

    // decompression state (reset on IHDR)
    tinfl_decompressor inflator; // 11000 bytes
    uint8_t lz_buf[TINFL_LZ_DICT_SIZE]; // 32768 bytes
    uint8_t *next_out; // NULL indicates IDAT hasn't been processed yet
    size_t  avail_out;

    // scanline decoder (reset on every set_interlace_pass() call)
    uint8_t *scanline_ringbuf;
    size_t scanline_ringbuf_size;
    size_t scanline_ringbuf_cidx;
    int_fast8_t scanline_remain_bytes_to_render;
    int_fast8_t filter_type;
    uint32_t drawing_x;
    uint32_t drawing_y;

    // interlace
    uint_fast8_t interlace_pass;

    const char *error;

#ifndef PNGLE_NO_GAMMA_CORRECTION
    uint8_t *gamma_table;
    double display_gamma;
#endif

    pngle_init_callback_t init_callback;
    pngle_draw_callback_t draw_callback;
    pngle_done_callback_t done_callback;

    void *user_data;
	uint16_t screenWidth;
	uint16_t screenHeight;
	uint16_t imageWidth;
	uint16_t imageHeight;
	pixel_png **pixels;
	bool reduction;
	double scale_factor;
};


// ----------------
// Basic interfaces
// ----------------
pngle_t *pngle_new(uint16_t width, uint16_t height);
void pngle_destroy(pngle_t *pngle, uint16_t width, uint16_t height);
void pngle_reset(pngle_t *pngle); // clear its internal state (not applied to pngle_set_* functions)
const char *pngle_error(pngle_t *pngle);
int pngle_feed(pngle_t *pngle, const void *buf, size_t len); // returns -1: On error, 0: Need more data, n: n bytes eaten

uint32_t pngle_get_width(pngle_t *pngle);
uint32_t pngle_get_height(pngle_t *pngle);

void pngle_set_init_callback(pngle_t *png, pngle_init_callback_t callback);
void pngle_set_draw_callback(pngle_t *png, pngle_draw_callback_t callback);
void pngle_set_done_callback(pngle_t *png, pngle_done_callback_t callback);

void pngle_set_display_gamma(pngle_t *pngle, double display_gamma); // enables gamma correction by specifying display gamma, typically 2.2. No effect when gAMA chunk is missing

void pngle_set_user_data(pngle_t *pngle, void *user_data);
void *pngle_get_user_data(pngle_t *pngle);


// Get IHDR information
pngle_ihdr_t *pngle_get_ihdr(pngle_t *pngle);


#ifdef __cplusplus
}
#endif

#endif /* __PNGLE_H__ */

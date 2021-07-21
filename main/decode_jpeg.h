#pragma once
#include <stdint.h>
#include "esp_err.h"

#if 0
typedef struct __attribute__((__packed__)) {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_jpeg;
#endif

//rgb565 format
typedef uint16_t pixel_jpeg;

/**
 * @brief Decode the jpeg ``image.jpg`` embedded into the program file into pixel data.
 *
 * @param pixels A pointer to a pointer for an array of rows, which themselves are an array of pixels.
 *        Effectively, you can get the pixel data by doing ``decode_jpeg(&myPixels); pixelval=myPixels[ypos][xpos];``
 * @return - ESP_ERR_NOT_SUPPORTED if image is malformed or a progressive jpeg file
 *         - ESP_ERR_NO_MEM if out of memory
 *         - ESP_OK on succesful decode
 */

esp_err_t decode_jpeg(pixel_jpeg ***pixels, char * file, uint16_t width, uint16_t height, uint16_t * imageWidth, uint16_t * imageHeight);

/**
 * @brief Release image memory.
 *
 */

esp_err_t release_image(pixel_jpeg ***pixels, uint16_t width, uint16_t height);


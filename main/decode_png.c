#include <stdio.h>
#include <inttypes.h>
#include "decode_png.h"
#include "pngle.h"
#include "esp_log.h"

void png_init(pngle_t *pngle, uint32_t w, uint32_t h)
{
	ESP_LOGD(__FUNCTION__, "png_init w=%"PRIu32" h=%"PRIu32, w, h);
	ESP_LOGD(__FUNCTION__, "screenWidth=%d screenHeight=%d", pngle->screenWidth, pngle->screenHeight);
	pngle->imageWidth = w;
	pngle->imageHeight = h;
	pngle->reduction = false;
	pngle->scale_factor = 1.0;

	// Calculate Reduction
	if (pngle->screenWidth < pngle->imageWidth || pngle->screenHeight < pngle->imageHeight) {
		pngle->reduction = true;
		double factorWidth = (double)pngle->screenWidth / (double)pngle->imageWidth;
		double factorHeight = (double)pngle->screenHeight / (double)pngle->imageHeight;
		pngle->scale_factor = factorWidth;
		if (factorHeight < factorWidth) pngle->scale_factor = factorHeight;
		pngle->imageWidth = pngle->imageWidth * pngle->scale_factor;
		pngle->imageHeight = pngle->imageHeight * pngle->scale_factor;
	}
	ESP_LOGD(__FUNCTION__, "reduction=%d scale_factor=%f", pngle->reduction, pngle->scale_factor);
	ESP_LOGD(__FUNCTION__, "imageWidth=%d imageHeight=%d", pngle->imageWidth, pngle->imageHeight);
		
}

#define rgb565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))

void png_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
	ESP_LOGD(__FUNCTION__, "png_draw x=%"PRIu32" y=%"PRIu32" w=%"PRIu32" h=%"PRIu32, x,y,w,h);
#if 0
	uint8_t r = rgba[0];
	uint8_t g = rgba[1];
	uint8_t b = rgba[2];
#endif

	// image reduction
	uint32_t _x = x;
	uint32_t _y = y;
	if (pngle->reduction) {
		_x = x * pngle->scale_factor;
		_y = y * pngle->scale_factor;
	}
	if (_y < pngle->screenHeight && _x < pngle->screenWidth) {
#if 0
		pngle->pixels[_y][_x].red = rgba[0];
		pngle->pixels[_y][_x].green = rgba[1];
		pngle->pixels[_y][_x].blue = rgba[2];
#endif
		pngle->pixels[_y][_x] = rgb565(rgba[0], rgba[1], rgba[2]);
	}

}

void png_finish(pngle_t *pngle) {
	ESP_LOGD(__FUNCTION__, "png_finish");
}

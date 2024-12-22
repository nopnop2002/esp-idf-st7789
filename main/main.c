#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"

#include "st7789.h"
#include "fontx.h"
#include "bmpfile.h"
#include "decode_jpeg.h"
#include "decode_png.h"
#include "pngle.h"

#define INTERVAL 400
#define WAIT vTaskDelay(INTERVAL)

static const char *TAG = "ST7789";

// You have to set these CONFIG value using menuconfig.
#if 0
#define CONFIG_WIDTH  240
#define CONFIG_HEIGHT 240
#define CONFIG_MOSI_GPIO 23
#define CONFIG_SCLK_GPIO 18
#define CONFIG_CS_GPIO -1
#define CONFIG_DC_GPIO 19
#define CONFIG_RESET_GPIO 15
#define CONFIG_BL_GPIO -1
#endif

void traceHeap() {
	static uint32_t _free_heap_size = 0;
	if (_free_heap_size == 0) _free_heap_size = esp_get_free_heap_size();

	int _diff_free_heap_size = _free_heap_size - esp_get_free_heap_size();
	ESP_LOGI(__FUNCTION__, "_diff_free_heap_size=%d", _diff_free_heap_size);

	printf("esp_get_free_heap_size() : %6"PRIu32"\n", esp_get_free_heap_size() );
#if 0
	printf("esp_get_minimum_free_heap_size() : %6"PRIu32"\n", esp_get_minimum_free_heap_size() );
	printf("xPortGetFreeHeapSize() : %6zd\n", xPortGetFreeHeapSize() );
	printf("xPortGetMinimumEverFreeHeapSize() : %6zd\n", xPortGetMinimumEverFreeHeapSize() );
	printf("heap_caps_get_free_size(MALLOC_CAP_32BIT) : %6d\n", heap_caps_get_free_size(MALLOC_CAP_32BIT) );
	// that is the amount of stack that remained unused when the task stack was at its greatest (deepest) value.
	printf("uxTaskGetStackHighWaterMark() : %6d\n", uxTaskGetStackHighWaterMark(NULL));
#endif
}

TickType_t FillTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdFillScreen(dev, RED);
	lcdDrawFinish(dev);
	vTaskDelay(50);
	lcdFillScreen(dev, GREEN);
	lcdDrawFinish(dev);
	vTaskDelay(50);
	lcdFillScreen(dev, BLUE);
	lcdDrawFinish(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t ColorBarTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	if (width < height) {
		uint16_t y1,y2;
		y1 = height/3;
		y2 = (height/3)*2;
		lcdDrawFillRect(dev, 0, 0, width-1, y1-1, RED);
		vTaskDelay(1);
		lcdDrawFillRect(dev, 0, y1, width-1, y2-1, GREEN);
		vTaskDelay(1);
		lcdDrawFillRect(dev, 0, y2, width-1, height-1, BLUE);
		lcdDrawFinish(dev);
	} else {
		uint16_t x1,x2;
		x1 = width/3;
		x2 = (width/3)*2;
		lcdDrawFillRect(dev, 0, 0, x1-1, height-1, RED);
		vTaskDelay(1);
		lcdDrawFillRect(dev, x1, 0, x2-1, height-1, GREEN);
		vTaskDelay(1);
		lcdDrawFillRect(dev, x2, 0, width-1, height-1, BLUE);
		lcdDrawFinish(dev);
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t ArrowTest(TFT_t * dev, FontxFile *fx, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%d fontHeight=%d",fontWidth,fontHeight);
	
	uint16_t xpos;
	uint16_t ypos;
	int	stlen;
	uint8_t ascii[24];
	uint16_t color;

	lcdFillScreen(dev, BLACK);

	strcpy((char *)ascii, "ST7789");
	if (width < height) {
		xpos = ((width - fontHeight) / 2) - 1;
		ypos = (height - (strlen((char *)ascii) * fontWidth)) / 2;
		lcdSetFontDirection(dev, DIRECTION90);
	} else {
		ypos = ((height - fontHeight) / 2) - 1;
		xpos = (width - (strlen((char *)ascii) * fontWidth)) / 2;
		lcdSetFontDirection(dev, DIRECTION0);
	}
	color = WHITE;
	lcdDrawString(dev, fx, xpos, ypos, ascii, color);

	lcdSetFontDirection(dev, 0);
	color = RED;
	lcdDrawFillArrow(dev, 10, 10, 0, 0, 5, color);
	strcpy((char *)ascii, "0,0");
	lcdDrawString(dev, fx, 0, 30, ascii, color);

	color = GREEN;
	lcdDrawFillArrow(dev, width-11, 10, width-1, 0, 5, color);
	//strcpy((char *)ascii, "79,0");
	sprintf((char *)ascii, "%d,0",width-1);
	stlen = strlen((char *)ascii);
	xpos = (width-1) - (fontWidth*stlen);
	lcdDrawString(dev, fx, xpos, 30, ascii, color);

	color = GRAY;
	lcdDrawFillArrow(dev, 10, height-11, 0, height-1, 5, color);
	//strcpy((char *)ascii, "0,159");
	sprintf((char *)ascii, "0,%d",height-1);
	ypos = (height-11) - (fontHeight) + 5;
	lcdDrawString(dev, fx, 0, ypos, ascii, color);

	color = CYAN;
	lcdDrawFillArrow(dev, width-11, height-11, width-1, height-1, 5, color);
	//strcpy((char *)ascii, "79,159");
	sprintf((char *)ascii, "%d,%d",width-1, height-1);
	stlen = strlen((char *)ascii);
	xpos = (width-1) - (fontWidth*stlen);
	lcdDrawString(dev, fx, xpos, ypos, ascii, color);
	lcdDrawFinish(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t DirectionTest(TFT_t * dev, FontxFile *fx, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%d fontHeight=%d",fontWidth,fontHeight);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	uint8_t ascii[20];

	color = RED;
	strcpy((char *)ascii, "Direction=0");
	lcdSetFontDirection(dev, 0);
	lcdDrawString(dev, fx, 0, fontHeight-1, ascii, color);

	color = BLUE;
	strcpy((char *)ascii, "Direction=2");
	lcdSetFontDirection(dev, 2);
	lcdDrawString(dev, fx, (width-1), (height-1)-(fontHeight*1), ascii, color);

	color = CYAN;
	strcpy((char *)ascii, "Direction=1");
	lcdSetFontDirection(dev, 1);
	lcdDrawString(dev, fx, (width-1)-fontHeight, 0, ascii, color);

	color = GREEN;
	strcpy((char *)ascii, "Direction=3");
	lcdSetFontDirection(dev, 3);
	lcdDrawString(dev, fx, (fontHeight-1), height-1, ascii, color);
	lcdDrawFinish(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t HorizontalTest(TFT_t * dev, FontxFile *fx, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%d fontHeight=%d",fontWidth,fontHeight);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	uint8_t ascii[20];

	color = RED;
	strcpy((char *)ascii, "Direction=0");
	lcdSetFontDirection(dev, 0);
	lcdDrawString(dev, fx, 0, fontHeight*1-1, ascii, color);
	lcdSetFontUnderLine(dev, RED);
	lcdDrawString(dev, fx, 0, fontHeight*2-1, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, GREEN);
	lcdDrawString(dev, fx, 0, fontHeight*3-1, ascii, color);
	lcdSetFontUnderLine(dev, RED);
	lcdDrawString(dev, fx, 0, fontHeight*4-1, ascii, color);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	color = BLUE;
	strcpy((char *)ascii, "Direction=2");
	lcdSetFontDirection(dev, 2);
	lcdDrawString(dev, fx, width, height-(fontHeight*1)-1, ascii, color);
	lcdSetFontUnderLine(dev, BLUE);
	lcdDrawString(dev, fx, width, height-(fontHeight*2)-1, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, YELLOW);
	lcdDrawString(dev, fx, width, height-(fontHeight*3)-1, ascii, color);
	lcdSetFontUnderLine(dev, BLUE);
	lcdDrawString(dev, fx, width, height-(fontHeight*4)-1, ascii, color);
	lcdDrawFinish(dev);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t VerticalTest(TFT_t * dev, FontxFile *fx, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%d fontHeight=%d",fontWidth,fontHeight);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	uint8_t ascii[20];

	color = RED;
	strcpy((char *)ascii, "Direction=1");
	lcdSetFontDirection(dev, 1);
	lcdDrawString(dev, fx, width-(fontHeight*1), 0, ascii, color);
	lcdSetFontUnderLine(dev, RED);
	lcdDrawString(dev, fx, width-(fontHeight*2), 0, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, GREEN);
	lcdDrawString(dev, fx, width-(fontHeight*3), 0, ascii, color);
	lcdSetFontUnderLine(dev, RED);
	lcdDrawString(dev, fx, width-(fontHeight*4), 0, ascii, color);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	color = BLUE;
	strcpy((char *)ascii, "Direction=3");
	lcdSetFontDirection(dev, 3);
	lcdDrawString(dev, fx, (fontHeight*1)-1, height, ascii, color);
	lcdSetFontUnderLine(dev, BLUE);
	lcdDrawString(dev, fx, (fontHeight*2)-1, height, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, YELLOW);
	lcdDrawString(dev, fx, (fontHeight*3)-1, height, ascii, color);
	lcdSetFontUnderLine(dev, BLUE);
	lcdDrawString(dev, fx, (fontHeight*4)-1, height, ascii, color);
	lcdDrawFinish(dev);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}


TickType_t LineTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	//lcdFillScreen(dev, WHITE);
	lcdFillScreen(dev, BLACK);
	color=RED;
	for(int ypos=0;ypos<height;ypos=ypos+10) {
		lcdDrawLine(dev, 0, ypos, width, ypos, color);
	}

	for(int xpos=0;xpos<width;xpos=xpos+10) {
		lcdDrawLine(dev, xpos, 0, xpos, height, color);
	}
	lcdDrawFinish(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t CircleTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	//lcdFillScreen(dev, WHITE);
	lcdFillScreen(dev, BLACK);
	color = CYAN;
	uint16_t xpos = width/2;
	uint16_t ypos = height/2;
	for(int i=5;i<height;i=i+5) {
		lcdDrawCircle(dev, xpos, ypos, i, color);
	}
	lcdDrawFinish(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t RectAngleTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	//lcdFillScreen(dev, WHITE);
	lcdFillScreen(dev, BLACK);
	color = CYAN;
	uint16_t xpos = width/2;
	uint16_t ypos = height/2;

	uint16_t w = width * 0.6;
	uint16_t h = w * 0.5;
	int angle;
	for(angle=0;angle<=(360*3);angle=angle+30) {
		lcdDrawRectAngle(dev, xpos, ypos, w, h, angle, color);
		usleep(10000);
		lcdDrawRectAngle(dev, xpos, ypos, w, h, angle, BLACK);
	}

	for(angle=0;angle<=180;angle=angle+30) {
		lcdDrawRectAngle(dev, xpos, ypos, w, h, angle, color);
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t TriangleTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	//lcdFillScreen(dev, WHITE);
	lcdFillScreen(dev, BLACK);
	color = CYAN;
	uint16_t xpos = width/2;
	uint16_t ypos = height/2;

	uint16_t w = width * 0.6;
	uint16_t h = w * 1.0;
	int angle;

	for(angle=0;angle<=(360*3);angle=angle+30) {
		lcdDrawTriangle(dev, xpos, ypos, w, h, angle, color);
		usleep(10000);
		lcdDrawTriangle(dev, xpos, ypos, w, h, angle, BLACK);
	}

	for(angle=0;angle<=360;angle=angle+30) {
		lcdDrawTriangle(dev, xpos, ypos, w, h, angle, color);
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t RoundRectTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();


	uint16_t color;
	uint16_t limit = width;
	if (width > height) limit = height;
	//lcdFillScreen(dev, WHITE);
	lcdFillScreen(dev, BLACK);
	color = BLUE;
	for(int i=5;i<limit;i=i+5) {
		if (i > (limit-i-1) ) break;
		//ESP_LOGI(__FUNCTION__, "i=%d, width-i-1=%d",i, width-i-1);
		lcdDrawRoundRect(dev, i, i, (width-i-1), (height-i-1), 10, color);
	}
	lcdDrawFinish(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t FillRectTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	lcdFillScreen(dev, CYAN);

	uint16_t red;
	uint16_t green;
	uint16_t blue;
	srand( (unsigned int)time( NULL ) );
	for(int i=1;i<100;i++) {
		red=rand()%255;
		green=rand()%255;
		blue=rand()%255;
		color=rgb565(red, green, blue);
		uint16_t xpos=rand()%width;
		uint16_t ypos=rand()%height;
		uint16_t size=rand()%(width/5);
		lcdDrawFillRect(dev, xpos, ypos, xpos+size, ypos+size, color);
	}
	lcdDrawFinish(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t ColorTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	lcdFillScreen(dev, WHITE);
	color = RED;
	uint16_t delta = height/16;
	uint16_t ypos = 0;
	for(int i=0;i<16;i++) {
		//ESP_LOGI(__FUNCTION__, "color=0x%x",color);
		lcdDrawFillRect(dev, 0, ypos, width-1, ypos+delta, color);
		color = color >> 1;
		ypos = ypos + delta;
	}
	lcdDrawFinish(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}


TickType_t BMPTest(TFT_t * dev, char * file, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdSetFontDirection(dev, 0);
	lcdFillScreen(dev, BLACK);

	// allocate memory
	bmpfile_t *bmpfile = (bmpfile_t*)malloc(sizeof(bmpfile_t));
	if (bmpfile == NULL) {
		ESP_LOGE(__FUNCTION__, "Error allocating memory for bmpfile");
		return 0;
	}

	// open requested file
	esp_err_t ret;
	FILE* fp = fopen(file, "rb");
	if (fp == NULL) {
		ESP_LOGW(__FUNCTION__, "File not found [%s]", file);
		return 0;
	}

	// read bmp header
	ret = fread(bmpfile->header.magic, 1, 2, fp);
	assert(ret == 2);
	if (bmpfile->header.magic[0]!='B' || bmpfile->header.magic[1] != 'M') {
		ESP_LOGW(__FUNCTION__, "File is not BMP");
		free(bmpfile);
		fclose(fp);
		return 0;
	}
	ret = fread(&bmpfile->header.filesz, 4, 1 , fp);
	assert(ret == 1);
	ESP_LOGD(__FUNCTION__,"bmpfile->header.filesz=%"PRIu32, bmpfile->header.filesz);
	ret = fread(&bmpfile->header.creator1, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->header.creator2, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->header.offset, 4, 1, fp);
	assert(ret == 1);

	// read dib header
	ret = fread(&bmpfile->dib.header_sz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.width, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.height, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.nplanes, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.depth, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.compress_type, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.bmp_bytesz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.hres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.vres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.ncolors, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.nimpcolors, 4, 1, fp);
	assert(ret == 1);

	if((bmpfile->dib.depth == 24) && (bmpfile->dib.compress_type == 0)) {
		// BMP rows are padded (if needed) to 4-byte boundary
		uint32_t rowSize = (bmpfile->dib.width * 3 + 3) & ~3;
		int w = bmpfile->dib.width;
		int h = bmpfile->dib.height;
		ESP_LOGD(__FUNCTION__,"w=%d h=%d", w, h);
		int _x;
		int _w;
		int _cols;
		int _cole;
		if (width >= w) {
			_x = (width - w) / 2;
			_w = w;
			_cols = 0;
			_cole = w - 1;
		} else {
			_x = 0;
			_w = width;
			_cols = (w - width) / 2;
			_cole = _cols + width - 1;
		}
		ESP_LOGD(__FUNCTION__,"_x=%d _w=%d _cols=%d _cole=%d",_x, _w, _cols, _cole);

		int _y;
		int _rows;
		int _rowe;
		if (height >= h) {
			_y = (height - h) / 2;
			_rows = 0;
			_rowe = h -1;
		} else {
			_y = 0;
			_rows = (h - height) / 2;
			_rowe = _rows + height - 1;
		}
		ESP_LOGD(__FUNCTION__,"_y=%d _rows=%d _rowe=%d", _y, _rows, _rowe);

#define BUFFPIXEL 20
		uint8_t sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
		uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * w);
		if (colors == NULL) {
			ESP_LOGE(__FUNCTION__, "Error allocating memory for color");
			free(bmpfile);
			fclose(fp);
			return 0;
		}

		for (int row=0; row<h; row++) { // For each scanline...
			if (row < _rows || row > _rowe) continue;
			// Seek to start of scan line.	It might seem labor-
			// intensive to be doing this on every line, but this
			// method covers a lot of gritty details like cropping
			// and scanline padding.  Also, the seek only takes
			// place if the file position actually needs to change
			// (avoids a lot of cluster math in SD library).
			// Bitmap is stored bottom-to-top order (normal BMP)
			int pos = bmpfile->header.offset + (h - 1 - row) * rowSize;
			fseek(fp, pos, SEEK_SET);
			int buffidx = sizeof(sdbuffer); // Force buffer reload

			int index = 0;
			for (int col=0; col<w; col++) { // For each pixel...
				if (buffidx >= sizeof(sdbuffer)) { // Indeed
					fread(sdbuffer, sizeof(sdbuffer), 1, fp);
					buffidx = 0; // Set index to beginning
				}
				if (col < _cols || col > _cole) continue;
				// Convert pixel from BMP to TFT format, push to display
				uint8_t b = sdbuffer[buffidx++];
				uint8_t g = sdbuffer[buffidx++];
				uint8_t r = sdbuffer[buffidx++];
				colors[index++] = rgb565(r, g, b);
			} // end for col
			ESP_LOGD(__FUNCTION__,"lcdDrawMultiPixels _x=%d _y=%d row=%d",_x, _y, row);
			//lcdDrawMultiPixels(dev, _x, row+_y, _w, colors);
			lcdDrawMultiPixels(dev, _x, _y, _w, colors);
			_y++;
		} // end for row
		free(colors);
	} // end if
	lcdDrawFinish(dev);
	free(bmpfile);
	fclose(fp);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t QRTest(TFT_t * dev, char * file, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdSetFontDirection(dev, 0);
	lcdFillScreen(dev, BLACK);

	// allocate memory
	bmpfile_t *bmpfile = (bmpfile_t*)malloc(sizeof(bmpfile_t));
	if (bmpfile == NULL) {
		ESP_LOGE(__FUNCTION__, "Error allocating memory for bmpfile");
		return 0;
	}

	// open requested file
	esp_err_t ret;
	FILE* fp = fopen(file, "rb");
	if (fp == NULL) {
		ESP_LOGW(__FUNCTION__, "File not found [%s]", file);
		return 0;
	}

	// read bmp header
	ret = fread(bmpfile->header.magic, 1, 2, fp);
	assert(ret == 2);
	if (bmpfile->header.magic[0]!='B' || bmpfile->header.magic[1] != 'M') {
		ESP_LOGW(__FUNCTION__, "File is not BMP");
		free(bmpfile);
		fclose(fp);
		return 0;
	}
	ret = fread(&bmpfile->header.filesz, 4, 1 , fp);
	assert(ret == 1);
	ESP_LOGD(__FUNCTION__,"bmpfile->header.filesz=%"PRIu32, bmpfile->header.filesz);
	ret = fread(&bmpfile->header.creator1, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->header.creator2, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->header.offset, 4, 1, fp);
	assert(ret == 1);

	// read dib header
	ret = fread(&bmpfile->dib.header_sz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.width, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.height, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.nplanes, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.depth, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.compress_type, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.bmp_bytesz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.hres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.vres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.ncolors, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&bmpfile->dib.nimpcolors, 4, 1, fp);
	assert(ret == 1);

	ESP_LOGD(__FUNCTION__, "dib.depth=%d dib.compress_type=%"PRIu32, bmpfile->dib.depth, bmpfile->dib.compress_type);
	//if((bmpfile->dib.depth == 24) && (bmpfile->dib.compress_type == 0)) {
	if((bmpfile->dib.depth == 1) && (bmpfile->dib.compress_type == 0)) {
		ESP_LOGD(__FUNCTION__, "dib.bmp_bytesz=%"PRIu32, bmpfile->dib.bmp_bytesz);
		// BMP rows are padded (if needed) to 4-byte boundary
		//uint32_t rowSize = (bmpfile->dib.width * 3 + 3) & ~3;
		int w = bmpfile->dib.width;
		int h = bmpfile->dib.height;
		uint32_t rowSize = bmpfile->dib.bmp_bytesz / bmpfile->dib.height;
		ESP_LOGD(__FUNCTION__,"dib.width=%"PRIu32" dib.height=%"PRIu32" rowSize=%"PRIu32, bmpfile->dib.width, bmpfile->dib.height, rowSize);
		int _x;
		int _w;
		int _cols;
		int _cole;
		if (width >= w) {
			_x = (width - w) / 2;
			_w = w;
			_cols = 0;
			_cole = w - 1;
		} else {
			_x = 0;
			_w = width;
			_cols = (w - width) / 2;
			_cole = _cols + width - 1;
		}
		ESP_LOGD(__FUNCTION__,"_x=%d _w=%d _cols=%d _cole=%d",_x, _w, _cols, _cole);

		int _y;
		int _rows;
		int _rowe;
		if (height >= h) {
			_y = (height - h) / 2;
			_rows = 0;
			_rowe = h -1;
		} else {
			_y = 0;
			_rows = (h - height) / 2;
			_rowe = _rows + height - 1;
		}
		ESP_LOGD(__FUNCTION__,"_y=%d _rows=%d _rowe=%d", _y, _rows, _rowe);

		uint8_t *sdbuffer = (uint8_t*)malloc(rowSize); // pixel buffer
		if (sdbuffer == NULL) {
			ESP_LOGE(__FUNCTION__, "Error allocating memory for sdbuffer");
			free(bmpfile);
			fclose(fp);
			return 0;
		}
		uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * _w); // tft buffer
		if (colors == NULL) {
			ESP_LOGE(__FUNCTION__, "Error allocating memory for colors");
			free(bmpfile);
			free(sdbuffer);
			fclose(fp);
			return 0;
		}

		int debug = 0; // number of logging output
		for (int row=0; row<h; row++) { // For each scanline...
			if (row < _rows || row > _rowe) continue;
			// Seek to start of scan line.	It might seem labor-
			// intensive to be doing this on every line, but this
			// method covers a lot of gritty details like cropping
			// and scanline padding.  Also, the seek only takes
			// place if the file position actually needs to change
			// (avoids a lot of cluster math in SD library).
			// Bitmap is stored bottom-to-top order (normal BMP)
			int pos = bmpfile->header.offset + (h - 1 - row) * rowSize;
			ESP_LOGD(__FUNCTION__,"pos=%d 0x%x", pos, pos);
			fseek(fp, pos, SEEK_SET);
			fread(sdbuffer, rowSize, 1, fp);
			int buffidx = 0;
			if (debug > 0) {
				ESP_LOGI(__FUNCTION__, "sdbuffer");
				ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, sdbuffer, rowSize, ESP_LOG_INFO);
			}

			//int buffidx = sizeof(sdbuffer); // Force buffer reload

			int index = 0;
			uint8_t mask = 0x80;
			for (int col=0; col<w; col++) { // For each pixel...
				if (col < _cols || col > _cole) continue;
				// Convert pixel from BMP to TFT format, push to display
				colors[index] = BLACK;
				if ( (sdbuffer[buffidx] & mask) != 0) colors[index] = WHITE;
				index++;
				mask = mask >> 1;
				if (mask == 0x00) {
					buffidx++;
					mask = 0x80;
				}

				//uint8_t b = sdbuffer[buffidx++];
				//uint8_t g = sdbuffer[buffidx++];
				//uint8_t r = sdbuffer[buffidx++];
				//colors[index++] = rgb565(r, g, b);
			} // end for col
			ESP_LOGD(__FUNCTION__,"lcdDrawMultiPixels _x=%d _y=%d row=%d",_x, _y, row);
			if (debug > 0) {
				ESP_LOGI(__FUNCTION__, "colors");
				ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, colors, _w*2, ESP_LOG_INFO);
			}
			lcdDrawMultiPixels(dev, _x, _y, _w, colors);
			debug--;
			_y++;
		} // end for row
		free(sdbuffer);
		free(colors);
	} // end if
	lcdDrawFinish(dev);
	free(bmpfile);
	fclose(fp);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t JPEGTest(TFT_t * dev, char * file, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdSetFontDirection(dev, 0);
	lcdFillScreen(dev, BLACK);


	pixel_jpeg **pixels;
	int imageWidth;
	int imageHeight;
	esp_err_t err = decode_jpeg(&pixels, file, width, height, &imageWidth, &imageHeight);
	ESP_LOGD(__FUNCTION__, "decode_image err=%d imageWidth=%d imageHeight=%d", err, imageWidth, imageHeight);
	if (err == ESP_OK) {

		uint16_t _width = width;
		uint16_t _cols = 0;
		if (width > imageWidth) {
			_width = imageWidth;
			_cols = (width - imageWidth) / 2;
		}
		ESP_LOGD(__FUNCTION__, "_width=%d _cols=%d", _width, _cols);

		uint16_t _height = height;
		uint16_t _rows = 0;
		if (height > imageHeight) {
			_height = imageHeight;
			_rows = (height - imageHeight) / 2;
		}
		ESP_LOGD(__FUNCTION__, "_height=%d _rows=%d", _height, _rows);

		// allocate memory
		uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * _width);
		if (colors == NULL) {
			ESP_LOGE(__FUNCTION__, "Error allocating memory for colors");
			return 0;
		}

#if 0
		for(int y = 0; y < _height; y++){
			for(int x = 0;x < _width; x++){
				pixel_jpeg pixel = pixels[y][x];
				uint16_t color = rgb565(pixel.red, pixel.green, pixel.blue);
				lcdDrawPixel(dev, x+_cols, y+_rows, color);
			}
			vTaskDelay(1);
		}
#endif

		for(int y = 0; y < _height; y++){
			for(int x = 0;x < _width; x++){
				//pixel_jpeg pixel = pixels[y][x];
				//colors[x] = rgb565(pixel.red, pixel.green, pixel.blue);
				colors[x] = pixels[y][x];
			}
			lcdDrawMultiPixels(dev, _cols, y+_rows, _width, colors);
			vTaskDelay(1);
		}

		lcdDrawFinish(dev);
		free(colors);
		release_image(&pixels, width, height);
		ESP_LOGD(__FUNCTION__, "Finish");
	} else {
		ESP_LOGE(__FUNCTION__, "decode_jpeg fail=%d", err);
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t PNGTest(TFT_t * dev, char * file, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdSetFontDirection(dev, 0);
	lcdFillScreen(dev, BLACK);

	// open PNG file
	FILE* fp = fopen(file, "rb");
	if (fp == NULL) {
		ESP_LOGW(__FUNCTION__, "File not found [%s]", file);
		return 0;
	}

	pngle_t *pngle = pngle_new(width, height);
	if (pngle == NULL) {
		ESP_LOGE(__FUNCTION__, "pngle_new fail");
		fclose(fp);
		return 0;
	}
	pngle_set_init_callback(pngle, png_init);
	pngle_set_draw_callback(pngle, png_draw);
	pngle_set_done_callback(pngle, png_finish);

	double display_gamma = 2.2;
	pngle_set_display_gamma(pngle, display_gamma);

	char buf[1024];
	size_t remain = 0;
	while (!feof(fp)) {
		if (remain >= sizeof(buf)) {
			ESP_LOGE(__FUNCTION__, "Buffer exceeded");
			while(1) vTaskDelay(1);
		}

		int len = fread(buf + remain, 1, sizeof(buf) - remain, fp);
		if (len <= 0) {
			//printf("EOF\n");
			break;
		}

		int fed = pngle_feed(pngle, buf, remain + len);
		if (fed < 0) {
			ESP_LOGE(__FUNCTION__, "ERROR; %s", pngle_error(pngle));
			while(1) vTaskDelay(1);
		}

		remain = remain + len - fed;
		if (remain > 0) memmove(buf, buf + fed, remain);
	}

	fclose(fp);

	uint16_t _width = width;
	uint16_t _cols = 0;
	if (width > pngle->imageWidth) {
		_width = pngle->imageWidth;
		_cols = (width - pngle->imageWidth) / 2;
	}
	ESP_LOGD(__FUNCTION__, "_width=%d _cols=%d", _width, _cols);

	uint16_t _height = height;
	uint16_t _rows = 0;
	if (height > pngle->imageHeight) {
			_height = pngle->imageHeight;
			_rows = (height - pngle->imageHeight) / 2;
	}
	ESP_LOGD(__FUNCTION__, "_height=%d _rows=%d", _height, _rows);

	// allocate memory
	uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * _width);
	if (colors == NULL) {
		ESP_LOGE(__FUNCTION__, "Error allocating memory for colors");
		return 0;
	}

#if 0
	for(int y = 0; y < _height; y++){
		for(int x = 0;x < _width; x++){
			pixel_png pixel = pngle->pixels[y][x];
			uint16_t color = rgb565(pixel.red, pixel.green, pixel.blue);
			lcdDrawPixel(dev, x+_cols, y+_rows, color);
		}
	}
#endif

	for(int y = 0; y < _height; y++){
		for(int x = 0;x < _width; x++){
			//pixel_png pixel = pngle->pixels[y][x];
			//colors[x] = rgb565(pixel.red, pixel.green, pixel.blue);
			colors[x] = pngle->pixels[y][x];
		}
		lcdDrawMultiPixels(dev, _cols, y+_rows, _width, colors);
		vTaskDelay(1);
	}
	lcdDrawFinish(dev);
	free(colors);
	pngle_destroy(pngle, width, height);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t IconTest(TFT_t * dev, char * file, int width, int height, int xpos, int ypos) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// open PNG file
	FILE* fp = fopen(file, "rb");
	if (fp == NULL) {
		ESP_LOGW(__FUNCTION__, "File not found [%s]", file);
		return 0;
	}

	pngle_t *pngle = pngle_new(width, height);
	if (pngle == NULL) {
		ESP_LOGE(__FUNCTION__, "pngle_new fail");
		fclose(fp);
		return 0;
	}
	pngle_set_init_callback(pngle, png_init);
	pngle_set_draw_callback(pngle, png_draw);
	pngle_set_done_callback(pngle, png_finish);

	double display_gamma = 2.2;
	pngle_set_display_gamma(pngle, display_gamma);

	char buf[1024];
	size_t remain = 0;
	while (!feof(fp)) {
		if (remain >= sizeof(buf)) {
			ESP_LOGE(__FUNCTION__, "Buffer exceeded");
			while(1) vTaskDelay(1);
		}

		int len = fread(buf + remain, 1, sizeof(buf) - remain, fp);
		if (len <= 0) {
			//printf("EOF\n");
			break;
		}

		int fed = pngle_feed(pngle, buf, remain + len);
		if (fed < 0) {
			ESP_LOGE(__FUNCTION__, "ERROR; %s", pngle_error(pngle));
			while(1) vTaskDelay(1);
		}

		remain = remain + len - fed;
		if (remain > 0) memmove(buf, buf + fed, remain);
	}

	fclose(fp);

	uint16_t pngWidth = pngle_get_width(pngle);
	uint16_t pngHeight = pngle_get_height(pngle);
	ESP_LOGD(__FUNCTION__, "pngWidth=%d pngHeight=%d", pngWidth, pngHeight);
	uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * pngWidth);
	if (colors == NULL) {
		ESP_LOGE(__FUNCTION__, "malloc fail");
		pngle_destroy(pngle, width, height);
		return 0; 
	}

	for(int y = 0; y < pngHeight; y++){
		for(int x = 0;x < pngWidth; x++){
			//pixel_png pixel = pngle->pixels[y][x];
			//colors[x] = rgb565(pixel.red, pixel.green, pixel.blue);
			colors[x] = pngle->pixels[y][x];
		}
		lcdDrawMultiPixels(dev, xpos, y+ypos, pngWidth, colors);
		vTaskDelay(1);
	}
	lcdDrawFinish(dev);
	free(colors);
	pngle_destroy(pngle, width, height);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t CodeTest(TFT_t * dev, FontxFile *fx, int width, int height, uint16_t start, uint16_t end) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%d fontHeight=%d",fontWidth,fontHeight);
	uint8_t xmoji = width / fontWidth;
	uint8_t ymoji = height / fontHeight;
	//ESP_LOGI(__FUNCTION__,"xmoji=%d ymoji=%d",xmoji, ymoji);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	uint8_t code;

	color = CYAN;
	lcdSetFontDirection(dev, 0);
	//code = 0xA0;
	code = start;
	for(int y=0;y<ymoji;y++) {
		uint16_t xpos = 0;
		uint16_t ypos = fontHeight*(y+1)-1;
		for(int x=0;x<xmoji;x++) {
			xpos = lcdDrawCode(dev, fx, xpos, ypos, code, color);
			if (code == 0xFF) break;
			code++;
			if (code > end) break;
		}
		if (code == 0xFF) break;
		if (code > end) break;
	}
	lcdDrawFinish(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t WrapArroundTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	int counter = 0;
	for (int i=0;i<height;i++) {
		lcdWrapArround(dev, SCROLL_UP, 40, 79);
		lcdWrapArround(dev, SCROLL_DOWN, width-80, width-41);
		counter++;
		if (counter == 5) {
			lcdDrawFinish(dev);
			counter = 0;
		}
	}
	if (counter != 0) lcdDrawFinish(dev);
	vTaskDelay(100);

	counter = 0;
	for (int i=0;i<width;i++) {
		lcdWrapArround(dev, SCROLL_RIGHT, 40, 79);
		lcdWrapArround(dev, SCROLL_LEFT, height-80, height-41);
		counter++;
		if (counter == 5) {
			lcdDrawFinish(dev);
			counter = 0;
		}
	}
	if (counter != 0) lcdDrawFinish(dev);
	vTaskDelay(100);

	if (width == height) {
		counter = 0;
		for (int i=0;i<width;i++) {
			lcdWrapArround(dev, SCROLL_UP, 0, width-1);
			lcdWrapArround(dev, SCROLL_RIGHT, 0, height-1);
			counter++;
			if (counter == 5) {
				lcdDrawFinish(dev);
				counter = 0;
			}
		}
		if (counter != 0) lcdDrawFinish(dev);

#if 0
		vTaskDelay(100);
		for (int i=0;i<width;i++) {
			lcdWrapArround(dev, SCROLL_DOWN, 0, width-1);
			lcdWrapArround(dev, SCROLL_LEFT, 0, height-1);
			if ((i % 2) == 1) {
				lcdDrawFinish(dev);
			}
		}
#endif
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

void RotateImages(int width, int height, uint16_t *image) {
	int index1 = 0;
	int index2 = width * height -1;
	for (int i=0;i<(width * height)/2;i++) {
		uint16_t d1 = image[index1];
		uint16_t d2 = image[index2];
		//ESP_LOGI(__FUNCTION__, "index1=%d index2=%d", index1, index2);
		image[index1] = d2;
		image[index2] = d1;
		index1++;
		index2--;
	}
}

TickType_t ImageMoveTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	int blockWidth = width / 5;
	int blockHeight = height / 5;
	ESP_LOGD(__FUNCTION__, "blockWidth=%d blockHeight=%d", blockWidth, blockHeight);

	uint16_t *block1 = (uint16_t*)malloc(sizeof(uint16_t) * blockWidth * blockHeight);
	if (block1 == NULL) {
		ESP_LOGE(__FUNCTION__, "Error allocating memory for block1");
		return 0;
	}

	uint16_t *block2 = (uint16_t*)malloc(sizeof(uint16_t) * blockWidth * blockHeight);
	if (block2 == NULL) {
		free(block1);
		ESP_LOGE(__FUNCTION__, "Error allocating memory for block2");
		return 0;
	}

	for (int y=0;y<2;y++) {
		for (int x=0;x<5;x++) {
			int x1 = x * blockWidth;
			int y1 = y * blockHeight;
			int x2 = x1 + blockWidth - 1;
			int y2 = y1 + blockHeight - 1;
			ESP_LOGD(__FUNCTION__, "x1=%d y1=%d x2=%d y2=%d", x1, y1, x2, y2);
			lcdGetRect(dev, x1, y1, x2, y2, block1);

			int x3 = blockWidth * (4-x);
			int y3 = blockHeight * (4-y);
			int x4 = x3 + blockWidth - 1;
			int y4 = y3 + blockHeight - 1;
			ESP_LOGD(__FUNCTION__, "x3=%d y3=%d x4=%d y4=%d", x3, y3, x4, y4);
			lcdGetRect(dev, x3, y3, x4, y4, block2);

			RotateImages(blockWidth, blockHeight, block1);
			RotateImages(blockWidth, blockHeight, block2);
			lcdSetRect(dev, x1, y1, x2, y2, block2);
			lcdSetRect(dev, x3, y3, x4, y4, block1);
			lcdDrawFinish(dev);
		}
	}

	for (int x=0;x<2;x++) {
		int x1 = x * blockWidth;;
		int y1 = 2 * blockHeight;
		int x2 = x1 + blockWidth - 1;
		int y2 = y1 + blockHeight - 1;
		ESP_LOGD(__FUNCTION__, "x1=%d y1=%d x2=%d y2=%d", x1, y1, x2, y2);
		lcdGetRect(dev, x1, y1, x2, y2, block1);

		int x3 = blockWidth * (4-x);
		int y3 = 2 * blockHeight;
		int x4 = x3 + blockWidth - 1;
		int y4 = y3 + blockHeight - 1;
		ESP_LOGD(__FUNCTION__, "x3=%d y3=%d x4=%d y4=%d", x3, y3, x4, y4);
		lcdGetRect(dev, x3, y3, x4, y4, block2);

		RotateImages(blockWidth, blockHeight, block1);
		RotateImages(blockWidth, blockHeight, block2);
		lcdSetRect(dev, x1, y1, x2, y2, block2);
		lcdSetRect(dev, x3, y3, x4, y4, block1);
		lcdDrawFinish(dev);
	}

	int x1 = 2 * blockWidth;;
	int y1 = 2 * blockHeight;
	int x2 = x1 + blockWidth - 1;
	int y2 = y1 + blockHeight - 1;
	ESP_LOGD(__FUNCTION__, "x1=%d y1=%d x2=%d y2=%d", x1, y1, x2, y2);
	lcdGetRect(dev, x1, y1, x2, y2, block1);
	RotateImages(blockWidth, blockHeight, block1);
	lcdSetRect(dev, x1, y1, x2, y2, block1);
	lcdDrawFinish(dev);

	free(block1);
	free(block2);
	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t ImageInversionTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	int blockWidth = width / 5;
	int blockHeight = height / 5;
	ESP_LOGD(__FUNCTION__, "blockWidth=%d blockHeight=%d", blockWidth, blockHeight);

	for (int y=0;y<2;y++) {
		for (int x=0;x<5;x++) {
			int x1 = x * blockWidth;
			int y1 = y * blockHeight;
			int x2 = x1 + blockWidth - 1;
			int y2 = y1 + blockHeight - 1;
			ESP_LOGD(__FUNCTION__, "x1=%d y1=%d x2=%d y2=%d", x1, y1, x2, y2);
			lcdInversionArea(dev, x1, y1, x2, y2, NULL);

			int x3 = blockWidth * (4-x);
			int y3 = blockHeight * (4-y);
			int x4 = x3 + blockWidth - 1;
			int y4 = y3 + blockHeight - 1;
			ESP_LOGD(__FUNCTION__, "x3=%d y3=%d x4=%d y4=%d", x3, y3, x4, y4);
			lcdInversionArea(dev, x3, y3, x4, y4, NULL);
			lcdDrawFinish(dev);
		}
	}

	for (int x=0;x<2;x++) {
		int x1 = x * blockWidth;;
		int y1 = 2 * blockHeight;
		int x2 = x1 + blockWidth - 1;
		int y2 = y1 + blockHeight - 1;
		ESP_LOGD(__FUNCTION__, "x1=%d y1=%d x2=%d y2=%d", x1, y1, x2, y2);
		lcdInversionArea(dev, x1, y1, x2, y2, NULL);

		int x3 = blockWidth * (4-x);
		int y3 = 2 * blockHeight;
		int x4 = x3 + blockWidth - 1;
		int y4 = y3 + blockHeight - 1;
		ESP_LOGD(__FUNCTION__, "x3=%d y3=%d x4=%d y4=%d", x3, y3, x4, y4);
		lcdInversionArea(dev, x3, y3, x4, y4, NULL);
		lcdDrawFinish(dev);
	}

	int x1 = 2 * blockWidth;;
	int y1 = 2 * blockHeight;
	int x2 = x1 + blockWidth - 1;
	int y2 = y1 + blockHeight - 1;
	ESP_LOGD(__FUNCTION__, "x1=%d y1=%d x2=%d y2=%d", x1, y1, x2, y2);
	lcdInversionArea(dev, x1, y1, x2, y2, NULL);
	lcdDrawFinish(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t TextBoxTest(TFT_t * dev, FontxFile *fx, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, &fontWidth, &fontHeight);
	ESP_LOGI(__FUNCTION__,"fontWidth=%d fontHeight=%d",fontWidth,fontHeight);

	uint16_t bg_color = BLACK;
	lcdFillScreen(dev, bg_color);

	uint16_t fg_color = WHITE;
	char AtoZ[27];
	strcpy(AtoZ, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	uint8_t ascii[20];
	memset(ascii, 0x00, sizeof(ascii));

	int delay = 200;
	for(int index=0;index<21;index++) {
		lcdSetFontDirection(dev, 0);
		lcdDrawString(dev, fx, 0, fontHeight-1, ascii, bg_color);
		lcdSetFontDirection(dev, 1);
		lcdDrawString(dev, fx, width-fontHeight, 0, ascii, bg_color);
		lcdSetFontDirection(dev, 2);
		lcdDrawString(dev, fx, width, height-fontHeight-1, ascii, bg_color);
		lcdSetFontDirection(dev, 3);
		lcdDrawString(dev, fx, fontHeight-1, height, ascii, bg_color);

		strncpy((char *)ascii, &AtoZ[index], 6);
		lcdSetFontDirection(dev, 0);
		lcdDrawString(dev, fx, 0, fontHeight-1, ascii, fg_color);
		lcdSetFontDirection(dev, 1);
		lcdDrawString(dev, fx, width-fontHeight, 0, ascii, fg_color);
		lcdSetFontDirection(dev, 2);
		lcdDrawString(dev, fx, width, height-fontHeight-1, ascii, fg_color);
		lcdSetFontDirection(dev, 3);
		lcdDrawString(dev, fx, fontHeight-1, height, ascii, fg_color);
		lcdDrawFinish(dev);
		vTaskDelay(delay);
		delay = 40;
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t CursorTest(TFT_t * dev, FontxFile *fx, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%d fontHeight=%d",fontWidth,fontHeight);
	uint8_t xmoji = width / fontWidth;
	uint8_t ymoji = height / fontHeight;
	//ESP_LOGI(__FUNCTION__,"xmoji=%d ymoji=%d",xmoji, ymoji);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	uint8_t code;

	color = CYAN;
	lcdSetFontDirection(dev, 0);
	code = 0x41;
	int ynext = 0;
	for(int y=0;y<ymoji;y++) {
		ynext = y;
		uint16_t xpos = 0;
		uint16_t ypos = fontHeight*(y+1)-1;
		for(int x=0;x<xmoji;x++) {
			xpos = lcdDrawCode(dev, fx, xpos, ypos, code, color);
			if (code == 0x5A) break;
			code++;
		}
		if (code == 0x5A) break;
	}

	code = 0x61;
	for(int y=ynext+1;y<ymoji;y++) {
		uint16_t xpos = 0;
		uint16_t ypos = fontHeight*(y+1)-1;
		for(int x=0;x<xmoji;x++) {
			xpos = lcdDrawCode(dev, fx, xpos, ypos, code, color);
			if (code == 0x7A) break;
			code++;
		}
		if (code == 0x7A) break;
	}
	lcdDrawFinish(dev);

	int x_position[10] = {7,  4, 11, 11, 14,  7, 14,  2, 11,  3};
	int y_position[10] = {0,  2,  2,  2,  2,  1,  2,  3,  2,  2};
	char ascii_code[10] = {'H', 'e', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd'};
	uint8_t ascii[20];
	memset(ascii, 0x00, sizeof(ascii));
	for (int i=0;i<10;i++) {
		uint16_t x_pos = x_position[i] * fontWidth;
		uint16_t y_pos = y_position[i] * fontHeight;
		lcdInversionArea(dev, x_pos, y_pos, x_pos+fontWidth-1, y_pos+fontHeight-1, NULL);
		ascii[i] = ascii_code[i];
		uint16_t color = RED;
		lcdSetFontUnderLine(dev, color);
		lcdDrawString(dev, fx, 0, CONFIG_HEIGHT-fontHeight, ascii, color);
		lcdUnsetFontUnderLine(dev);
		lcdDrawFinish(dev);
		vTaskDelay(100);
		lcdInversionArea(dev, x_pos, y_pos, x_pos+fontWidth-1, y_pos+fontHeight-1, NULL);
		lcdDrawFinish(dev);
		vTaskDelay(10);
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

void ST7789(void *pvParameters)
{
	// set font file
	FontxFile fx16G[2];
	FontxFile fx24G[2];
	FontxFile fx32G[2];
	FontxFile fx32L[2];
	InitFontx(fx16G,"/fonts/ILGH16XB.FNT",""); // 8x16Dot Gothic
	InitFontx(fx24G,"/fonts/ILGH24XB.FNT",""); // 12x24Dot Gothic
	InitFontx(fx32G,"/fonts/ILGH32XB.FNT",""); // 16x32Dot Gothic
	InitFontx(fx32L,"/fonts/LATIN32B.FNT",""); // 16x32Dot Latin

	FontxFile fx16M[2];
	FontxFile fx24M[2];
	FontxFile fx32M[2];
	InitFontx(fx16M,"/fonts/ILMH16XB.FNT",""); // 8x16Dot Mincyo
	InitFontx(fx24M,"/fonts/ILMH24XB.FNT",""); // 12x24Dot Mincyo
	InitFontx(fx32M,"/fonts/ILMH32XB.FNT",""); // 16x32Dot Mincyo
	
	TFT_t dev;

	// Change SPI Clock Frequency
	//spi_clock_speed(40000000); // 40MHz
	//spi_clock_speed(60000000); // 60MHz

	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO, CONFIG_BL_GPIO);
	lcdInit(&dev, CONFIG_WIDTH, CONFIG_HEIGHT, CONFIG_OFFSETX, CONFIG_OFFSETY);

#if CONFIG_INVERSION
	ESP_LOGI(TAG, "Enable Display Inversion");
	//lcdInversionOn(&dev);
	lcdInversionOff(&dev);
#endif

	char file[32];
#if 0
	while (1) {
		FillTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;
		TextBoxTest(&dev, fx24G, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;
		TextBoxTest(&dev, fx32G, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;
	}
#endif

	while(1) {
		traceHeap();

		FillTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		ColorBarTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		ArrowTest(&dev, fx16G, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		LineTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		CircleTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		RoundRectTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		if (dev._use_frame_buffer == false) {
			RectAngleTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
			WAIT;

			TriangleTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
			WAIT;
		}

		if (CONFIG_WIDTH >= 240) {
			DirectionTest(&dev, fx24G, CONFIG_WIDTH, CONFIG_HEIGHT);
		} else {
			DirectionTest(&dev, fx16G, CONFIG_WIDTH, CONFIG_HEIGHT);
		}
		WAIT;

		if (CONFIG_WIDTH >= 240) {
			HorizontalTest(&dev, fx24G, CONFIG_WIDTH, CONFIG_HEIGHT);
		} else {
			HorizontalTest(&dev, fx16G, CONFIG_WIDTH, CONFIG_HEIGHT);
		}
		WAIT;

		if (CONFIG_WIDTH >= 240) {
			VerticalTest(&dev, fx24G, CONFIG_WIDTH, CONFIG_HEIGHT);
		} else {
			VerticalTest(&dev, fx16G, CONFIG_WIDTH, CONFIG_HEIGHT);
		}
		WAIT;

		FillRectTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		ColorTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		CodeTest(&dev, fx32G, CONFIG_WIDTH, CONFIG_HEIGHT, 0xA0, 0xFF);
		WAIT;

		CodeTest(&dev, fx32L, CONFIG_WIDTH, CONFIG_HEIGHT, 0xA0, 0xFF);
		WAIT;

		strcpy(file, "/images/image.bmp");
		BMPTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		strcpy(file, "/images/esp32.jpeg");
		JPEGTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		strcpy(file, "/images/esp_logo.png");
		PNGTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		if (dev._use_frame_buffer == true) {
			WrapArroundTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
			WAIT;

			ImageMoveTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
			WAIT;

			ImageInversionTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
			WAIT;

#if 0
			CursorTest(&dev, fx32G, CONFIG_WIDTH, CONFIG_HEIGHT);
			WAIT;
#endif
			TextBoxTest(&dev, fx32G, CONFIG_WIDTH, CONFIG_HEIGHT);
			WAIT;
		}

		strcpy(file, "/images/qrcode.bmp");
		QRTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		lcdSetFontDirection(&dev, 0);
		lcdFillScreen(&dev, WHITE);
		strcpy(file, "/icons/battery01.png");
		IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 0);
		strcpy(file, "/icons/battery02.png");
		IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 50);
		strcpy(file, "/icons/battery03.png");
		IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 100);
		strcpy(file, "/icons/battery04.png");
		IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 150);
		strcpy(file, "/icons/battery05.png");
		IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 200);

		if (CONFIG_WIDTH > 160) {
			strcpy(file, "/icons/battery06.png");
			IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, (CONFIG_WIDTH/2)-1, 0);
			strcpy(file, "/icons/battery07.png");
			IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, (CONFIG_WIDTH/2)-1, 50);
			strcpy(file, "/icons/battery08.png");
			IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, (CONFIG_WIDTH/2)-1, 100);
			strcpy(file, "/icons/battery11.png");
			IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, (CONFIG_WIDTH/2)-1, 150);
			strcpy(file, "/icons/battery12.png");
			IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, (CONFIG_WIDTH/2)-1, 200);
		}
		WAIT;

		// Multi Font Test
		uint16_t color;
		uint8_t ascii[40];
		uint16_t margin = 10;
		lcdFillScreen(&dev, BLACK);
		color = WHITE;
		lcdSetFontDirection(&dev, 0);
		uint16_t xpos = 0;
		uint16_t ypos = 15;
		int xd = 0;
		int yd = 1;
		if(CONFIG_WIDTH < CONFIG_HEIGHT) {
			lcdSetFontDirection(&dev, 1);
			xpos = (CONFIG_WIDTH-1)-16;
			ypos = 0;
			xd = 1;
			yd = 0;
		}
		strcpy((char *)ascii, "16Dot Gothic Font");
		lcdDrawString(&dev, fx16G, xpos, ypos, ascii, color);

		xpos = xpos - (24 * xd) - (margin * xd);
		ypos = ypos + (16 * yd) + (margin * yd);
		strcpy((char *)ascii, "24Dot Gothic Font");
		lcdDrawString(&dev, fx24G, xpos, ypos, ascii, color);

		xpos = xpos - (32 * xd) - (margin * xd);
		ypos = ypos + (24 * yd) + (margin * yd);
		if (CONFIG_WIDTH >= 240) {
			strcpy((char *)ascii, "32Dot Gothic Font");
			lcdDrawString(&dev, fx32G, xpos, ypos, ascii, color);
			xpos = xpos - (32 * xd) - (margin * xd);;
			ypos = ypos + (32 * yd) + (margin * yd);
		}

		xpos = xpos - (10 * xd) - (margin * xd);
		ypos = ypos + (10 * yd) + (margin * yd);
		strcpy((char *)ascii, "16Dot Mincyo Font");
		lcdDrawString(&dev, fx16M, xpos, ypos, ascii, color);

		xpos = xpos - (24 * xd) - (margin * xd);;
		ypos = ypos + (16 * yd) + (margin * yd);
		strcpy((char *)ascii, "24Dot Mincyo Font");
		lcdDrawString(&dev, fx24M, xpos, ypos, ascii, color);

		if (CONFIG_WIDTH >= 240) {
			xpos = xpos - (32 * xd) - (margin * xd);;
			ypos = ypos + (24 * yd) + (margin * yd);
			strcpy((char *)ascii, "32Dot Mincyo Font");
			lcdDrawString(&dev, fx32M, xpos, ypos, ascii, color);
		}
		lcdDrawFinish(&dev);
		lcdSetFontDirection(&dev, 0);
		WAIT;

	} // end while

	// never reach here
	while (1) {
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}

static void listSPIFFS(char * path) {
	DIR* dir = opendir(path);
	assert(dir != NULL);
	while (true) {
		struct dirent*pe = readdir(dir);
		if (!pe) break;
		ESP_LOGI(__FUNCTION__,"d_name=%s d_ino=%d d_type=%x", pe->d_name,pe->d_ino, pe->d_type);
	}
	closedir(dir);
}

esp_err_t mountSPIFFS(char * path, char * label, int max_files) {
	esp_vfs_spiffs_conf_t conf = {
		.base_path = path,
		.partition_label = label,
		.max_files = max_files,
		.format_if_mount_failed = true
	};

	// Use settings defined above to initialize and mount SPIFFS filesystem.
	// Note: esp_vfs_spiffs_register is an all-in-one convenience function.
	esp_err_t ret = esp_vfs_spiffs_register(&conf);

	if (ret != ESP_OK) {
		if (ret ==ESP_FAIL) {
			ESP_LOGE(TAG, "Failed to mount or format filesystem");
		} else if (ret== ESP_ERR_NOT_FOUND) {
			ESP_LOGE(TAG, "Failed to find SPIFFS partition");
		} else {
			ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)",esp_err_to_name(ret));
		}
		return ret;
	}

#if 0
	ESP_LOGI(TAG, "Performing SPIFFS_check().");
	ret = esp_spiffs_check(conf.partition_label);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
		return ret;
	} else {
			ESP_LOGI(TAG, "SPIFFS_check() successful");
	}
#endif

	size_t total = 0, used = 0;
	ret = esp_spiffs_info(conf.partition_label, &total, &used);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG,"Failed to get SPIFFS partition information (%s)",esp_err_to_name(ret));
	} else {
		ESP_LOGI(TAG,"Mount %s to %s success", path, label);
		ESP_LOGI(TAG,"Partition size: total: %d, used: %d", total, used);
	}

	return ret;
}


void app_main(void)
{
	ESP_LOGI(TAG, "Initializing SPIFFS");
	// Maximum files that could be open at the same time is 7.
	ESP_ERROR_CHECK(mountSPIFFS("/fonts", "storage1", 7));
	listSPIFFS("/fonts/");

	// Maximum files that could be open at the same time is 1.
	ESP_ERROR_CHECK(mountSPIFFS("/images", "storage2", 1));
	listSPIFFS("/images/");

	// Maximum files that could be open at the same time is 1.
	ESP_ERROR_CHECK(mountSPIFFS("/icons", "storage3", 1));
	listSPIFFS("/icons/");

	xTaskCreate(ST7789, "ST7789", 1024*6, NULL, 2, NULL);
}

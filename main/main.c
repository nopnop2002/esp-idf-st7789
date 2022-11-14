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

static void SPIFFS_Directory(char * path) {
	DIR* dir = opendir(path);
	assert(dir != NULL);
	while (true) {
		struct dirent*pe = readdir(dir);
		if (!pe) break;
		ESP_LOGI(__FUNCTION__,"d_name=%s d_ino=%d d_type=%x", pe->d_name,pe->d_ino, pe->d_type);
	}
	closedir(dir);
}

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

TickType_t FillTest(TFT_t * dev, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdFillScreen(dev, RED);
	vTaskDelay(50);
	lcdFillScreen(dev, GREEN);
	vTaskDelay(50);
	lcdFillScreen(dev, BLUE);
	vTaskDelay(50);

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
	} else {
		uint16_t x1,x2;
		x1 = width/3;
		x2 = (width/3)*2;
		lcdDrawFillRect(dev, 0, 0, x1-1, height-1, RED);
		vTaskDelay(1);
		lcdDrawFillRect(dev, x1, 0, x2-1, height-1, GREEN);
		vTaskDelay(1);
		lcdDrawFillRect(dev, x2, 0, width-1, height-1, BLUE);
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
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
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

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t DirectionTest(TFT_t * dev, FontxFile *fx, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
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

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t HorizontalTest(TFT_t * dev, FontxFile *fx, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
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
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
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
		color=rgb565_conv(red, green, blue);
		uint16_t xpos=rand()%width;
		uint16_t ypos=rand()%height;
		uint16_t size=rand()%(width/5);
		lcdDrawFillRect(dev, xpos, ypos, xpos+size, ypos+size, color);
	}

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

	// open requested file
	esp_err_t ret;
	FILE* fp = fopen(file, "rb");
	if (fp == NULL) {
		ESP_LOGW(__FUNCTION__, "File not found [%s]", file);
		return 0;
	}

	// read bmp header
	bmpfile_t *result = (bmpfile_t*)malloc(sizeof(bmpfile_t));
	ret = fread(result->header.magic, 1, 2, fp);
	assert(ret == 2);
	if (result->header.magic[0]!='B' || result->header.magic[1] != 'M') {
		ESP_LOGW(__FUNCTION__, "File is not BMP");
		free(result);
		fclose(fp);
		return 0;
	}
	ret = fread(&result->header.filesz, 4, 1 , fp);
	assert(ret == 1);
	ESP_LOGD(__FUNCTION__,"result->header.filesz=%"PRIu32, result->header.filesz);
	ret = fread(&result->header.creator1, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->header.creator2, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->header.offset, 4, 1, fp);
	assert(ret == 1);

	// read dib header
	ret = fread(&result->dib.header_sz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.width, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.height, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.nplanes, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.depth, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.compress_type, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.bmp_bytesz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.hres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.vres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.ncolors, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.nimpcolors, 4, 1, fp);
	assert(ret == 1);

	if((result->dib.depth == 24) && (result->dib.compress_type == 0)) {
		// BMP rows are padded (if needed) to 4-byte boundary
		uint32_t rowSize = (result->dib.width * 3 + 3) & ~3;
		int w = result->dib.width;
		int h = result->dib.height;
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

		for (int row=0; row<h; row++) { // For each scanline...
			if (row < _rows || row > _rowe) continue;
			// Seek to start of scan line.	It might seem labor-
			// intensive to be doing this on every line, but this
			// method covers a lot of gritty details like cropping
			// and scanline padding.  Also, the seek only takes
			// place if the file position actually needs to change
			// (avoids a lot of cluster math in SD library).
			// Bitmap is stored bottom-to-top order (normal BMP)
			int pos = result->header.offset + (h - 1 - row) * rowSize;
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
				colors[index++] = rgb565_conv(r, g, b);
			} // end for col
			ESP_LOGD(__FUNCTION__,"lcdDrawMultiPixels _x=%d _y=%d row=%d",_x, _y, row);
			//lcdDrawMultiPixels(dev, _x, row+_y, _w, colors);
			lcdDrawMultiPixels(dev, _x, _y, _w, colors);
			_y++;
		} // end for row
		free(colors);
	} // end if
	free(result);
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

	// open requested file
	esp_err_t ret;
	FILE* fp = fopen(file, "rb");
	if (fp == NULL) {
		ESP_LOGW(__FUNCTION__, "File not found [%s]", file);
		return 0;
	}

	// read bmp header
	bmpfile_t *result = (bmpfile_t*)malloc(sizeof(bmpfile_t));
	ret = fread(result->header.magic, 1, 2, fp);
	assert(ret == 2);
	if (result->header.magic[0]!='B' || result->header.magic[1] != 'M') {
		ESP_LOGW(__FUNCTION__, "File is not BMP");
		free(result);
		fclose(fp);
		return 0;
	}
	ret = fread(&result->header.filesz, 4, 1 , fp);
	assert(ret == 1);
	ESP_LOGD(__FUNCTION__,"result->header.filesz=%"PRIu32, result->header.filesz);
	ret = fread(&result->header.creator1, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->header.creator2, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->header.offset, 4, 1, fp);
	assert(ret == 1);

	// read dib header
	ret = fread(&result->dib.header_sz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.width, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.height, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.nplanes, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.depth, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.compress_type, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.bmp_bytesz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.hres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.vres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.ncolors, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.nimpcolors, 4, 1, fp);
	assert(ret == 1);

	ESP_LOGD(__FUNCTION__, "dib.depth=%d dib.compress_type=%"PRIu32, result->dib.depth, result->dib.compress_type);
	//if((result->dib.depth == 24) && (result->dib.compress_type == 0)) {
	if((result->dib.depth == 1) && (result->dib.compress_type == 0)) {
		ESP_LOGD(__FUNCTION__, "dib.bmp_bytesz=%"PRIu32, result->dib.bmp_bytesz);
		// BMP rows are padded (if needed) to 4-byte boundary
		//uint32_t rowSize = (result->dib.width * 3 + 3) & ~3;
		int w = result->dib.width;
		int h = result->dib.height;
		uint32_t rowSize = result->dib.bmp_bytesz / result->dib.height;
		ESP_LOGD(__FUNCTION__,"dib.width=%"PRIu32" dib.height=%"PRIu32" rowSize=%"PRIu32, result->dib.width, result->dib.height, rowSize);
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
		uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * _w); // tft buffer

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
			int pos = result->header.offset + (h - 1 - row) * rowSize;
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
				//colors[index++] = rgb565_conv(r, g, b);
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
	free(result);
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
		uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * _width);

#if 0
		for(int y = 0; y < _height; y++){
			for(int x = 0;x < _width; x++){
				pixel_jpeg pixel = pixels[y][x];
				uint16_t color = rgb565_conv(pixel.red, pixel.green, pixel.blue);
				lcdDrawPixel(dev, x+_cols, y+_rows, color);
			}
			vTaskDelay(1);
		}
#endif

		for(int y = 0; y < _height; y++){
			for(int x = 0;x < _width; x++){
				//pixel_jpeg pixel = pixels[y][x];
				//colors[x] = rgb565_conv(pixel.red, pixel.green, pixel.blue);
				colors[x] = pixels[y][x];
			}
			lcdDrawMultiPixels(dev, _cols, y+_rows, _width, colors);
			vTaskDelay(1);
		}

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

	char buf[1024];
	size_t remain = 0;
	int len;

	pngle_t *pngle = pngle_new(width, height);

	pngle_set_init_callback(pngle, png_init);
	pngle_set_draw_callback(pngle, png_draw);
	pngle_set_done_callback(pngle, png_finish);

	double display_gamma = 2.2;
	pngle_set_display_gamma(pngle, display_gamma);


	while (!feof(fp)) {
		if (remain >= sizeof(buf)) {
			ESP_LOGE(__FUNCTION__, "Buffer exceeded");
			while(1) vTaskDelay(1);
		}

		len = fread(buf + remain, 1, sizeof(buf) - remain, fp);
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
	uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * _width);

#if 0
	for(int y = 0; y < _height; y++){
		for(int x = 0;x < _width; x++){
			pixel_png pixel = pngle->pixels[y][x];
			uint16_t color = rgb565_conv(pixel.red, pixel.green, pixel.blue);
			lcdDrawPixel(dev, x+_cols, y+_rows, color);
		}
	}
#endif

	for(int y = 0; y < _height; y++){
		for(int x = 0;x < _width; x++){
			//pixel_png pixel = pngle->pixels[y][x];
			//colors[x] = rgb565_conv(pixel.red, pixel.green, pixel.blue);
			colors[x] = pngle->pixels[y][x];
		}
		lcdDrawMultiPixels(dev, _cols, y+_rows, _width, colors);
		vTaskDelay(1);
	}
	free(colors);
	pngle_destroy(pngle, width, height);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t CodeTest(TFT_t * dev, FontxFile *fx, int width, int height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%d fontHeight=%d",fontWidth,fontHeight);
	uint8_t xmoji = width / fontWidth;
	uint8_t ymoji = height / fontHeight;
	//ESP_LOGI(__FUNCTION__,"xmoji=%d ymoji=%d",xmoji, ymoji);


	uint16_t color;
	lcdFillScreen(dev, BLACK);
	uint8_t code;

	color = CYAN;
	lcdSetFontDirection(dev, 0);
	code = 0xA0;
	for(int y=0;y<ymoji;y++) {
		uint16_t xpos = 0;
		uint16_t ypos =  fontHeight*(y+1)-1;
		for(int x=0;x<xmoji;x++) {
			xpos = lcdDrawCode(dev, fx, xpos, ypos, code, color);
			if (code == 0xFF) break;
			code++;
		}
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
	InitFontx(fx16G,"/spiffs/ILGH16XB.FNT",""); // 8x16Dot Gothic
	InitFontx(fx24G,"/spiffs/ILGH24XB.FNT",""); // 12x24Dot Gothic
	InitFontx(fx32G,"/spiffs/ILGH32XB.FNT",""); // 16x32Dot Gothic
	InitFontx(fx32L,"/spiffs/LATIN32B.FNT",""); // 16x32Dot Latin

	FontxFile fx16M[2];
	FontxFile fx24M[2];
	FontxFile fx32M[2];
	InitFontx(fx16M,"/spiffs/ILMH16XB.FNT",""); // 8x16Dot Mincyo
	InitFontx(fx24M,"/spiffs/ILMH24XB.FNT",""); // 12x24Dot Mincyo
	InitFontx(fx32M,"/spiffs/ILMH32XB.FNT",""); // 16x32Dot Mincyo
	
	TFT_t dev;
	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO, CONFIG_BL_GPIO);
	lcdInit(&dev, CONFIG_WIDTH, CONFIG_HEIGHT, CONFIG_OFFSETX, CONFIG_OFFSETY);

#if CONFIG_INVERSION
	ESP_LOGI(TAG, "Enable Display Inversion");
	lcdInversionOn(&dev);
#endif

#if 0
	while (1) {
		FillTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		char file[32];
		strcpy(file, "/spiffs/qrcode.bmp");
		QRTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

#ifndef CONFIG_IDF_TARGET_ESP32S2
		strcpy(file, "/spiffs/esp32.jpeg");
		JPEGTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;
#endif
	}
#endif

#if 0
	//for TEST
	lcdDrawFillRect(&dev, 0, 0, 10, 10, RED);
	lcdDrawFillRect(&dev, 10, 10, 20, 20, GREEN);
	lcdDrawFillRect(&dev, 20, 20, 30, 30, BLUE);
#endif

	while(1) {

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

		RectAngleTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		TriangleTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

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

		CodeTest(&dev, fx32G, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		CodeTest(&dev, fx32L, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		char file[32];
		strcpy(file, "/spiffs/image.bmp");
		BMPTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

#ifndef CONFIG_IDF_TARGET_ESP32S2
		strcpy(file, "/spiffs/esp32.jpeg");
		JPEGTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;
#endif

		strcpy(file, "/spiffs/esp_logo.png");
		PNGTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
		WAIT;

		strcpy(file, "/spiffs/qrcode.bmp");
		QRTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
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
		lcdSetFontDirection(&dev, 0);
		WAIT;

	} // end while

	// never reach
	while (1) {
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}


void app_main(void)
{
	ESP_LOGI(TAG, "Initializing SPIFFS");

	esp_vfs_spiffs_conf_t conf = {
		.base_path = "/spiffs",
		.partition_label = NULL,
		.max_files = 12,
		.format_if_mount_failed =true
	};

	// Use settings defined above toinitialize and mount SPIFFS filesystem.
	// Note: esp_vfs_spiffs_register is anall-in-one convenience function.
	esp_err_t ret = esp_vfs_spiffs_register(&conf);

	if (ret != ESP_OK) {
		if (ret == ESP_FAIL) {
			ESP_LOGE(TAG, "Failed to mount or format filesystem");
		} else if (ret == ESP_ERR_NOT_FOUND) {
			ESP_LOGE(TAG, "Failed to find SPIFFS partition");
		} else {
			ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)",esp_err_to_name(ret));
		}
		return;
	}

	size_t total = 0, used = 0;
	ret = esp_spiffs_info(NULL, &total,&used);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG,"Failed to get SPIFFS partition information (%s)",esp_err_to_name(ret));
	} else {
		ESP_LOGI(TAG,"Partition size: total: %d, used: %d", total, used);
	}

	SPIFFS_Directory("/spiffs/");
	xTaskCreate(ST7789, "ST7789", 1024*6, NULL, 2, NULL);
}

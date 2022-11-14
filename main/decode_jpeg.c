#include <stdio.h>
#include "decode_jpeg.h"
//#include "esp32/rom/tjpgd.h"
#include "rom/tjpgd.h"
#include "esp_log.h"

//Data that is passed from the decoder function to the infunc/outfunc functions.
typedef struct {
	pixel_jpeg **outData;		// Array of IMAGE_H pointers to arrays of 16-bit pixel values
	int screenWidth;		// Width of the screen
	int screenHeight;		// Height of the screen
	FILE* fp;				// File pointer of jpeg file
} JpegDev;


//Input function for jpeg decoder. Just returns bytes from the inData field of the JpegDev structure.
//static UINT infunc(JDEC *decoder, BYTE *buf, UINT len) {
static unsigned int infunc(JDEC *decoder, uint8_t *buf, unsigned int len) {
	JpegDev *jd = (JpegDev *) decoder->device;
	ESP_LOGD(__FUNCTION__, "infunc len=%d fp=%p", len, jd->fp);
	int rlen;
	if (buf != NULL) { /* Read nd bytes from the input strem */
		rlen = fread(buf, 1, len, jd->fp);
		ESP_LOGD(__FUNCTION__, "rlen=%d",rlen);
	} else { /* Skip nd bytes on the input stream */
		ESP_LOGD(__FUNCTION__, "buff is NULL");
		fseek(jd->fp, len, SEEK_CUR);
		rlen = len;
	}
	return rlen;
}

#define rgb565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))

//Output function. Re-encodes the RGB888 data from the decoder as big-endian RGB565 and
//stores it in the outData array of the JpegDev structure.
//static UINT outfunc(JDEC *decoder, void *bitmap, JRECT *rect) {
static unsigned int outfunc(JDEC *decoder, void *bitmap, JRECT *rect) {
	JpegDev *jd = (JpegDev *) decoder->device;
	uint8_t *in = (uint8_t *) bitmap;
	ESP_LOGD(__FUNCTION__, "rect->top=%d rect->bottom=%d", rect->top, rect->bottom);
	ESP_LOGD(__FUNCTION__, "rect->left=%d rect->right=%d", rect->left, rect->right);
	ESP_LOGD(__FUNCTION__, "jd->screenWidth=%d jd->screenHeight=%d", jd->screenWidth, jd->screenHeight);

	for (int y = rect->top; y <= rect->bottom; y++) {
		for (int x = rect->left; x <= rect->right; x++) {

			if (y < jd->screenHeight && x < jd->screenWidth) {
#if 0
				jd->outData[y][x].red = in[0];
				jd->outData[y][x].green = in[1];
				jd->outData[y][x].blue = in[2];
#endif
				jd->outData[y][x] = rgb565(in[0], in[1], in[2]);
			}

			in += 3;
		}
	}
	return 1;
}

// Specifies scaling factor N for output. The output image is descaled to 1 / 2 ^ N (N = 0 to 3).
// When scaling feature is disabled (JD_USE_SCALE == 0), it must be 0.
uint8_t getScale(int screenWidth, int screenHeight, uint16_t decodeWidth, uint16_t decodeHeight) {
	if (screenWidth >= decodeWidth && screenHeight >= decodeHeight)  return 0;

	double scaleWidth = (double)decodeWidth / (double)screenWidth;
	double scaleHeight = (double)decodeHeight / (double)screenHeight;
	ESP_LOGD(__FUNCTION__, "scaleWidth=%f scaleHeight=%f", scaleWidth, scaleHeight);
	double scale = scaleWidth;
	if (scaleWidth < scaleHeight) scale = scaleHeight;
	ESP_LOGD(__FUNCTION__, "scale=%f", scale);
	if (scale <= 2.0) return 1;
	if (scale <= 4.0) return 2;
	return 3;

}

//Size of the work space for the jpeg decoder.
#define WORKSZ 3100

//Decode the embedded image into pixel lines that can be used with the rest of the logic.
esp_err_t decode_jpeg(pixel_jpeg ***pixels, char * file, int screenWidth, int screenHeight, int * imageWidth, int * imageHeight) {
	char *work = NULL;
	int r;
	JDEC decoder;
	JpegDev jd;
	*pixels = NULL;
	esp_err_t ret = ESP_OK;


	//Alocate pixel memory. Each line is an array of IMAGE_W 16-bit pixels; the `*pixels` array itself contains pointers to these lines.
	*pixels = calloc(screenHeight, sizeof(pixel_jpeg *));
	if (*pixels == NULL) {
		ESP_LOGE(__FUNCTION__, "Error allocating memory for lines");
		ret = ESP_ERR_NO_MEM;
		goto err;
	}
	for (int i = 0; i < screenHeight; i++) {
		(*pixels)[i] = malloc(screenWidth * sizeof(pixel_jpeg));
		if ((*pixels)[i] == NULL) {
			ESP_LOGE(__FUNCTION__, "Error allocating memory for line %d", i);
			ret = ESP_ERR_NO_MEM;
			goto err;
		}
	}

	//Allocate the work space for the jpeg decoder.
	work = calloc(WORKSZ, 1);
	if (work == NULL) {
		ESP_LOGE(__FUNCTION__, "Cannot allocate workspace");
		ret = ESP_ERR_NO_MEM;
		goto err;
	}

	
	//Populate fields of the JpegDev struct.
	jd.outData = *pixels;
	jd.screenWidth = screenWidth;
	jd.screenHeight = screenHeight;
	jd.fp = fopen(file, "rb");
	if (jd.fp == NULL) {
		ESP_LOGW(__FUNCTION__, "Image file not found [%s]", file);
		ret = ESP_ERR_NOT_FOUND;
		goto err;
	}
	ESP_LOGD(__FUNCTION__, "jd.fp=%p", jd.fp);

	//Prepare and decode the jpeg.
	r = jd_prepare(&decoder, infunc, work, WORKSZ, (void *) &jd);
	if (r != JDR_OK) {
		ESP_LOGE(__FUNCTION__, "Image decoder: jd_prepare failed (%d)", r);
		ret = ESP_ERR_NOT_SUPPORTED;
		goto err;
	}
	ESP_LOGD(__FUNCTION__, "decoder.width=%d decoder.height=%d", decoder.width, decoder.height);

	//Calculate Scaling factor
	uint8_t scale = getScale(screenWidth, screenHeight, decoder.width, decoder.height);
	ESP_LOGD(__FUNCTION__, "scale=%d", scale);

	//Calculate image size
	double factor = 1.0;
	if (scale == 1) factor = 0.5;
	if (scale == 2) factor = 0.25;
	if (scale == 3) factor = 0.125;
	ESP_LOGD(__FUNCTION__, "factor=%f",factor);
	*imageWidth = (double)decoder.width * factor;
	*imageHeight = (double)decoder.height * factor;
	ESP_LOGD(__FUNCTION__, "imageWidth=%d imageHeight=%d", *imageWidth, *imageHeight);


	r = jd_decomp(&decoder, outfunc, scale);
	if (r != JDR_OK) {
		ESP_LOGE(__FUNCTION__, "Image decoder: jd_decode failed (%d)", r);
		ret = ESP_ERR_NOT_SUPPORTED;
		goto err;
	}

	//All done! Free the work area (as we don't need it anymore) and return victoriously.
	free(work);
	fclose(jd.fp);
	return ret;

	//Something went wrong! Exit cleanly, de-allocating everything we allocated.
	err:
	fclose(jd.fp);
	if (*pixels != NULL) {
		for (int i = 0; i < screenHeight; i++) {
			free((*pixels)[i]);
		}
		free(*pixels);
	}
	free(work);
	return ret;
}


esp_err_t release_image(pixel_jpeg ***pixels, int screenWidth, int screenHeight) {
	if (*pixels != NULL) {
		for (int i = 0; i < screenHeight; i++) {
			free((*pixels)[i]);
		}
		free(*pixels);
	}
	return ESP_OK;
}


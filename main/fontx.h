#ifndef MAIN_FONTX_H_
#define MAIN_FONTX_H_
#define FontxGlyphBufSize (32*32/8)

typedef struct {
	const char *path;
	char  fxname[10];
	bool  opened;
	bool  valid;
	bool  is_ank;
	uint8_t w;
	uint8_t h;
	uint16_t fsz;
	uint8_t bc;
	FILE *file;
} FontxFile;

void AaddFontx(FontxFile *fx, const char *path);
void InitFontx(FontxFile *fxs, const char *f0, const char *f1);
bool OpenFontx(FontxFile *fx);
void CloseFontx(FontxFile *fx);
void DumpFontx(FontxFile *fxs);
uint8_t getFortWidth(FontxFile *fx);
uint8_t getFortHeight(FontxFile *fx);
bool GetFontx(FontxFile *fxs, uint8_t ascii , uint8_t *pGlyph, uint8_t *pw, uint8_t *ph);
void Font2Bitmap(uint8_t *fonts, uint8_t *line, uint8_t w, uint8_t h, uint8_t inverse);
void UnderlineBitmap(uint8_t *line, uint8_t w, uint8_t h);
void ReversBitmap(uint8_t *line, uint8_t w, uint8_t h);
void ShowFont(uint8_t *fonts, uint8_t pw, uint8_t ph);
void ShowBitmap(uint8_t *bitmap, uint8_t pw, uint8_t ph);
uint8_t RotateByte(uint8_t ch);

// UTF8 to SJIS table
// https://www.mgo-tec.com/blog-entry-utf8sjis01.html
//#define Utf8Sjis "Utf8Sjis.tbl"
//uint16_t UTF2SJIS(spiffs_file fd, uint8_t *utf8);
//int String2SJIS(spiffs_file fd, unsigned char *str_in, size_t stlen, uint16_t *sjis, size_t ssize);
#endif /* MAIN_FONTX_H_ */


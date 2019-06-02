#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

#include "fontx.h"

#define FontxDebug 0 // for Debug

// フォントファイルパスを構造体に保存
void AddFontx(FontxFile *fx, const char *path)
{
	memset(fx, 0, sizeof(FontxFile));
	fx->path = path;
	fx->opened = false;
}

// フォント構造体を初期化
void InitFontx(FontxFile *fxs, const char *f0, const char *f1)
{
	AddFontx(&fxs[0], f0);
	AddFontx(&fxs[1], f1);
}

// フォントファイルをOPEN
bool OpenFontx(FontxFile *fx)
{
	FILE *f;
	if(!fx->opened){
		if(FontxDebug)printf("[openFont]fx->path=[%s]\n",fx->path);
		f = fopen(fx->path, "r");
		if(FontxDebug)printf("[openFont]fopen=%p\n",f);
		if (f == NULL) {
			fx->valid = false;
			printf("Fontx:%s not found.\n",fx->path);
			return fx->valid ;
		}
		fx->opened = true;
		fx->file = f;
		char buf[18];
		if (fread(buf, 1, sizeof(buf), fx->file) != sizeof(buf)) {
			fx->valid = false;
			printf("Fontx:%s not FONTX format.\n",fx->path);
			fclose(fx->file);
			return fx->valid ;
		}

		if(FontxDebug) {
			for(int i=0;i<sizeof(buf);i++) {
				printf("buf[%d]=0x%x\n",i,buf[i]);
			}
		}
		memcpy(fx->fxname, &buf[6], 8);
		fx->w = buf[14];
		fx->h = buf[15];
		fx->is_ank = (buf[16] == 0);
		fx->bc = buf[17];
		fx->fsz = (fx->w + 7)/8 * fx->h;
		if(fx->fsz > FontxGlyphBufSize){
			printf("Fontx:%s is too big font size.\n",fx->path);
			fx->valid = false;
			fclose(fx->file);
			return fx->valid ;
		}
		fx->valid = true;
	}
	return fx->valid;
}

// フォントファイルをCLOSE
void CloseFontx(FontxFile *fx)
{
	if(fx->opened){
		fclose(fx->file);
		fx->opened = false;
	}
}

// フォント構造体の表示
void DumpFontx(FontxFile *fxs)
{
	for(int i=0;i<2;i++) {
		printf("fxs[%d]->path=%s\n",i,fxs[i].path);
		printf("fxs[%d]->opened=%d\n",i,fxs[i].opened);
		printf("fxs[%d]->fxname=%s\n",i,fxs[i].fxname);
		printf("fxs[%d]->valid=%d\n",i,fxs[i].valid);
		printf("fxs[%d]->is_ank=%d\n",i,fxs[i].is_ank);
		printf("fxs[%d]->w=%d\n",i,fxs[i].w);
		printf("fxs[%d]->h=%d\n",i,fxs[i].h);
		printf("fxs[%d]->fsz=%d\n",i,fxs[i].fsz);
		printf("fxs[%d]->bc=%d\n",i,fxs[i].bc);
	}
}

uint8_t getFortWidth(FontxFile *fx) {
	printf("fx->w=%d\n",fx->w);
	return(fx->w);
}

uint8_t getFortHeight(FontxFile *fx) {
	printf("fx->h=%d\n",fx->h);
	return(fx->h);
}


/*
 フォントファイルからフォントパターンを取り出す

 フォントの並び(16X16ドット)
    00000000    01111111
    12345678    90123456
 01 pGlyph[000] pGlyph[001]
 02 pGlyph[002] pGlyph[003]
 03 pGlyph[004] pGlyph[005]
 04 pGlyph[006] pGlyph[007]
 05 pGlyph[008] pGlyph[009]
 06 pGlyph[010] pGlyph[011]
 07 pGlyph[012] pGlyph[013]
 08 pGlyph[014] pGlyph[015]
 09 pGlyph[016] pGlyph[017]
 10 pGlyph[018] pGlyph[019]
 11 pGlyph[020] pGlyph[021]
 12 pGlyph[022] pGlyph[023]
 13 pGlyph[024] pGlyph[025]
 14 pGlyph[026] pGlyph[027]
 15 pGlyph[028] pGlyph[029]
 16 pGlyph[030] pGlyph[031]

 フォントの並び(24X24ドット)
    00000000    01111111    11122222
    12345678    90123456    78901234
 01 pGlyph[000] pGlyph[001] pGlyph[002]
 02 pGlyph[003] pGlyph[004] pGlyph[005]
 03 pGlyph[006] pGlyph[007] pGlyph[008]
 04 pGlyph[009] pGlyph[010] pGlyph[011]
 05 pGlyph[012] pGlyph[013] pGlyph[014]
 06 pGlyph[015] pGlyph[016] pGlyph[017]
 07 pGlyph[018] pGlyph[019] pGlyph[020]
 08 pGlyph[021] pGlyph[022] pGlyph[023]
 09 pGlyph[024] pGlyph[025] pGlyph[026]
 10 pGlyph[027] pGlyph[028] pGlyph[029]
 11 pGlyph[030] pGlyph[031] pGlyph[032]
 12 pGlyph[033] pGlyph[034] pGlyph[035]
 13 pGlyph[036] pGlyph[037] pGlyph[038]
 14 pGlyph[039] pGlyph[040] pGlyph[041]
 15 pGlyph[042] pGlyph[043] pGlyph[044]
 16 pGlyph[045] pGlyph[046] pGlyph[047]
 17 pGlyph[048] pGlyph[049] pGlyph[050]
 18 pGlyph[051] pGlyph[052] pGlyph[053]
 19 pGlyph[054] pGlyph[055] pGlyph[056]
 20 pGlyph[057] pGlyph[058] pGlyph[059]
 21 pGlyph[060] pGlyph[061] pGlyph[062]
 22 pGlyph[063] pGlyph[064] pGlyph[065]
 23 pGlyph[066] pGlyph[067] pGlyph[068]
 24 pGlyph[069] pGlyph[070] pGlyph[071]

 フォントの並び(32X32ドット)
    00000000    01111111    11122222    22222333
    12345678    90123456    78901234    56789012
 01 pGlyph[000] pGlyph[001] pGlyph[002] pGlyph[003]
 02 pGlyph[004] pGlyph[005] pGlyph[006] pGlyph[007]
 03 pGlyph[008] pGlyph[009] pGlyph[010] pGlyph[011]
 04 pGlyph[012] pGlyph[013] pGlyph[014] pGlyph[015]
 05 pGlyph[016] pGlyph[017] pGlyph[018] pGlyph[019]
 06 pGlyph[020] pGlyph[021] pGlyph[022] pGlyph[023]
 07 pGlyph[024] pGlyph[025] pGlyph[026] pGlyph[027]
 08 pGlyph[028] pGlyph[029] pGlyph[030] pGlyph[031]
 09 pGlyph[032] pGlyph[033] pGlyph[034] pGlyph[035]
 10 pGlyph[036] pGlyph[037] pGlyph[038] pGlyph[039]
 11 pGlyph[040] pGlyph[041] pGlyph[042] pGlyph[043]
 12 pGlyph[044] pGlyph[045] pGlyph[046] pGlyph[047]
 13 pGlyph[048] pGlyph[049] pGlyph[050] pGlyph[051]
 14 pGlyph[052] pGlyph[053] pGlyph[054] pGlyph[055]
 15 pGlyph[056] pGlyph[057] pGlyph[058] pGlyph[059]
 16 pGlyph[060] pGlyph[061] pGlyph[062] pGlyph[063]
 17 pGlyph[064] pGlyph[065] pGlyph[066] pGlyph[067]
 18 pGlyph[068] pGlyph[069] pGlyph[070] pGlyph[071]
 19 pGlyph[072] pGlyph[073] pGlyph[074] pGlyph[075]
 20 pGlyph[076] pGlyph[077] pGlyph[078] pGlyph[079]
 21 pGlyph[080] pGlyph[081] pGlyph[082] pGlyph[083]
 22 pGlyph[084] pGlyph[085] pGlyph[086] pGlyph[087]
 23 pGlyph[088] pGlyph[089] pGlyph[090] pGlyph[091]
 24 pGlyph[092] pGlyph[093] pGlyph[094] pGlyph[095]
 25 pGlyph[096] pGlyph[097] pGlyph[098] pGlyph[099]
 26 pGlyph[100] pGlyph[101] pGlyph[102] pGlyph[103]
 27 pGlyph[104] pGlyph[105] pGlyph[106] pGlyph[107]
 28 pGlyph[108] pGlyph[109] pGlyph[110] pGlyph[111]
 29 pGlyph[112] pGlyph[113] pGlyph[114] pGlyph[115]
 30 pGlyph[116] pGlyph[117] pGlyph[118] pGlyph[119]
 31 pGlyph[120] pGlyph[121] pGlyph[122] pGlyph[123]
 32 pGlyph[124] pGlyph[125] pGlyph[127] pGlyph[128]

*/

bool GetFontx(FontxFile *fxs, uint8_t ascii , uint8_t *pGlyph, uint8_t *pw, uint8_t *ph)
{
  
	int i;
	uint32_t offset;

	if(FontxDebug)printf("[GetFontx]ascii=0x%x\n",ascii);
	for(i=0; i<2; i++){
	//for(i=0; i<1; i++){
		if(!OpenFontx(&fxs[i])) continue;
		if(FontxDebug)printf("[GetFontx]openFontxFile[%d] ok\n",i);
	
		//if(ascii < 0x100){
		if(ascii < 0x80){
			if(fxs[i].is_ank){
if(FontxDebug)printf("[GetFontx]fxs.is_ank fxs.fsz=%d\n",fxs[i].fsz);
				offset = 17 + ascii * fxs[i].fsz;
if(FontxDebug)printf("[GetFontx]offset=%d\n",offset);
				if(fseek(fxs[i].file, offset, SEEK_SET)) {
					printf("Fontx:seek(%u) failed.\n",offset);
					return false;
				}
				if(fread(pGlyph, 1, fxs[i].fsz, fxs[i].file) != fxs[i].fsz) {
					printf("Fontx:fread failed.\n");
					return false;
				}
				if(pw) *pw = fxs[i].w;
				if(ph) *ph = fxs[i].h;
				return true;
			}

		} else {
#if 0
			if(!fxs[i].is_ank){
				offset = 18;
				if(fseek(fxs[i].file, offset, SEEK_SET)) {
					printf("Fontx:seek(%u) failed.\n",offset);
					return false;
				}
				uint16_t buf[2], nc = 0, bc = fxs[i].bc;

				while(bc--){ 
					if(fread((char *)buf, 1, 4, fxs[i].file) != 4) {
						printf("Fontx:fread failed.\n");
						return false;
					}
if(FontxDebug)printf("[GetFontx]buf=0x%x-0x%x\n",buf[0],buf[1]);
					if(sjis >= buf[0] && sjis <= buf[1]) {
						nc += sjis - buf[0];
						offset = 18 + fxs[i].bc * 4 + nc * fxs[i].fsz;
						if(fseek(fxs[i].file, offset, SEEK_SET)) {
							printf("Fontx:seek(%u) failed.\n",offset);
							return false;
						}
						if(fread(pGlyph, 1, fxs[i].fsz, fxs[i].file) != fxs[i].fsz) {
							printf("Fontx:fread failed.\n");
							return false;
						}
						if(pw) *pw = fxs[i].w;
						if(ph) *ph = fxs[i].h;
						return true;
					}
					nc += buf[1] - buf[0] + 1;
				}
			}
#endif
		}
	}
	return false;
}


/*
 フォントパターンをビットマップイメージに変換する

 fonts(16X16ドット)
    00000000    01111111
    12345678    90123456
 01 pGlyph[000] pGlyph[001]
 02 pGlyph[002] pGlyph[003]
 03 pGlyph[004] pGlyph[005]
 04 pGlyph[006] pGlyph[007]
 05 pGlyph[008] pGlyph[009]
 06 pGlyph[010] pGlyph[011]
 07 pGlyph[012] pGlyph[013]
 08 pGlyph[014] pGlyph[015]
 09 pGlyph[016] pGlyph[017]
 10 pGlyph[018] pGlyph[019]
 11 pGlyph[020] pGlyph[021]
 12 pGlyph[022] pGlyph[023]
 13 pGlyph[024] pGlyph[025]
 14 pGlyph[026] pGlyph[027]
 15 pGlyph[028] pGlyph[029]
 16 pGlyph[030] pGlyph[031]
              
 line[32*4]
 01 line[000] line[001] line[002] .... line[014] line[015] line[016-031]
 |                                                         Not Use
 07 line[000] line[001] line[002] .... line[014] line[015] line[016-031]

 08 line[032] line[033] line[034] .... line[046] line[047] line[048-063]
 |                                                         Not Use
 16 line[032] line[033] line[034] .... line[046] line[047] line[048-063]



 fonts(24X24ドット)
    00000000    01111111    11122222
    12345678    90123456    78901234
 01 pGlyph[000] pGlyph[001] pGlyph[002]
 02 pGlyph[003] pGlyph[004] pGlyph[005]
 03 pGlyph[006] pGlyph[007] pGlyph[008]
 04 pGlyph[009] pGlyph[010] pGlyph[011]
 05 pGlyph[012] pGlyph[013] pGlyph[014]
 06 pGlyph[015] pGlyph[016] pGlyph[017]
 07 pGlyph[018] pGlyph[019] pGlyph[020]
 08 pGlyph[021] pGlyph[022] pGlyph[023]
 09 pGlyph[024] pGlyph[025] pGlyph[026]
 10 pGlyph[027] pGlyph[028] pGlyph[029]
 11 pGlyph[030] pGlyph[031] pGlyph[032]
 12 pGlyph[033] pGlyph[034] pGlyph[035]
 13 pGlyph[036] pGlyph[037] pGlyph[038]
 14 pGlyph[039] pGlyph[040] pGlyph[041]
 15 pGlyph[042] pGlyph[043] pGlyph[044]
 16 pGlyph[045] pGlyph[046] pGlyph[047]
 17 pGlyph[048] pGlyph[049] pGlyph[050]
 18 pGlyph[051] pGlyph[052] pGlyph[053]
 19 pGlyph[054] pGlyph[055] pGlyph[056]
 20 pGlyph[057] pGlyph[058] pGlyph[059]
 21 pGlyph[060] pGlyph[061] pGlyph[062]
 22 pGlyph[063] pGlyph[064] pGlyph[065]
 23 pGlyph[066] pGlyph[067] pGlyph[068]
 24 pGlyph[069] pGlyph[070] pGlyph[071]
              
 line[32*4]
 01 line[000] line[001] line[002] .... line[022] line[023] line[024-031]
 |                                                         Not Use
 08 line[000] line[001] line[002] .... line[022] line[023] line[024-031]

 09 line[032] line[033] line[034] .... line[054] line[055] line[056-063]
 |                                                         Not Use
 16 line[032] line[033] line[034] .... line[054] line[055] line[056-063]

 17 line[064] line[065] line[066] .... line[086] line[087] line[088-095]
 |                                                         Not Use
 24 line[064] line[065] line[066] .... line[086] line[087] line[088-095]


 fonts(32X32ドット)
    00000000    01111111    11122222    22222333
    12345678    90123456    78901234    56789012
 01 pGlyph[000] pGlyph[001] pGlyph[002] pGlyph[003]
 02 pGlyph[004] pGlyph[005] pGlyph[006] pGlyph[007]
 03 pGlyph[008] pGlyph[009] pGlyph[010] pGlyph[011]
 04 pGlyph[012] pGlyph[013] pGlyph[014] pGlyph[015]
 05 pGlyph[016] pGlyph[017] pGlyph[018] pGlyph[019]
 06 pGlyph[020] pGlyph[021] pGlyph[022] pGlyph[023]
 07 pGlyph[024] pGlyph[025] pGlyph[026] pGlyph[027]
 08 pGlyph[028] pGlyph[029] pGlyph[030] pGlyph[031]
 09 pGlyph[032] pGlyph[033] pGlyph[034] pGlyph[035]
 10 pGlyph[036] pGlyph[037] pGlyph[038] pGlyph[039]
 11 pGlyph[040] pGlyph[041] pGlyph[042] pGlyph[043]
 12 pGlyph[044] pGlyph[045] pGlyph[046] pGlyph[047]
 13 pGlyph[048] pGlyph[049] pGlyph[050] pGlyph[051]
 14 pGlyph[052] pGlyph[053] pGlyph[054] pGlyph[055]
 15 pGlyph[056] pGlyph[057] pGlyph[058] pGlyph[059]
 16 pGlyph[060] pGlyph[061] pGlyph[062] pGlyph[063]
 17 pGlyph[064] pGlyph[065] pGlyph[066] pGlyph[067]
 18 pGlyph[068] pGlyph[069] pGlyph[070] pGlyph[071]
 19 pGlyph[072] pGlyph[073] pGlyph[074] pGlyph[075]
 20 pGlyph[076] pGlyph[077] pGlyph[078] pGlyph[079]
 21 pGlyph[080] pGlyph[081] pGlyph[082] pGlyph[083]
 22 pGlyph[084] pGlyph[085] pGlyph[086] pGlyph[087]
 23 pGlyph[088] pGlyph[089] pGlyph[090] pGlyph[091]
 24 pGlyph[092] pGlyph[093] pGlyph[094] pGlyph[095]
 25 pGlyph[096] pGlyph[097] pGlyph[098] pGlyph[099]
 26 pGlyph[100] pGlyph[101] pGlyph[102] pGlyph[103]
 27 pGlyph[104] pGlyph[105] pGlyph[106] pGlyph[107]
 28 pGlyph[108] pGlyph[109] pGlyph[110] pGlyph[111]
 29 pGlyph[112] pGlyph[113] pGlyph[114] pGlyph[115]
 30 pGlyph[116] pGlyph[117] pGlyph[118] pGlyph[119]
 31 pGlyph[120] pGlyph[121] pGlyph[122] pGlyph[123]
 32 pGlyph[124] pGlyph[125] pGlyph[127] pGlyph[128]
              
 line[32*4]
 01 line[000] line[001] line[002] .... line[030] line[031]
 |
 08 line[000] line[001] line[002] .... line[030] line[031]

 09 line[032] line[033] line[034] .... line[062] line[063]
 |
 16 line[032] line[033] line[034] .... line[062] line[063]

 17 line[064] line[065] line[066] .... line[094] line[095]
 |
 24 line[064] line[065] line[066] .... line[094] line[095]

 25 line[096] line[097] line[098] .... line[126] line[127]
 |
 32 line[096] line[097] line[098] .... line[126] line[127]

*/
void Font2Bitmap(uint8_t *fonts, uint8_t *line, uint8_t w, uint8_t h, uint8_t inverse) {
	int x,y;
	for(y=0; y<(h/8); y++){
		for(x=0; x<w; x++){
			line[y*32+x] = 0;
		}
	}

	int mask = 7;
	int fontp;
	fontp = 0;
	for(y=0; y<h; y++){
		for(x=0; x<w; x++){
			uint8_t d = fonts[fontp+x/8];
			uint8_t linep = (y/8)*32+x;
			if (d & (0x80 >> (x % 8))) line[linep] = line[linep] + (1 << mask);
		}
		mask--;
		if (mask < 0) mask = 7;
		fontp += (w + 7)/8;
	}

	if (inverse) {
		for(y=0; y<(h/8); y++){
			for(x=0; x<w; x++){
				line[y*32+x] = RotateByte(line[y*32+x]);
			}
		}
	}
}

// アンダーラインを追加
void UnderlineBitmap(uint8_t *line, uint8_t w, uint8_t h) {
	int x,y;
	uint8_t wk;
	for(y=0; y<(h/8); y++){
		for(x=0; x<w; x++){
			wk = line[y*32+x];
			if ( (y+1) == (h/8)) line[y*32+x] = wk + 0x80;
		}
	}
}

// ビットマップを反転
void ReversBitmap(uint8_t *line, uint8_t w, uint8_t h) {
	int x,y;
	uint8_t wk;
	for(y=0; y<(h/8); y++){
		for(x=0; x<w; x++){
			wk = line[y*32+x];
			line[y*32+x] = ~wk;
		}
	}
}

// フォントパターンの表示
void ShowFont(uint8_t *fonts, uint8_t pw, uint8_t ph) {
	int x,y,fpos;
	printf("[ShowFont pw=%d ph=%d]\n",pw,ph);
	fpos=0;
	for (y=0;y<ph;y++) {
		printf("%02d",y);
		for (x=0;x<pw;x++) {
			if (fonts[fpos+x/8] & (0x80 >> (x % 8))) {
			printf("*");
			} else {
				printf(".");
			}
		}
		printf("\n");
		fpos=fpos+(pw+7)/8;
	}
	printf("\n");
}

// Bitmapの表示
void ShowBitmap(uint8_t *bitmap, uint8_t pw, uint8_t ph) {
	int x,y,fpos;
	printf("[ShowBitmap pw=%d ph=%d]\n",pw,ph);
#if 0
	for (y=0;y<(ph+7)/8;y++) {
		for (x=0;x<pw;x++) {
			printf("%02x ",bitmap[x+y*32]);
		}
		printf("\n");
	}
#endif

	fpos=0;
	for (y=0;y<ph;y++) {
		printf("%02d",y);
		for (x=0;x<pw;x++) {
//printf("b=%x m=%x\n",bitmap[x+(y/8)*32],0x80 >> fpos);
			if (bitmap[x+(y/8)*32] & (0x80 >> fpos)) {
				printf("*");
			} else {
				printf(".");
			}
		}
		printf("\n");
		fpos++;
		if (fpos > 7) fpos = 0;
	}
	printf("\n");
}


// 8ビットデータを反転
uint8_t RotateByte(uint8_t ch1) {
	uint8_t ch2 = 0;
	int j;
	for (j=0;j<8;j++) {
		ch2 = (ch2 << 1) + (ch1 & 0x01);
		ch1 = ch1 >> 1;
	}
	return ch2;
}


#if 0
// UTF code(3Byte) を SJIS Code(2 Byte) に変換
// https://www.mgo-tec.com/blog-entry-utf8sjis01.html
uint16_t UTF2SJIS(spiffs_file fd, uint8_t *utf8) {

  uint32_t offset = 0;
  uint32_t ret;
  uint32_t UTF8uint = utf8[0]*256*256 + utf8[1]*256 + utf8[2];
   
  if(utf8[0]>=0xC2 && utf8[0]<=0xD1){ //0xB0からS_JISコード実データ。0x00-0xAFまではライセンス文ヘッダなのでそれをカット。
    offset = ((utf8[0]*256 + utf8[1])-0xC2A2)*2 + 0xB0; //文字"￠" UTF8コード C2A2～、S_jisコード8191
  }else if(utf8[0]==0xE2 && utf8[1]>=0x80){
    offset = (UTF8uint-0xE28090)*2 + 0x1EEC; //文字"‐" UTF8コード E28090～、S_jisコード815D
  }else if(utf8[0]==0xE3 && utf8[1]>=0x80){
    offset = (UTF8uint-0xE38080)*2 + 0x9DCC; //スペース UTF8コード E38080～、S_jisコード8140
  }else if(utf8[0]==0xE4 && utf8[1]>=0x80){
    offset = (UTF8uint-0xE4B880)*2 + 0x11CCC; //文字"一" UTF8コード E4B880～、S_jisコード88EA
  }else if(utf8[0]==0xE5 && utf8[1]>=0x80){
    offset = (UTF8uint-0xE58085)*2 + 0x12BCC; //文字"倅" UTF8コード E58085～、S_jisコード98E4
  }else if(utf8[0]==0xE6 && utf8[1]>=0x80){
    offset = (UTF8uint-0xE6808E)*2 + 0x1AAC2; //文字"怎" UTF8コード E6808E～、S_jisコード9C83
  }else if(utf8[0]==0xE7 && utf8[1]>=0x80){
    offset = (UTF8uint-0xE78081)*2 + 0x229A6; //文字"瀁" UTF8コード E78081～、S_jisコードE066
  }else if(utf8[0]==0xE8 && utf8[1]>=0x80){
    offset = (UTF8uint-0xE88080)*2 + 0x2A8A4; //文字"耀" UTF8コード E88080～、S_jisコード9773
  }else if(utf8[0]==0xE9 && utf8[1]>=0x80){
    offset = (UTF8uint-0xE98080)*2 + 0x327A4; //文字"退" UTF8コード E98080～、S_jisコード91DE
  }else if(utf8[0]>=0xEF && utf8[1]>=0xBC){
    offset = (UTF8uint-0xEFBC81)*2 + 0x3A6A4; //文字"！" UTF8コード EFBC81～、S_jisコード8149
    if(utf8[0]==0xEF && utf8[1]==0xBD && utf8[2]==0x9E){
      offset = 0x3A8DE; // "～" UTF8コード EFBD9E、S_jisコード8160
    }
  }

if(FontxDebug)printf("[UTF2SJIS] offset=%d\n",offset);
  char buf[2];
  ret = SPIFFS_lseek(&fs, fd, offset, SPIFFS_SEEK_SET);
if(FontxDebug)printf("[UTF2SJIS] lseek ret=%d\n",ret);
  if (ret != offset) {
    printf("UTF2SJIS:seek(%u) failed.\n",offset);
    return 0;
  }
  if (SPIFFS_read(&fs, fd, buf, sizeof(buf)) != sizeof(buf)) {
    printf("UTF2SJIS:read failed.\n");
    return 0;
  }
if(FontxDebug)printf("[UTF2SJIS] sjis=0x%x%x\n",buf[0],buf[1]);
  return buf[0]*256+buf[1];
}


// UTFを含む文字列をSJISに変換
int String2SJIS(spiffs_file fd, unsigned char *str_in, size_t stlen,
		uint16_t *sjis, size_t ssize) {
  int i;
  uint8_t sp;
  uint8_t c1 = 0;
  uint8_t c2 = 0;
  uint8_t utf8[3];
  uint16_t sjis2;
  int spos = 0;

  for(i=0;i<stlen;i++) {
    sp = str_in[i];
if(FontxDebug)printf("[String2SJIS]sp[%d]=%x\n",i,sp);
    if ((sp & 0xf0) == 0xe0) { // 上位4ビットが1110なら、3バイト文字の1バイト目
      c1 = sp;
    } else if ((sp & 0xc0) == 0x80) { // 上位2ビットが10なら、他バイト文字の2バイト目以降
      if (c2 == 0) {
        c2 = sp;
      } else {
        if (c1 == 0xef && c2 == 0xbd) {
if(FontxDebug)printf("[String2SJIS]hankaku kana %x-%x-%x\n",c1,c2,sp);
          sjis2 = sp;
if(FontxDebug)printf("[String2SJIS]sjis2=%x\n",sjis2);
          if (spos < ssize) sjis[spos++] = sjis2;
        } else if (c1 == 0xef && c2 == 0xbe) {
if(FontxDebug)printf("[String2SJIS]hankaku kana %x-%x-%x\n",c1,c2,sp);
          sjis2 = 0xc0 + (sp - 0x80);
if(FontxDebug)printf("[String2SJIS]sjis2=%x\n",sjis2);
          if (spos < ssize) sjis[spos++] = sjis2;
        } else {
if(FontxDebug)printf("[String2SJIS]UTF8 %x-%x-%x\n",c1,c2,sp);
          utf8[0] = c1;
          utf8[1] = c2;
          utf8[2] = sp;
          sjis2 = UTF2SJIS(fd, utf8);
if(FontxDebug)printf("[String2SJIS]sjis2=%x\n",sjis2);
          if (spos < ssize) sjis[spos++] = sjis2;
        }
        c1 = c2 = 0;
      }
    } else if ((sp & 0x80) == 0) { // 1バイト文字の場合
if(FontxDebug)printf("[String2SJIS]ANK %x\n",sp);
        if (spos < ssize) sjis[spos++] = sp;
    }
  }
  return spos;
}
#endif


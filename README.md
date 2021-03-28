# esp-idf-st7789
ST7789 Driver for esp-idf

The demo video by Dmitry Andreev.   
https://www.youtube.com/watch?v=aOyaK0pUiPk&t

# Installation for ESP32

```
git clone https://github.com/nopnop2002/esp-idf-st7789
cd esp-idf-st7789/
idf.py set-target esp32
idf.py menuconfig
idf.py flash
```

# Installation for ESP32-S2

```
git clone https://github.com/nopnop2002/esp-idf-st7789
cd esp-idf-st7789/
idf.py set-target esp32s2
idf.py menuconfig
idf.py flash
```


You have to set this config value with menuconfig.   
- CONFIG_WIDTH   
- CONFIG_HEIGHT   
- CONFIG_OFFSETX   
- CONFIG_OFFSETY   
- CONFIG_MOSI_GPIO   
- CONFIG_SCLK_GPIO   
- CONFIG_CS_GPIO   
- CONFIG_DC_GPIO   
- CONFIG_RESET_GPIO   
- CONFIG_BL_GPIO   

![st7789-config-1](https://user-images.githubusercontent.com/6020549/59019692-7d8d0d00-8883-11e9-996a-24253105ff89.jpg)

---

# Generic ST7789 1.3 Inch

There is 2 kinds of marking.   
![st7789-back](https://user-images.githubusercontent.com/6020549/59020150-5f73dc80-8884-11e9-8272-99525f956b9d.JPG)

![st7789-config-2](https://user-images.githubusercontent.com/6020549/64900167-50de6200-d6ca-11e9-8d64-2096b230e2af.jpg)

__MISO is not use.__   

![st7789-1](https://user-images.githubusercontent.com/6020549/58755161-fce3af00-8518-11e9-8833-2c90ffe4be42.JPG)
![st7789-2](https://user-images.githubusercontent.com/6020549/58755162-fce3af00-8518-11e9-97cb-32502d1a7206.JPG)
![st7789-3](https://user-images.githubusercontent.com/6020549/58755163-fd7c4580-8518-11e9-8491-ff0927f3fc15.JPG)
![st7789-4](https://user-images.githubusercontent.com/6020549/58755164-fd7c4580-8518-11e9-8760-90f3d8fb6fdf.JPG)
![st7789-5](https://user-images.githubusercontent.com/6020549/58755165-fd7c4580-8518-11e9-945d-0066d78be413.JPG)
![st7789-6](https://user-images.githubusercontent.com/6020549/58755166-fd7c4580-8518-11e9-81ae-7a8b56f57bb2.JPG)
![st7789-7](https://user-images.githubusercontent.com/6020549/58755167-fe14dc00-8518-11e9-97d5-66ddc6c59738.JPG)
![st7789-8](https://user-images.githubusercontent.com/6020549/58755168-fe14dc00-8518-11e9-9c25-4be71ac4943c.JPG)
![st7789-9](https://user-images.githubusercontent.com/6020549/58755159-fce3af00-8518-11e9-9548-062fad44d0c9.JPG)
![st7789-10](https://user-images.githubusercontent.com/6020549/58755160-fce3af00-8518-11e9-8ced-813b7e6cc06f.JPG)

BMP file   
![st7789-11](https://user-images.githubusercontent.com/6020549/72672303-56deb800-3a9b-11ea-8c64-bee19fb6771c.JPG)

JPEG file(ESP32 only)   
![st7789-JPEG](https://user-images.githubusercontent.com/6020549/78464976-3255bc00-772b-11ea-9d5a-328754430489.JPG)

PNG file   
![st7789-PNG](https://user-images.githubusercontent.com/6020549/78632596-64f1e700-78da-11ea-96bd-c773ff8de4a2.JPG)

---

# Generic ST7789 1.14 Inch

![st7789-135x240-1](https://user-images.githubusercontent.com/6020549/66249793-ca221f80-e773-11e9-94b9-ea33c054e0eb.jpg)
![st7789-135x240-2](https://user-images.githubusercontent.com/6020549/66249792-ca221f80-e773-11e9-9dee-a4d03b2990d3.JPG)

---

# LILYGO TTGO 1.14 Inch

![ttgo-config-1](https://user-images.githubusercontent.com/6020549/64899699-a402e580-d6c7-11e9-9b09-e2a27c82f0fa.jpg)
![ttgo-1](https://user-images.githubusercontent.com/6020549/64899702-a6653f80-d6c7-11e9-8621-fd66393fcf42.JPG)

---

# LILYGO TTGO T8 ESP32-S2

![TTGO_T8_ESP32-S2-1](https://user-images.githubusercontent.com/6020549/100829346-7b216880-34a4-11eb-806d-807c5e497124.jpg)
![TTGO_T8_ESP32-S2-2](https://user-images.githubusercontent.com/6020549/100829356-82487680-34a4-11eb-9a1f-d132ba278ad2.JPG)

---

# JPEG Decoder   
The ESP-IDF component includes Tiny JPEG Decompressor.   
The document of Tiny JPEG Decompressor is [here](http://elm-chan.org/fsw/tjpgd/00index.html).   
This can reduce the image to 1/2 1/4 1/8.   

---

# PNG Decoder   
The ESP-IDF component includes part of the miniz library, such as mz_crc32.   
But it doesn't support all of the miniz.   
The document of miniz library is [here](https://github.com/richgel999/miniz).   

And I ported the pngle library from [here](https://github.com/kikuchan/pngle).   
This can reduce the image to any size.   

---

# Font File   
You can add your original fonts.   
The format of the font file is the FONTX format.   
Your font file is put in font directory.   
Your font file is uploaded to SPIFFS partition using meke flash.   

Please refer [this](http://elm-chan.org/docs/dosv/fontx_e.html) page about FONTX format.   

```
FontxFile yourFont[2];
InitFontx(yourFont,"/spiffs/your_font_file_name","");
uint8_t ascii[10];
strcpy((char *)ascii, "MyFont");
uint16_t color = RED;
lcdDrawString(&dev, yourFont, x, y, ascii, color);
```

---

# Font File Editor(FONTX Editor)   
[There](http://elm-chan.org/fsw/fontxedit.zip) is a font file editor.   
This can be done on Windows 10.   
Developer page is [here](http://elm-chan.org/fsw_e.html).   

![FontxEditor](https://user-images.githubusercontent.com/6020549/78731275-3b889800-797a-11ea-81ba-096dbf07c4b8.png)

This library uses the following as default fonts:   
- font/ILGH16XB.FNT // 8x16Dot Gothic
- font/ILGH24XB.FNT // 12x24Dot Gothic
- font/ILGH32XB.FNT // 16x32Dot Gothic
- font/ILMH16XB.FNT // 8x16Dot Mincyo
- font/ILMH24XB.FNT // 12x24Dot Mincyo
- font/ILMH32XB.FNT // 16x32Dot Mincyo

From 0x00 to 0x7f, the characters image of Alphanumeric are stored.   
From 0x80 to 0xff, the characters image of Japanese are stored.   
Changing this file will change the font.

# How to build your own font file   
step1) download fontxedit.exe.   

step2) download BDF font file from Internet. I downloaded from [here](https://github.com/fcambus/spleen).   

step3) import one of them into your fontxedit.exe. This tool can convert from BDF to FONTX.   
![FONTX-EDITTOR-1](https://user-images.githubusercontent.com/6020549/112736427-d7e5e900-8f95-11eb-80d5-11dd9df42903.jpg)

step4) adjust font size.   
![FONTX-EDITTOR-2](https://user-images.githubusercontent.com/6020549/112736434-e6cc9b80-8f95-11eb-8b8e-b523746c1c96.jpg)

step5) check font pattern.   
![FONTX-EDITTOR-3](https://user-images.githubusercontent.com/6020549/112736437-edf3a980-8f95-11eb-946a-93426b62c137.jpg)

step6) save as .fnt file from your fontedit.exe.   
![FONTX-EDITTOR-4](https://user-images.githubusercontent.com/6020549/112736443-095eb480-8f96-11eb-8516-b03e10aeadbe.jpg)

step7) upload your font file to $HOME/esp-idf-st7789/fonts directory.   

step8) change font to use   
```
// For JAPANESE
InitFontx(fx32G,"/spiffs/ILGH32XB.FNT",""); // 16x32Dot Gothic
// For LATIN
//InitFontx(fx32G,"/spiffs/LATAN32B.FNT",""); // 16x32Dot Gothic
```

Font file that From 0x80 to 0xff, the characters image of Japanese are stored.   
![st7789-KANA](https://user-images.githubusercontent.com/6020549/112739904-205fcf80-8fb3-11eb-823c-5410570e068a.JPG)

Font file that From 0x80 to 0xff, the characters image of Latin are stored.   
![st7789-LATIN](https://user-images.githubusercontent.com/6020549/112739906-25248380-8fb3-11eb-960f-a483a7e078b5.JPG)

# Other method   
[Here](http://sofia.nmsu.edu/~mleisher/Software/otf2bdf/) is library to generate BDF bitmap fonts from OpenType.   
Convert OpenType to BDF and then convert from BDF to FONTX.   

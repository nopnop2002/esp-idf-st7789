# esp-idf-st7789
ST7789 Driver for esp-idf

The demo video by Dmitry Andreev.   
https://www.youtube.com/watch?v=aOyaK0pUiPk&t

# Software requirements
ESP-IDF V4.4/V5.0 or later.   

# Installation for ESP-IDF V4.4
```Shell
git clone -b v4.4 https://github.com/nopnop2002/esp-idf-st7789
cd esp-idf-st7789/
idf.py set-target {esp32/esp32s2/esp32s3/esp32c3}
idf.py menuconfig
idf.py flash
```

__Note for ESP32-S2__   
The tjpgd library is not included in the ESP32-S2 ROM because the ROM of the ESP32-S2 is small.   
Therefore, JPEG files cannot be displayed.   

__Note for ESP32-C3__   
For some reason, there are development boards that cannot use GPIO06, GPIO08, GPIO09, GPIO19 for SPI clock pins.   
According to the ESP32C3 specifications, these pins can also be used as SPI clocks.   
I used a raw ESP-C3-13 to verify that these pins could be used as SPI clocks.   


# Installation for ESP-IDF V5.x

```Shell
git clone https://github.com/nopnop2002/esp-idf-st7789
cd esp-idf-st7789/
idf.py set-target {esp32/esp32s2/esp32s3/esp32c2/esp32c3}
idf.py menuconfig
idf.py flash
```

__Note for ESP32-S2/ESP32-C2__   
The tjpgd library is not included in the ESP32-S2/ESP32-C2 ROM.   
However, you can use [this](https://components.espressif.com/components/espressif/esp_jpeg) IDF component registry.   
JPEG files can be displayed.   


# Configuration   

![config-top](https://user-images.githubusercontent.com/6020549/202874786-d0072c20-9313-4095-babd-1e6a33075e4e.jpg)

# Generic ST7789 1.3 Inch

There is 2 kinds of marking.   
The pin marking is written as SCL/SDA, so it looks like i2c, but it is SPI.   

![st7789-back](https://user-images.githubusercontent.com/6020549/59020150-5f73dc80-8884-11e9-8272-99525f956b9d.JPG)

![config-240x240](https://user-images.githubusercontent.com/6020549/202874791-98df2cdc-804b-4c2f-8703-8eaf4028f393.jpg)

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

JPEG file(ESP32/ESP32S2/ESP32C3)   
![st7789-JPEG](https://user-images.githubusercontent.com/6020549/78464976-3255bc00-772b-11ea-9d5a-328754430489.JPG)

PNG file   
![st7789-PNG](https://user-images.githubusercontent.com/6020549/78632596-64f1e700-78da-11ea-96bd-c773ff8de4a2.JPG)

QR-CODE   
![st7789-QRCODE](https://user-images.githubusercontent.com/6020549/181865160-a872ac62-35d3-4e40-87ff-f81b6847f69d.JPG)

# Generic ST7789 1.14 Inch

![config-135x240](https://user-images.githubusercontent.com/6020549/202874847-bb5ee2d2-4c87-4192-8aa7-6fbc0a6f85e7.jpg)
![st7789-135x240](https://user-images.githubusercontent.com/6020549/202874959-f41aa789-4f85-4d88-8846-9895a6a9888a.JPG)

# SPI BUS selection   
![config-spi-bus](https://user-images.githubusercontent.com/6020549/202875013-ad2ce3d4-6a2b-458b-9542-f3a17e79d5b1.jpg)

The ESP32 series has three SPI BUSs.   
SPI1_HOST is used for communication with Flash memory.   
You can use SPI2_HOST and SPI3_HOST freely.   
When you use SDSPI(SD Card via SPI), SDSPI uses SPI2_HOST BUS.   
When using this module at the same time as SDSPI or other SPI device using SPI2_HOST, it needs to be changed to SPI3_HOST.   
When you don't use SDSPI, both SPI2_HOST and SPI3_HOST will work.   
Previously it was called HSPI_HOST / VSPI_HOST, but now it is called SPI2_HOST / SPI3_HOST.   


# About 7Pin breakout
There are two types of ST7789 breakouts.   
One has a CS pin and one does not have a CS pin.   
I think there is an electrical problem __around reset circuit__ in the breakout without the CS pin.   

I pulled up RESET by inserting a 100 ohm resistor between Vcc and RESET.   

![TroubleShooting](https://user-images.githubusercontent.com/6020549/167105707-20799cc3-0f01-4815-aecf-829d0257122a.JPG)

Perhaps the reset circuit requires a lot of current.   
Ultimately I ended up using the following circuit.   
And press the reset button several times.

```
ESP32 3V3  -------------------------------+
                                          |
                                          | Collector
                                         /
                                        /
ESP32 RES  ------------R(2.2K)---------|    SS8050
                                Base    \
                                         \
                                          | Emitter
                                          |
                                          +------------------------- ST77889 RST
                                          |
                                          |
ESP32 GND  ------------R(220)-------------+
```

If you know the cause, please let me know.



# LILYGO TTGO 1.14 Inch ESP32

![ttgo-1](https://user-images.githubusercontent.com/6020549/202874897-9d06ddf2-b392-44a0-aea1-55884767c9f0.jpg)
![ttgo-2](https://user-images.githubusercontent.com/6020549/202874898-80e4a195-b690-4425-8bb9-94346533ce5a.JPG)


# LILYGO TTGO T8 ESP32-S2

![TTGO_T8_ESP32-S2-1](https://user-images.githubusercontent.com/6020549/202875184-6c3890a6-d20a-4f35-8bdd-0c4a980159a7.jpg)
![TTGO_T8_ESP32-S2-2](https://user-images.githubusercontent.com/6020549/100829356-82487680-34a4-11eb-9a1f-d132ba278ad2.JPG)



# JPEG Decoder   
The ESP-IDF component includes Tiny JPEG Decompressor.   
The document of Tiny JPEG Decompressor is [here](http://elm-chan.org/fsw/tjpgd/00index.html).   
This can reduce the image to 1/2 1/4 1/8.   


# PNG Decoder   
The ESP-IDF component includes part of the miniz library, such as mz_crc32.   
But it doesn't support all of the miniz.   
The document of miniz library is [here](https://github.com/richgel999/miniz).   

And I ported the pngle library from [here](https://github.com/kikuchan/pngle).   
This can reduce the image to any size.   


# Font File   
You can add your original fonts.   
The format of the font file is the FONTX format.   
Your font file is put in font directory.   
Your font file is uploaded to SPIFFS partition using meke flash.   

Please refer [this](http://elm-chan.org/docs/dosv/fontx_e.html) page about FONTX format.   


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
step1)   
download fontxedit.exe.   

step2)   
download BDF font file from Internet.   
I downloaded from [here](https://github.com/fcambus/spleen).   
fontxedit.exe can __ONLY__ import Monospaced bitmap fonts file.   
Monospaced bitmap fonts can also be downloaded [here](https://github.com/Tecate/bitmap-fonts).

step3)   
import the BDF font file into your fontxedit.exe.   
this tool can convert from BDF to FONTX.   
![FONTX-EDITTOR-1](https://user-images.githubusercontent.com/6020549/112736427-d7e5e900-8f95-11eb-80d5-11dd9df42903.jpg)

step4)   
adjust font size.   
![FONTX-EDITTOR-2](https://user-images.githubusercontent.com/6020549/112736434-e6cc9b80-8f95-11eb-8b8e-b523746c1c96.jpg)

step5)   
check font pattern.   
![FONTX-EDITTOR-13](https://user-images.githubusercontent.com/6020549/112746492-11e0da80-8fea-11eb-94f1-8d299b2dc756.jpg)

step6)   
save as .fnt file from your fontedit.exe.   
![FONTX-EDITTOR-14](https://user-images.githubusercontent.com/6020549/112746501-2329e700-8fea-11eb-9a3a-4481c1a14ddc.jpg)

step7)   
upload your font file to $HOME/esp-idf-st7789/fonts directory.   

step8)   
add font to use   
```
FontxFile fx32L[2];
InitFontx(fx32L,"/spiffs/LATIN32B.FNT",""); // 16x32Dot LATIN
```

Font file that From 0x80 to 0xff, the characters image of Japanese are stored.   
![st7789-KANA](https://user-images.githubusercontent.com/6020549/112739904-205fcf80-8fb3-11eb-823c-5410570e068a.JPG)

Font file that From 0x80 to 0xff, the characters image of Latin are stored.   
![st7789-LATIN](https://user-images.githubusercontent.com/6020549/112739906-25248380-8fb3-11eb-960f-a483a7e078b5.JPG)


# External input module   

You can use these if you need some input.   
- MPR121 Capacitive Touch switch   
https://github.com/nopnop2002/esp-idf-mpr121

- TTP229 Capacitive Touch switch   
https://github.com/nopnop2002/esp-idf-ttp229



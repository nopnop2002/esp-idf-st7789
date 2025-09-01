# esp-idf-st7789
ST7789 Driver for esp-idf

The demo video by Dmitry Andreev.   
https://www.youtube.com/watch?v=aOyaK0pUiPk&t

# Software requirements
ESP-IDF V5.0 or later.   
ESP-IDF V4.4 release branch reached EOL in July 2024.   

__Note for ESP32-C6__   
ESP-IDF V5.1 is required when using ESP32-C6.   

# Installation

```Shell
git clone https://github.com/nopnop2002/esp-idf-st7789
cd esp-idf-st7789/
idf.py set-target {esp32/esp32s2/esp32s3/esp32c2/esp32c3/esp32c6}
idf.py menuconfig
idf.py flash
```

__Note for ESP32-S2/ESP32-C2__   
The tjpgd library is not included in the ESP32-S2/ESP32-C2 ROM.   
However, you can use [this](https://components.espressif.com/components/espressif/esp_jpeg) IDF component registry.   
JPEG files can be displayed.   

__Note for ESP32-C2__   
ESP32-C2 has less SRAM, so JPEG and PNG may not be displayed on higher resolution TFTs.   
```
E (256560) decode_jpeg: Error allocating memory for line 251
E (260630) pngle_new: Error allocating memory for line 160
```

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

PNG icon   
![st7789-11](https://github.com/user-attachments/assets/63d85b89-f3eb-4fbc-93cc-61d2d4571d2e)

# Configuration   

![config-top](https://user-images.githubusercontent.com/6020549/202874786-d0072c20-9313-4095-babd-1e6a33075e4e.jpg)

## Generic ST7789 1.3 Inch

There is 2 kinds of marking.   
The pin marking is written as SCL/SDA, so it looks like i2c, but it is SPI.   
SCL is SCLK, SDA is MOSI.   
![st7789-back](https://user-images.githubusercontent.com/6020549/59020150-5f73dc80-8884-11e9-8272-99525f956b9d.JPG)

![config-1_3inch](https://github.com/user-attachments/assets/2c0da7b4-b6b6-46de-892a-0ac44c4bfb2b)

__Note for display remains black__   
This module may require a large current to reset.   
This may be improved by following the steps below.   
1. Pull up RES with 100 ohms.   
2. Write the firmware.   
3. Press the reset button several times.   

![PullUp-Register](https://github.com/user-attachments/assets/653cf7fb-29e9-4f69-98e0-f9eb6e32171a)


## Generic ST7789 1.14 Inch

![st7789-135x240-2](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/fd10b6db-27b1-496d-97ee-2f8f9af5f660)
![st7789-135x240-1](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/86d31590-7c35-413c-8dfc-c73509a0e12f)

## Generic ST7789 1.54 Inch (Non IPS)
Left:1.54inch Right:1.3inch   
![config_1_54inch-1](https://github.com/user-attachments/assets/1b29c771-5b1f-4a72-a42d-bb1f894eab56)
![config_1_54inch-2](https://github.com/user-attachments/assets/99373809-816c-42f0-85e8-17292e5a44b9)

## Generic ST7789 1.9 Inch (Non IPS)

![st7789-170x320-1](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/99db2a5f-c64c-497f-94a3-dc3268d86f41)
![st7789-170x320-2](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/2ce847ef-00ee-406d-bf9c-e6d41c53f01d)
![st7789-170x320-3](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/b1dad0db-4896-4cf5-a0ae-2410998110f7)
![config](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/b5f03dcc-a3a0-4fbe-b015-96d0250682c7)

## Generic ST7789 2.0 Inch (Non IPS)
Left:2.0inch Right:1.54inch   
![st7789-240x320-3](https://github.com/user-attachments/assets/938b416e-5f3e-4312-b8a4-c5527aecf6f6)
![st778-240x320-1](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/e497d802-5d6a-4a91-8827-cfeb0e0ec6ad)
![st778-240x320-2](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/18bda7f0-c6a1-4d75-a2c8-b73d14dc4773)
![st7789-240x320-4](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/546cb8f5-ff7f-482e-8032-e7acf16da766)

## Generic ST7789 2.4 Inch (Non IPS)
Left:2.4inch Right:2.0inch   
![config_2_4inch-1](https://github.com/user-attachments/assets/5fd0953e-9cef-40e5-adf5-2fc35607b78c)
![config_2_4inch-2](https://github.com/user-attachments/assets/d5ad926d-6166-4086-a7ac-d0242b166aa3)

## Generic ST7789 2.8 Inch (Non IPS)
Left:2.4inch Right:2.8inch   
![2_8inch-1](https://github.com/user-attachments/assets/022875ed-dbcd-46f9-b041-6765782292eb)
![2_8inch-2](https://github.com/user-attachments/assets/db5c037a-5116-49c1-a4da-9cd06e6d5cc1)

## LILYGO TTGO 1.14 Inch ESP32

![ttgo-1](https://user-images.githubusercontent.com/6020549/202874897-9d06ddf2-b392-44a0-aea1-55884767c9f0.jpg)
![ttgo-2](https://user-images.githubusercontent.com/6020549/202874898-80e4a195-b690-4425-8bb9-94346533ce5a.JPG)

## TENSTAR T-Display 1.14 Inch ESP32(LILYGO TTGO clone)

![ttgo-1](https://user-images.githubusercontent.com/6020549/202874897-9d06ddf2-b392-44a0-aea1-55884767c9f0.jpg)
![Image](https://github.com/user-attachments/assets/7ab1c350-b11b-42f6-9aa9-64197a8d346d)

## LILYGO TTGO T8 ESP32-S2

![TTGO_T8_ESP32-S2-1](https://user-images.githubusercontent.com/6020549/202875184-6c3890a6-d20a-4f35-8bdd-0c4a980159a7.jpg)
![TTGO_T8_ESP32-S2-2](https://user-images.githubusercontent.com/6020549/100829356-82487680-34a4-11eb-9a1f-d132ba278ad2.JPG)

## Custom PCB
Click [here](https://github.com/nopnop2002/esp-idf-st7789/tree/master/CustomPCB) for more information.   

![esp32s2-pcb-1](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/eadf0272-11da-467b-b2e3-fd322365053b)
![esp32s2_pcb-2](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/1de19ace-b953-4fb8-8a8b-13c31e9ef4af)

# SPI Clock speed   
According to the ST7789 datasheet, the minimum SPI clock cycle time is 16ns.   
Therefore, the maximum SPI clock frequency is 62.5MHz.   
The SPI clock frequency used by this project is 20MHz.   
Higher SPI clock frequencies can be specified using ```spi_clock_speed()```.   
When using higher SPI clock frequencies, you need to be careful about the length of the wire cable.   
```
    //int speed = 40000000; // 40MHz
    int speed = 60000000; // 60MHz
    spi_clock_speed(speed);
    spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO, CONFIG_BL_GPIO);
    lcdInit(&dev, CONFIG_WIDTH, CONFIG_HEIGHT, CONFIG_OFFSETX, CONFIG_OFFSETY);
```

- Benchmarking using ESP32@240 & 1.3 inch TFT Without Frame Buffer.   
 Clock up has little effect.   

||20MHz|40MHz|60MHz|
|:-:|:-:|:-:|:-:|
|FillTest|1150|1090|1090|
|ColorBarTest|50|50|50|
|ArrowTest|280|250|250|
|LineTest|2190|2150|2150|
|CircleTest|1940|1910|1910|
|RoundRectTest|1980|1940|1940|
|DirectionTest|450|430|430|
|HorizontalTest|1070|1040|1040|
|VerticalTest|1070|1040|1040|
|FillRectTest|190|150|150|
|ColorTest|260|220|220|
|CodeTest|1070|1040|1040|
|BMPTest|7160|7130|7130|
|JPEGTest|2550|2530|2530|
|PNGTest|2850|2830|2830|
|QRTest|220|170|170|

- Benchmarking using ESP32@240 & 1.3 inch TFT With Frame Buffer.   
 The effect of clocking up varies depending on the test case.   

||20MHz|40MHz|60MHz|
|:-:|:-:|:-:|:-:|
|FillTest|1150|1090|1090|
|ColorBarTest|70|50|50|
|ArrowTest|60|30|30|
|LineTest|50|30|30|
|CircleTest|60|30|30|
|RoundRectTest|50|30|30|
|DirectionTest|60|40|40|
|HorizontalTest|70|50|50|
|VerticalTest|70|50|50|
|FillRectTest|60|30|30|
|ColorTest|60|50|30|
|CodeTest|100|80|50|
|BMPTest|2220|2190|2190|
|JPEGTest|2570|2550|2550|
|PNGTest|2860|2840|2840|
|QRTest|190|170|170|


# SPI BUS selection   
![config-spi-bus](https://user-images.githubusercontent.com/6020549/202875013-ad2ce3d4-6a2b-458b-9542-f3a17e79d5b1.jpg)

The ESP32 series has three SPI BUSs.   
SPI1_HOST is used for communication with Flash memory.   
You can use SPI2_HOST and SPI3_HOST freely.   
When you use SDSPI(SD Card via SPI), SDSPI uses SPI2_HOST BUS.   
When using this module at the same time as SDSPI or other SPI device using SPI2_HOST, it needs to be changed to SPI3_HOST.   
When you don't use SDSPI, both SPI2_HOST and SPI3_HOST will work.   
Previously it was called HSPI_HOST / VSPI_HOST, but now it is called SPI2_HOST / SPI3_HOST.   

# Using Frame Buffer   
![config-frame-buffer](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/5fe48143-fa91-408e-b62a-be3f5c16bd37)

When FrameBuffer is enabled, all output will be stored in the internal FrameBuffer and reflected to the device with ```lcdDrawFinish```.   
If you don't use FrameBuffer, ```lcdDrawFinish``` does nothing.   
If your main purpose is to display text, it's well worth using FrameBuffer.   
If your main purpose is to display images, there is no value in using FrameBuffer.   
Enabling FrameBuffer does not make image display faster.   
This is because image analysis takes time.   
___ESP32-C2 has too small memory to use this function.___   
___Note that using FrameBuffer consumes memory.___   
Memory allocate errors may occur with TFTs with large resolutions.   

ESP32-S2 has less SRAM, but some ESP32-S2 have PSRAM.   
If your SoC has PSRAM, you can avoid running out of memory by enabling PSRAM.   
![config-psram](https://github.com/user-attachments/assets/a71b320d-f5fb-4cd5-9466-f191b8704d40)

Benchmarking using ESP32@240 & 1.3 inch TFT   

||Disable Frame Buffer|Enable Frame Buffer|
|:-:|:-:|:-:|
|FillTest|1150|1150|
|ColorBarTest|50|70|
|ArrowTest|280|60|
|LineTest|2190|50|
|CircleTest|1940|60|
|RoundRectTest|1980|50|
|DirectionTest|450|60|
|HorizontalTest|1070|70|
|VerticalTest|1070|70|
|FillRectTest|190|60|
|ColorTest|260|60|
|CodeTest|1070|70|
|BMPTest|7160|2220|
|JPEGTest|2550|2570|
|PNGTest|2850|2860|
|QRTest|220|190|



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
This project uses the following as default fonts:   
- font/ILGH16XB.FNT // 8x16Dot Gothic
- font/ILGH24XB.FNT // 12x24Dot Gothic
- font/ILGH32XB.FNT // 16x32Dot Gothic
- font/ILMH16XB.FNT // 8x16Dot Mincyo
- font/ILMH24XB.FNT // 12x24Dot Mincyo
- font/ILMH32XB.FNT // 16x32Dot Mincyo

From 0x00 to 0x7f, the characters image of Alphanumeric are stored.   
From 0x80 to 0xff, the characters image of Japanese are stored.   
Changing this file will change the font.

You can add your original fonts.   
The format of the font file is the FONTX format.   
Your font file is put in font directory.   
When you build the firmware, the font files are uploaded to the SPIFFS partition.   

Please refer [this](http://elm-chan.org/docs/dosv/fontx_e.html) page about FONTX format.   

# Font File Editor(FONTX Editor)   
[There](http://elm-chan.org/fsw/fontxedit.zip) is a font file editor.   
This can be done on Windows 10.   
You can create your own font files.   
Developer page is [here](http://elm-chan.org/fsw_e.html).   

![fontx-editor-1](https://github.com/user-attachments/assets/76a8c96f-74c3-4583-a4f1-5664f0e81f3a)

# Convert from BDF font to FONTX font   
step1)   
download Font File Editor(FONTX Editor) from [here](http://elm-chan.org/fsw_e.html).   

step2)   
download BDF font file from Internet.   
I downloaded from [here](https://github.com/fcambus/spleen).   
fontxedit.exe can __ONLY__ import Monospaced bitmap fonts file.   
Monospaced bitmap fonts can also be downloaded [here](https://github.com/Tecate/bitmap-fonts).

step3)   
import the BDF font file into your fontxedit.exe.   
this tool can convert from BDF to FONTX.   
![fontx-editor-2](https://github.com/user-attachments/assets/3353bf23-01f0-455d-8c9c-b56d55b4dc9c)

step4)   
adjust font size.   
![fontx-editor-3](https://github.com/user-attachments/assets/0a99fb0b-1725-472e-8310-ca57362ae6d1)

step5)   
check font pattern.   
when you have made any changes, press the apply button.   
![fontx-editor-4](https://github.com/user-attachments/assets/44b8ed95-0c3e-4507-87fa-b94c3c2349de)

step6)   
save as .fnt file from your fontedit.exe.   
![fontx-editor-5](https://github.com/user-attachments/assets/db5b62a8-3a61-49aa-8505-b906067f1111)

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

u8g2 library contains many BDF fonts.   
This is a 24x24 font converted from emoticons21.bdf.   
![bdf-font-1](https://github.com/user-attachments/assets/6a8f8dd1-7716-44a4-b234-903275da698c)

This is a 16x16 font converted from Scroll-o-Sprites.bdf.   
![bdf-font-2](https://github.com/user-attachments/assets/506b82bb-577d-4628-9cfb-b93b115b43f1)


# Convert from TTF font to FONTX font  
step1)   
Download WFONTX64.exe from [here](https://github.com/nemuisan/WFONTX64/releases).
Developer page is [here](https://github.com/nemuisan/WFONTX64).   

step2)   
Select ttf font.   
___Please note that if you select a proportional font, some fonts may not convert correctly.___   
If you select a proportional font, some fonts will need to be modified using fontxedit.exe.   
Monospaced fonts can be converted correctly.   
You can find Monospaced fonts [here](https://en.wikipedia.org/wiki/List_of_monospaced_typefaces).   
![WFONTX64-1](https://github.com/user-attachments/assets/2193a3c4-021c-48e6-8486-2ce500bdac36)

step3)   
Enter Height, Width, FontX2 name.   
Specify half of Height for Width.   
Specify your favorite font name in the FontX2 name field using up to 8 characters.   
![WFONTX64-2](https://github.com/user-attachments/assets/c87a9ec9-8e28-4d34-8475-60b15a47fb22)


step4)   
Specify the file name to save.   
![WFONTX64-3](https://github.com/user-attachments/assets/9715d4bf-e460-41a6-9a4b-38c0f10020f7)

step5)   
Specify the font style as required.   
![WFONTX64-4](https://github.com/user-attachments/assets/0ff3072d-6a78-48ae-b855-60c692f8d771)

step6)   
Press the RUN button to convert TTF fonts to FONTX format.   
![WFONTX64-5](https://github.com/user-attachments/assets/d9797e3d-1fd6-4504-b161-c1280f1242c0)

step7)   
upload your font file to $HOME/esp-idf-st7789/fonts directory.   

step8)   
add font to use   
```
    FontxFile fx16G[2];
    FontxFile fx24G[2];
    FontxFile fx32G[2];
    //InitFontx(fx16G,"/spiffs/ILGH16XB.FNT",""); // 8x16Dot Gothic
    //InitFontx(fx24G,"/spiffs/ILGH24XB.FNT",""); // 12x24Dot Gothic
    //InitFontx(fx32G,"/spiffs/ILGH32XB.FNT",""); // 16x32Dot Gothic
    InitFontx(fx16G,"/spiffs/Gigi16.FNT",""); // 8x16Dot Gigi
    InitFontx(fx24G,"/spiffs/Gigi24.FNT",""); // 12x24Dot Gigi
    InitFontx(fx32G,"/spiffs/Gigi32.FNT",""); // 16x32Dot Gigi

```

![TTF_FONT](https://github.com/user-attachments/assets/507c7cc5-f82f-45a6-8f13-11d0fda50bc7)

# How to add your color   
Change here.   
```
#define RED    rgb565(255,   0,   0) // 0xf800
#define GREEN  rgb565(  0, 255,   0) // 0x07e0
#define BLUE   rgb565(  0,   0, 255) // 0x001f
#define BLACK  rgb565(  0,   0,   0) // 0x0000
#define WHITE  rgb565(255, 255, 255) // 0xffff
#define GRAY   rgb565(128, 128, 128) // 0x8410
#define YELLOW rgb565(255, 255,   0) // 0xFFE0
#define CYAN   rgb565(  0, 156, 209) // 0x04FA
#define PURPLE rgb565(128,   0, 128) // 0x8010
```

# How to use this component in your project   
Create idf_component.yml in the same directory as main.c.   
```
YourProject --+-- CMakeLists.txt
              +-- main --+-- main.c
                         +-- CMakeLists.txt
                         +-- idf_component.yml
```

Contents of idf_component.yml.
```
dependencies:
  nopnop2002/st7789:
    path: components/st7789/
    git: https://github.com/nopnop2002/esp-idf-st7789.git
```

When you build a projects esp-idf will automaticly fetch repository to managed_components dir and link with your code.   
```
YourProject --+-- CMakeLists.txt
              +-- main --+-- main.c
              |          +-- CMakeLists.txt
              |          +-- idf_component.yml
              +-- managed_components ----- nopnop2002__st7789
```


# External input module   

You can use these if you need some input.   
- MPR121 Capacitive Touch switch   
https://github.com/nopnop2002/esp-idf-mpr121

- TTP229 Capacitive Touch switch   
https://github.com/nopnop2002/esp-idf-ttp229



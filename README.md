# esp-idf-st7789
ST7789 Driver for esp-idf

You have to set this config value with menuconfig.   
CONFIG_WIDTH   
CONFIG_HEIGHT   
CONFIG_OFFSETX   
CONFIG_OFFSETY   
CONFIG_MOSI_GPIO   
CONFIG_SCLK_GPIO   
CONFIG_CS_GPIO   
CONFIG_DC_GPIO   
CONFIG_RESET_GPIO   
CONFIG_BL_GPIO   


```
git clone https://github.com/nopnop2002/esp-idf-st7789
cd esp-idf-st7789/
make menuconfig
make flash
```

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

---

# LILYGO TTGO 1.14 Inch

![ttgo-config-1](https://user-images.githubusercontent.com/6020549/64899699-a402e580-d6c7-11e9-9b09-e2a27c82f0fa.jpg)

![ttgo-1](https://user-images.githubusercontent.com/6020549/64899702-a6653f80-d6c7-11e9-8621-fd66393fcf42.JPG)

---

# Font File   
You can add your original font file.   
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

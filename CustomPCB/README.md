# ESP32S2 Mini PCB
I designed a PCB that can accommodate ST7789 products of various sizes.   
I use ESP32S2-Mini Development board.   
Although this product is very small, it is equipped with 2MB of PSRAM.   

![esp32s2-pcb-1](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/eadf0272-11da-467b-b2e3-fd322365053b)
![esp32s2_pcb-2](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/1de19ace-b953-4fb8-8a8b-13c31e9ef4af)

You can download the Garbers file from [here](https://www.pcbway.com/project/shareproject/esp32s2_st7789_pcb_4781f326.html).   
You can use the zip file to order it yourself from PCB manufacturing vendors such as Elecrow/FusionPCB/PCBWay.   
You can repeat my order from [here](https://www.pcbway.com/project/shareproject/esp32s2_st7789_pcb_4781f326.html).

# Configuration for 1.14inch   
![1 14inch](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/440f3af6-60a6-4d20-b2e3-23bac1abaeb1)
![config-1 14](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/15947572-ff89-4333-a93e-8448acd1b173)

# Configuration for 1.3inch   
__Don't use IO34__   
![1 3inch](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/077e8050-c96d-42b0-a6c3-128924481f45)
![config-1 3](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/01ed0f82-cabb-4354-a928-2c1a3c799ac5)

# Configuration for 1.54inch   
![1 54inch](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/4c9dc2c2-8d5c-4f5e-afe7-120686285027)
![config-1 54](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/e89af827-0310-4af2-84f1-4890e523bfa7)

# Configuration for 1.9inch   
![1 9inch](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/977afbba-731d-4857-88de-6099d99e353f)
![config-1 9](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/f71ff95b-4a50-405f-beea-6bb81cd90fdc)

# Configuration for 2.0inch   
__Don't use IO34__   
![2 0inch](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/c3d40d7e-5c19-44d7-8faa-59c74d514a91)
![config-2 0](https://github.com/nopnop2002/esp-idf-st7789/assets/6020549/1cc852b7-d69c-47f7-8a94-41d58d3cdc65)

# Why ESP32S2 Mini?
Cheap!!   
Only $1.49 excluding shipping.   
I bought two and it cost $4.27 including shipping.   

# About ESP32S2 Mini
A Type-C USB connector is mounted on the board, but a USB-TTL conversion chip is not mounted.   
The circuit diagram is available [here](https://www.wemos.cc/en/latest/_static/files/sch_s2_mini_v1.0.0.pdf).   
USB ports D- and D+ are connected to GPIO19/20.   
Since it uses the chip's USB-OTG function, it will not be recognized as a USB device even if you simply connect it to the host with a USB cable.   
To write firmware, press ButtonRST while pressing Button0, then release ButtonRST and then Button0.   
Now the device will enter DFU mode, the USB device (/dev/ttyACM0) will appear, and you can upload the firmware.   
Once the firmware upload is complete, press ButtonRST again.   
The firmware will now start.   
If you press ButtonRST without pressing Button0, the USB device will disappear.   

To use STDOUT, you need to change Channel for console output to USB CDC in menuconfig.   
![esp32-s2-mini-4](https://github.com/user-attachments/assets/d9dab093-7882-4f57-a978-38fa69c71bb6)

After changing Channel for console output to USB CDC in menuconfig, upload the firmware again.   
Once the firmware has been uploaded, the following display will appear.   
![esp32-s2-mini-5](https://github.com/user-attachments/assets/32f7e2e9-b50c-423f-ac93-9081995c7763)

__Now press ButtonRST.__   
The firmware with USB CDC enabled will now start running.   
From now on, you will be able to write and monitor the firmware without pressing Button0 or ButtonRST.

This board has 4MB of FLASH and ___2MB of PSRAM___.   
It's strange that it is recognized as having an external flash.   
There is no external flash anywhere on the board.   
![esp32-s2-mini-6](https://github.com/user-attachments/assets/f185ad9b-299b-4c23-a63a-63ed4404af3f)

# Enable PSRAM   
ESP32S2's RAM is very small.   
You can avoid running out of memory by enabling PSRAM.   
In order to use PSRAM, you need to enable PSRAM by following the steps below.   
Please note that this procedure is for ESP-IDF V5.2 and may change in the future.   
![psram-1](https://github.com/nopnop2002/esp-idf-net-logging/assets/6020549/9feae820-609d-4955-94d0-2d7e4e5887ab)
![psram-2](https://github.com/nopnop2002/esp-idf-net-logging/assets/6020549/3b6c8c0a-380f-492e-b72a-74d408c73fb0)
![psram-3](https://github.com/nopnop2002/esp-idf-net-logging/assets/6020549/a72b8638-8de1-4c9e-b8e1-98562e2b515e)
![psram-4](https://github.com/nopnop2002/esp-idf-net-logging/assets/6020549/ae980ac1-470e-4b9d-bd2b-82ec4777f204)
![psram-5](https://github.com/nopnop2002/esp-idf-net-logging/assets/6020549/76a7a4f3-e231-4509-b997-c1d710ee7301)




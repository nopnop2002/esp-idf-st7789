# Wifi Signal images

![st7789-icons-4](https://github.com/user-attachments/assets/05d4ad4f-b145-4648-abfc-01597c4388a7)

These images were created based on [this](https://www.vecteezy.com/vector-art/7149393-outlined-icon-set-of-cellular-phone-signal-indicator-suitable-for-design-element-of-smartphone-app-internet-connection-and-network-signal-symbol-information).   

The original image file is grayscale.   
One image contains 9 icons.   
We used ImageMagick to convert the images.   
Images of any size and any color can be generated using split.sh.   

![wifi_blue01](https://github.com/user-attachments/assets/ed4836e9-f76f-4965-af95-c88bf835557c)
![wifi_blue02](https://github.com/user-attachments/assets/908a20bc-2edb-46c5-bfb9-025196653bd7)
![wifi_blue03](https://github.com/user-attachments/assets/ed39f78f-e77e-4210-873c-8fe963849acd)
![wifi_blue04](https://github.com/user-attachments/assets/946c0a97-2110-4cbb-b2b4-8bcd7f48b0d9)
![wifi_blue05](https://github.com/user-attachments/assets/a0d8c229-8112-45c0-a332-4564b69bb1f5)
![wifi_blue06](https://github.com/user-attachments/assets/3557b9d2-d798-46bd-b8db-f9013f199300)
![wifi_blue07](https://github.com/user-attachments/assets/0460117c-9ff0-4790-96b5-cc641480500e)
![wifi_blue08](https://github.com/user-attachments/assets/ccbb21f3-58db-4801-b8e4-c2c1eafaf225)
![wifi_blue09](https://github.com/user-attachments/assets/dbfa586c-90bf-4ef4-99ac-0d450daec31c)

```
$ identify wifi*.png
wifi_blue01.png PNG 60x60 288x288+30+27 8-bit sRGB 28749B 0.000u 0:00.000
wifi_blue02.png PNG 60x60 288x288+115+27 8-bit sRGB 28819B 0.000u 0:00.000
wifi_blue03.png PNG 60x60 288x288+202+27 8-bit sRGB 217c 27188B 0.000u 0:00.000
wifi_blue04.png PNG 60x60 288x288+30+109 8-bit sRGB 28122B 0.010u 0:00.000
wifi_blue05.png PNG 60x60 288x288+115+109 8-bit sRGB 28720B 0.000u 0:00.000
wifi_blue06.png PNG 60x60 288x288+202+109 8-bit sRGB 29792B 0.000u 0:00.000
wifi_blue07.png PNG 60x60 288x288+30+198 8-bit sRGB 30143B 0.000u 0:00.000
wifi_blue08.png PNG 60x60 288x288+115+198 8-bit sRGB 28928B 0.000u 0:00.009
wifi_blue09.png PNG 60x60 288x288+202+198 8-bit sRGB 27726B 0.000u 0:00.000
wifi_red01.png PNG 60x60 288x288+30+27 8-bit sRGB 28761B 0.000u 0:00.000
wifi_red02.png PNG 60x60 288x288+115+27 8-bit sRGB 28732B 0.000u 0:00.000
wifi_red03.png PNG 60x60 288x288+202+27 8-bit sRGB 216c 27158B 0.000u 0:00.000
wifi_red04.png PNG 60x60 288x288+30+109 8-bit sRGB 28113B 0.000u 0:00.000
wifi_red05.png PNG 60x60 288x288+115+109 8-bit sRGB 28692B 0.000u 0:00.000
wifi_red06.png PNG 60x60 288x288+202+109 8-bit sRGB 29769B 0.000u 0:00.000
wifi_red07.png PNG 60x60 288x288+30+198 8-bit sRGB 30109B 0.000u 0:00.000
wifi_red08.png PNG 60x60 288x288+115+198 8-bit sRGB 28875B 0.000u 0:00.000
wifi_red09.png PNG 60x60 288x288+202+198 8-bit sRGB 27727B 0.000u 0:00.010
wifi_yellow01.png PNG 60x60 288x288+30+27 8-bit sRGB 28768B 0.000u 0:00.000
wifi_yellow02.png PNG 60x60 288x288+115+27 8-bit sRGB 28948B 0.000u 0:00.000
wifi_yellow03.png PNG 60x60 288x288+202+27 8-bit sRGB 242c 27293B 0.010u 0:00.000
wifi_yellow04.png PNG 60x60 288x288+30+109 8-bit sRGB 27923B 0.000u 0:00.000
wifi_yellow05.png PNG 60x60 288x288+115+109 8-bit sRGB 28551B 0.000u 0:00.000
wifi_yellow06.png PNG 60x60 288x288+202+109 8-bit sRGB 29707B 0.000u 0:00.000
wifi_yellow07.png PNG 60x60 288x288+30+198 8-bit sRGB 30066B 0.000u 0:00.000
wifi_yellow08.png PNG 60x60 288x288+115+198 8-bit sRGB 28980B 0.000u 0:00.000
wifi_yellow09.png PNG 60x60 288x288+202+198 8-bit sRGB 27694B 0.000u 0:00.000
```

## How to use these icon   
```
cd $HOME/esp-idf-st7789/icons
rm *
cd $HOME/esp-idf-st7789/wifi_signal-image3
cp wifi_blue01.png wifi_blue04.png wifi_blue08.png wifi_red02.png wifi_red05.png wifi_red08.png wifi_yellow08.png wifi_blue03.png wifi_blue06.png wifi_blue09.png wifi_red03.png wifi_red07.png wifi_red09.png $HOME/esp-idf-st7789/icons
```


```
lcdSetFontDirection(&dev, 0);
lcdFillScreen(&dev, WHITE);
strcpy(file, "/icons/wifi_blue01.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 0);
strcpy(file, "/icons/wifi_red02.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 60, 0);
strcpy(file, "/icons/wifi_yellow08.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 120, 0);

strcpy(file, "/icons/wifi_blue04.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 60);
strcpy(file, "/icons/wifi_red05.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 60, 60);

strcpy(file, "/icons/wifi_blue06.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 120);
strcpy(file, "/icons/wifi_red07.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 60, 120);

strcpy(file, "/icons/wifi_blue03.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 180);
strcpy(file, "/icons/wifi_red03.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 60, 180);
strcpy(file, "/icons/wifi_blue09.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 120, 180);
strcpy(file, "/icons/wifi_red09.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 180, 180);
```

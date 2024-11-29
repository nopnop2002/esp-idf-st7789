# Wifi Signal images

![st7789-icons-2](https://github.com/user-attachments/assets/bd1259aa-84c9-41c4-b081-de4f052a635a)

These images were created based on [this](https://www.vecteezy.com/vector-art/8610642-wifi-signal-icons-various-shapes-communication-connection-internet-wireless).   

The original image file is grayscale.   
One image contains 12 icons.   
We used ImageMagick to convert the images.   
Images of any size and any color can be generated using split.sh.   

![wifi01](https://github.com/user-attachments/assets/79ce7da2-f0e1-4475-bd7c-b3ef2d21c25b)
![wifi02](https://github.com/user-attachments/assets/fcad8587-1d4d-486a-b65f-0d5994d952d7)
![wifi03](https://github.com/user-attachments/assets/ca5780c6-3ade-4add-828c-38b40925bacb)
![wifi04](https://github.com/user-attachments/assets/8decf11c-d217-463c-a8a7-63b494ffebd9)
![wifi05](https://github.com/user-attachments/assets/48af8b12-fb63-4de6-9047-828fdca20e04)
![wifi06](https://github.com/user-attachments/assets/da41e1da-c1cb-43ab-8291-de0d1804cc2b)
![wifi07](https://github.com/user-attachments/assets/be107bf9-a265-471a-89c5-233f49334e82)
![wifi08](https://github.com/user-attachments/assets/81874ee3-cc24-48b2-be91-1a0b5bbb75b8)
![wifi09](https://github.com/user-attachments/assets/70c85e3a-ecc5-43be-bb5d-872c67a7fab9)
![wifi10](https://github.com/user-attachments/assets/341396e9-1f88-4ce3-adad-9125a5fa8ddc)
![wifi11](https://github.com/user-attachments/assets/23773d52-b634-48c5-94c3-b5e1121c7b80)
![wifi12](https://github.com/user-attachments/assets/6ab06a75-a12b-4c66-b6a4-c0fdc8ae3367)
![wifi13](https://github.com/user-attachments/assets/94aeeba4-3407-449c-9df6-e12f9274765d)
![wifi14](https://github.com/user-attachments/assets/76c518cb-6e1f-466e-bd0b-21af4725e64e)
![wifi15](https://github.com/user-attachments/assets/dca800f7-7821-404f-8261-28b8ebacdb3b)
![wifi16](https://github.com/user-attachments/assets/9f2c631f-b8d9-4dfe-adad-1e3f315eacf8)

```
$ identify wifi*.png
wifi01.png PNG 60x60 384x230+30+0 8-bit sRGB 172c 19180B 0.000u 0:00.000
wifi02.png PNG 60x60 384x230+118+0 8-bit sRGB 20012B 0.000u 0:00.000
wifi03.png PNG 60x60 384x230+206+0 8-bit sRGB 20271B 0.000u 0:00.000
wifi04.png PNG 60x60 384x230+294+0 8-bit sRGB 20369B 0.000u 0:00.000
wifi05.png PNG 60x60 384x230+34+54 8-bit sRGB 139c 18973B 0.000u 0:00.000
wifi06.png PNG 60x60 384x230+120+54 8-bit sRGB 20019B 0.000u 0:00.000
wifi07.png PNG 60x60 384x230+204+54 8-bit sRGB 20191B 0.000u 0:00.000
wifi08.png PNG 60x60 384x230+290+54 8-bit sRGB 20368B 0.000u 0:00.009
wifi09.png PNG 60x60 384x230+36+110 8-bit sRGB 168c 19195B 0.000u 0:00.000
wifi10.png PNG 60x60 384x230+120+110 8-bit sRGB 20117B 0.000u 0:00.000
wifi11.png PNG 60x60 384x230+204+110 8-bit sRGB 20280B 0.000u 0:00.000
wifi12.png PNG 60x60 384x230+288+110 8-bit sRGB 20260B 0.000u 0:00.000
wifi13.png PNG 60x60 384x230+36+164 8-bit sRGB 172c 19245B 0.000u 0:00.000
wifi14.png PNG 60x60 384x230+120+164 8-bit sRGB 20196B 0.000u 0:00.000
wifi15.png PNG 60x60 384x230+204+164 8-bit sRGB 20405B 0.000u 0:00.000
wifi16.png PNG 60x60 384x230+288+164 8-bit sRGB 20452B 0.000u 0:00.000
```

## How to use these icon   
```
cd $HOME/esp-idf-st7789/icons
rm *
cd $HOME/esp-idf-st7789/wifi_signal-image
cp *.png $HOME/esp-idf-st7789/icons
```


```
lcdSetFontDirection(&dev, 0);
lcdFillScreen(&dev, WHITE);
strcpy(file, "/icons/wifi01.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 0);
strcpy(file, "/icons/wifi02.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 60, 0);
strcpy(file, "/icons/wifi03.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 120, 0);
strcpy(file, "/icons/wifi04.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 180, 0);

strcpy(file, "/icons/wifi05.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 60);
strcpy(file, "/icons/wifi06.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 60, 60);
strcpy(file, "/icons/wifi07.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 120, 60);
strcpy(file, "/icons/wifi08.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 180, 60);

strcpy(file, "/icons/wifi09.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 120);
strcpy(file, "/icons/wifi10.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 60, 120);
strcpy(file, "/icons/wifi11.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 120, 120);
strcpy(file, "/icons/wifi12.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 180, 120);

strcpy(file, "/icons/wifi13.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 180);
strcpy(file, "/icons/wifi14.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 60, 180);
strcpy(file, "/icons/wifi15.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 120, 180);
strcpy(file, "/icons/wifi16.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 180, 180);
```

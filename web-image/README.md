# Web images

![st7789-icons-5](https://github.com/user-attachments/assets/fef240fe-3699-427a-a57c-7163075cf448)

These images were created based on [this](https://www.vecteezy.com/vector-art/20255358-colorful-business-web-modern-shadow-vector-icon-set).   

The original image file is grayscale.   
One image contains 6 icons.   
We used ImageMagick to convert the images.   
Images of any size and any color can be generated using split.sh.   

![web01](https://github.com/user-attachments/assets/a890251c-9c80-4d39-a182-72dd442db7af)
![web02](https://github.com/user-attachments/assets/61926adc-f80c-4750-9a23-8392cfa7e6e9)
![web03](https://github.com/user-attachments/assets/ea422d7d-51f7-4417-9d74-a482a82e9a41)
![web04](https://github.com/user-attachments/assets/82fd47f0-f136-4435-a53c-58b10877a7d9)
![web05](https://github.com/user-attachments/assets/8dc91df4-f8fb-4345-9d83-e49b537fa608)
![web06](https://github.com/user-attachments/assets/b860b25e-5b00-475f-b71f-65e31e75fcbd)

```
$ identify *.png
web01.png PNG 60x60 230x157+16+14 8-bit sRGB 26711B 0.000u 0:00.000
web02.png PNG 60x60 230x157+85+14 8-bit sRGB 26719B 0.000u 0:00.000
web03.png PNG 60x60 230x157+154+14 8-bit sRGB 27142B 0.000u 0:00.000
web04.png PNG 60x60 230x157+16+83 8-bit sRGB 26815B 0.000u 0:00.000
web05.png PNG 60x60 230x157+85+83 8-bit sRGB 26633B 0.000u 0:00.000
web06.png PNG 60x60 230x157+154+83 8-bit sRGB 27956B 0.000u 0:00.000```
```

## How to use these icon   
```
cd $HOME/esp-idf-st7789/icons
rm *
cd $HOME/esp-idf-st7789/web-image
cp *.png $HOME/esp-idf-st7789/icons
```


```
lcdFillScreen(&dev, WHITE);
strcpy(file, "/icons/web01.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 0);
strcpy(file, "/icons/web02.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 60, 0);
strcpy(file, "/icons/web03.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 120, 0);

strcpy(file, "/icons/web04.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 60);
strcpy(file, "/icons/web05.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 60, 60);
strcpy(file, "/icons/web06.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 120, 60);
```

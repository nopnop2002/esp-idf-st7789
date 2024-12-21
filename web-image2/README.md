# Web images

![st7789-icons-6](https://github.com/user-attachments/assets/8441ba3f-472e-4a46-a7d6-edc687bbab01)

These images were created based on [this](https://www.vecteezy.com/vector-art/3706367-set-of-icons-for-business-card-and-information-id-card-business-card-icon-element-in-outline-glyph-and-filled-outline-styles).   

The original image file is grayscale.   
One image contains 21 icons.   
We used ImageMagick to convert the images.   
Images of any size and any color can be generated using split.sh.   

![web01](https://github.com/user-attachments/assets/e7101ecd-b478-4dc8-9dec-bfb4c837027a)
![web02](https://github.com/user-attachments/assets/b72adb85-580c-4d65-87f5-8dd6dc4633b4)
![web03](https://github.com/user-attachments/assets/cfcd12fa-681b-4805-b009-1de46bc561f5)
![web04](https://github.com/user-attachments/assets/9265347e-2825-4908-bfe5-a3653dc36824)
![web05](https://github.com/user-attachments/assets/8bd7ecb0-616a-46d2-b35b-c1cebcfa69d1)
![web06](https://github.com/user-attachments/assets/62e7397e-9572-47b6-9b70-652de9a7ad8e)
![web07](https://github.com/user-attachments/assets/8633cdc9-ab31-49a0-8769-4169f9db4247)

```
$ identify *.png
web01.png PNG 60x60 411x247+7+163 8-bit sRGB 34955B 0.000u 0:00.009
web02.png PNG 60x60 411x247+66+163 8-bit sRGB 35151B 0.000u 0:00.000
web03.png PNG 60x60 411x247+120+163 8-bit sRGB 35995B 0.000u 0:00.000
web04.png PNG 60x60 411x247+168+163 8-bit sRGB 34827B 0.000u 0:00.000
web05.png PNG 60x60 411x247+219+163 8-bit sRGB 35284B 0.000u 0:00.000
web06.png PNG 60x60 411x247+274+163 8-bit sRGB 37567B 0.000u 0:00.000
web07.png PNG 60x60 411x247+340+163 8-bit sRGB 35524B 0.000u 0:00.000
```

## How to use these icon   
```
cd $HOME/esp-idf-st7789/icons
rm *
cd $HOME/esp-idf-st7789/web-image2
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

strcpy(file, "/icons/web07.png");
IconTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT, 0, 120);
```

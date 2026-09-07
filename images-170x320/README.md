# 170x320 images
I obtained these image files from [here](https://github.com/mboehmerm/Three-IPS-Displays-with-ST7789-170x320-240x280-240x320/tree/main/Arduino/show_pictures_170x320).   

<img width="864" height="576" alt="Image" src="https://github.com/user-attachments/assets/f9ada054-04bd-4641-b334-f9ae16149c2e" />
<img width="864" height="576" alt="Image" src="https://github.com/user-attachments/assets/eef4684b-998a-4c84-a1a8-3fc7e0c1958a" />
<img width="864" height="576" alt="Image" src="https://github.com/user-attachments/assets/f6f09adf-8a8e-4af8-ba83-67a484ee89d4" />
<img width="864" height="576" alt="Image" src="https://github.com/user-attachments/assets/7fe79aca-091e-4341-bf92-9fb0f84eaec4" />
<img width="864" height="576" alt="Image" src="https://github.com/user-attachments/assets/5fad5793-b6b9-42d6-9458-81b3ac8f96c3" />

```
$ identify *.png
Balloon170x320.png PNG 170x320 170x320+0+0 8-bit sRGB 65081B 0.000u 0:00.000
Boat170x320.png PNG 170x320 170x320+0+0 8-bit sRGB 77288B 0.000u 0:00.000
Field170x320.png PNG 170x320 170x320+0+0 8-bit sRGB 103339B 0.000u 0:00.000
Flower170x320.png PNG 170x320 170x320+0+0 8-bit sRGB 87002B 0.000u 0:00.000
adabot170x320.png PNG 170x320 170x320+0+0 8-bit sRGB 92247B 0.000u 0:00.000
```

## How to use these images   
```
strcpy(file, "/images/Balloon170x320.png");
PNGTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
WAIT;
strcpy(file, "/images/Boat170x320.png");
PNGTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
WAIT;
strcpy(file, "/images/Flower170x320.png");
PNGTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
WAIT;
strcpy(file, "/images/Field170x320.png");
PNGTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
WAIT;
strcpy(file, "/images/adabot170x320.png");
PNGTest(&dev, file, CONFIG_WIDTH, CONFIG_HEIGHT);
WAIT;
```

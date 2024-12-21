#!/bin/bash

SOURCE=vecteezy_colorful-business-web-modern-shadow-vector-icon-set_20255358.jpg
convert ${SOURCE} -crop 500x500+135+115 web01.png
convert ${SOURCE} -crop 500x500+710+115 web02.png
convert ${SOURCE} -crop 500x500+1285+115 web03.png

convert ${SOURCE} -crop 500x500+135+690 web04.png
convert ${SOURCE} -crop 500x500+710+690 web05.png
convert ${SOURCE} -crop 500x500+1285+690 web06.png

# resize images
#mogrify -resize 100x *.png
#mogrify -resize 80x *.png
mogrify -resize 60x *.png
#mogrify -resize 40x *.png


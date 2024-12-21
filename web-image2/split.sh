#!/bin/bash

SOURCE=vecteezy_set-of-icons-for-business-card-and-information-id-card_3706367.jpg
convert ${SOURCE} -crop 280x280+35+760 web01.png
convert ${SOURCE} -crop 280x280+310+760 web02.png
convert ${SOURCE} -crop 280x280+561+760 web03.png
convert ${SOURCE} -crop 280x280+784+760 web04.png
convert ${SOURCE} -crop 280x280+1020+760 web05.png
convert ${SOURCE} -crop 280x280+1280+760 web06.png
convert ${SOURCE} -crop 280x280+1585+760 web07.png

# resize images
#mogrify -resize 100x *.png
#mogrify -resize 80x *.png
mogrify -resize 60x *.png
#mogrify -resize 40x *.png


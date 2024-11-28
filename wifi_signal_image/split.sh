#!/bin/bash

# change color from black to red
convert -fuzz 80% -fill '#ff0000' -opaque '#000000' vecteezy_wifi-signal-icons-various-shapes-communication-connection_8610642.jpg red.jpg

# change color from black to blue
convert -fuzz 80% -fill '#0000ff' -opaque '#000000' vecteezy_wifi-signal-icons-various-shapes-communication-connection_8610642.jpg blue.jpg

# crop images
COLOR=red
convert ${COLOR}.jpg -crop 300x300+150+0 wifi01.png
convert ${COLOR}.jpg -crop 300x300+590+0 wifi02.png
convert ${COLOR}.jpg -crop 300x300+1030+0 wifi03.png
convert ${COLOR}.jpg -crop 300x300+1470+0 wifi04.png

convert ${COLOR}.jpg -crop 300x300+170+270 wifi05.png
convert ${COLOR}.jpg -crop 300x300+600+270 wifi06.png
convert ${COLOR}.jpg -crop 300x300+1020+270 wifi07.png
convert ${COLOR}.jpg -crop 300x300+1450+270 wifi08.png

convert ${COLOR}.jpg -crop 300x300+180+550 wifi09.png
convert ${COLOR}.jpg -crop 300x300+600+550 wifi10.png
convert ${COLOR}.jpg -crop 300x300+1020+550 wifi11.png
convert ${COLOR}.jpg -crop 300x300+1440+550 wifi12.png

convert ${COLOR}.jpg -crop 300x300+180+820 wifi13.png
convert ${COLOR}.jpg -crop 300x300+600+820 wifi14.png
convert ${COLOR}.jpg -crop 300x300+1020+820 wifi15.png
convert ${COLOR}.jpg -crop 300x300+1440+820 wifi16.png

# resize images
#mogrify -resize 100x *.png
#mogrify -resize 80x *.png
mogrify -resize 60x *.png


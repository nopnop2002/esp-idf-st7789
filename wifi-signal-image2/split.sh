#!/bin/bash

# change color from black to red
convert -fuzz 80% -fill '#ff0000' -opaque '#000000' vecteezy_cellular-signal-icon-set-suitable-for-design-element_6306050.jpg red.jpg

# change color from black to blue
convert -fuzz 80% -fill '#0000ff' -opaque '#000000' vecteezy_cellular-signal-icon-set-suitable-for-design-element_6306050.jpg blue.jpg

# crop images
COLOR=blue
convert ${COLOR}.jpg -crop 300x300+40+60 wifi01.png
convert ${COLOR}.jpg -crop 300x300+300+60 wifi02.png
convert ${COLOR}.jpg -crop 300x300+560+60 wifi03.png
convert ${COLOR}.jpg -crop 300x300+820+60 wifi04.png
convert ${COLOR}.jpg -crop 300x300+1080+60 wifi05.png
convert ${COLOR}.jpg -crop 300x300+1340+60 wifi06.png
convert ${COLOR}.jpg -crop 300x300+1600+60 wifi07.png

convert ${COLOR}.jpg -crop 300x300+40+400 wifi11.png
convert ${COLOR}.jpg -crop 300x300+300+400 wifi12.png
convert ${COLOR}.jpg -crop 300x300+560+400 wifi13.png
convert ${COLOR}.jpg -crop 300x300+820+400 wifi14.png
convert ${COLOR}.jpg -crop 300x300+1080+400 wifi15.png
convert ${COLOR}.jpg -crop 300x300+1340+400 wifi16.png
convert ${COLOR}.jpg -crop 300x300+1600+400 wifi17.png

convert ${COLOR}.jpg -crop 300x300+40+740 wifi21.png
convert ${COLOR}.jpg -crop 300x300+300+740 wifi22.png
convert ${COLOR}.jpg -crop 300x300+560+740 wifi23.png
convert ${COLOR}.jpg -crop 300x300+820+740 wifi24.png
convert ${COLOR}.jpg -crop 300x300+1080+740 wifi25.png
convert ${COLOR}.jpg -crop 300x300+1340+740 wifi26.png
convert ${COLOR}.jpg -crop 300x300+1600+740 wifi27.png

# resize images
#mogrify -resize 100x *.png
#mogrify -resize 80x *.png
#mogrify -resize 60x *.png
mogrify -resize 40x *.png


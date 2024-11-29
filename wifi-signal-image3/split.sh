#!/bin/bash

# change color from black to red
convert -fuzz 80% -fill '#ff0000' -opaque '#000000' vecteezy_outlined-icon-set-of-cellular-phone-signal-indicator_7149393.jpg red.jpg

# change color from black to blue
convert -fuzz 80% -fill '#0000ff' -opaque '#000000' vecteezy_outlined-icon-set-of-cellular-phone-signal-indicator_7149393.jpg blue.jpg

# change color from black to yellow
convert -fuzz 80% -fill '#ffdc00' -opaque '#000000' vecteezy_outlined-icon-set-of-cellular-phone-signal-indicator_7149393.jpg yellow.jpg

function crop_function() {
	convert ${COLOR}.jpg -crop 400x400+200+180 wifi_${COLOR}01.png
	convert ${COLOR}.jpg -crop 400x400+770+180 wifi_${COLOR}02.png
	convert ${COLOR}.jpg -crop 400x400+1350+180 wifi_${COLOR}03.png
	convert ${COLOR}.jpg -crop 400x400+200+730 wifi_${COLOR}04.png
	convert ${COLOR}.jpg -crop 400x400+770+730 wifi_${COLOR}05.png
	convert ${COLOR}.jpg -crop 400x400+1350+730 wifi_${COLOR}06.png
	convert ${COLOR}.jpg -crop 400x400+200+1320 wifi_${COLOR}07.png
	convert ${COLOR}.jpg -crop 400x400+770+1320 wifi_${COLOR}08.png
	convert ${COLOR}.jpg -crop 400x400+1350+1320 wifi_${COLOR}09.png
}

# crop images
COLOR=blue
crop_function

COLOR=red
crop_function

COLOR=yellow
crop_function

# resize images
#mogrify -resize 100x *.png
#mogrify -resize 80x *.png
mogrify -resize 60x *.png
#mogrify -resize 40x *.png


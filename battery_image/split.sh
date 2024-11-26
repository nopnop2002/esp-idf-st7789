#!/bin/bash

# crop images
convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+100+100 battery01.png
convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+540+100 battery02.png
convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+970+100 battery03.png
convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+1420+100 battery04.png

convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+100+450 battery05.png
convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+540+450 battery06.png
convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+970+450 battery07.png
convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+1420+450 battery08.png

convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+100+800 battery09.png
convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+540+800 battery10.png
convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+970+800 battery11.png
convert vecteezy_vector-illustration-of-battery-level-icon-set-suitable-for_.jpg -crop 400x200+1420+800 battery12.png

# resize images
#mogrify -resize 100x *.png
mogrify -resize 80x *.png
#mogrify -resize 60x *.png

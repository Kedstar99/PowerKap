#include !/bin/bash

profKap -c -e -- /usr/bin/mplayer -benchmark -nosound -lavdopts threads=2 bbb_sunflower_1080p_60fps_stereo_abl.mp4
mkdir cpu2
mv *.json cpu2
profKap -c -e -- /usr/bin/mplayer -benchmark -nosound -lavdopts threads=3 bbb_sunflower_1080p_60fps_stereo_abl.mp4
mkdir cpu3
mv *.json cpu3
profKap -c -e -- /usr/bin/mplayer -benchmark -nosound -lavdopts threads=4 bbb_sunflower_1080p_60fps_stereo_abl.mp4
mkdir cpu4
mv *.json cpu4

#!/bin/bash
export FORCE_TIMES_TO_RUN=1

mkdir run1
# cd run1

#profKap -e -c -- /usr/sbin/john -test
#mkdir johnDir
#mv *.json ./johnDir
#mv johnDir ./run1

#profKap -e -i 100 -c -- /usr/bin/openssl speed -engine padlock -evp aes-256-cbc
#mkdir opensslDir
#mv *.json ./opensslDir
#mv opensslDir ./run1

#profKap -e -i 100 -c -- /usr/bin/java -jar $HOME/dacapo-9.12-bach.jar sunflow
#mkdir sunflowDir
#mv *.json ./sunflowDir
#mv sunflowDir ./run1

profKap -e -c -- $HOME/stream
mkdir streamDir
mv *.json ./streamDir
mv streamDir ./run1 

#profKap -c -e -- /usr/bin/mplayer -benchmark -nosound -lavdopts threads=2 $HOME/bbb_sunflower_1080p_60fps_stereo_abl.mp4
#mkdir mplayerDir
#mv *.json mplayerDir
#mv mplayerDir ./run1


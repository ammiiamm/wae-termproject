#!/bin/bash
#make clean
make
#
# == RTSP stream ==
#
./AppTest -c1 ipcam -n1 1 -w1 1920 -h1 1080 -p1 "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov" -process  -fps 60 -skipfrozenframes -outputfolder "./BuckBunny/" -enablecircbuf -numframesincircbuf 10 -debugmode 0 -writetimestamponframe -savevideo -usewholeimageasroi -resizeimage 480 -streamresizedimg #-dispimgcapture 

# -setthreadpriority 50  "Need sudo access"
#-debugmode 0| 1 | 2 for NO_DEBUG | INFO | VERBOSE 
#-dispimgcapture 


/*
 * VideoStreamerVlc
 * Step By Step Guide For Using VideoStreamerVlc.
 * 1. #include "VideoStreamerVlc.h"  ** Include Header File **
 * 2. VideoStreamerVlc Streamer; ** Instance Variable **
 *    iWidth = Width of the frame you are trying to stream.
 *    iHeight = Height of the frame you are trying to stream.
 *    iChannels = Number Of Color Channels Of the Frame eg = 3 (For RGB)
 *    iFrameRate = Frame Rate You want to stream.
 *    sPort = The Port that is used for Streaming.
 *    sStreamName = Stream Name.
 *  
 * 3. Streamer.setFrameInformation(int iWidth, int iHeight, int iChannels, float iFrameRate, string sPort, string sStreamName);
 * 4. Streamer.InitializeStreamer(); Start Video Streaming Server.
 * 5. Streamer.addFrameForStream(frame);  Add Frames to stream.
 * 
 *    View Your Stream @   ******      http://localhost:sPort/sStreamName.mjpg    OR    rtsp://localhost:sPort/sStreamName   *******  
 *    :D Good Luck :)
 */

#ifndef VIDEOSTREAMERVLC_H_
#define VIDEOSTREAMERVLC_H_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>
#include <vlc/vlc.h>
#include <vlc/libvlc_vlm.h>
#include <vlc/libvlc.h>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <boost/thread/thread.hpp>
#include <unistd.h>

#define USE_MJPEG       1
#define USE_H264        2
#define USE_WEB         3

using namespace cv;
using namespace std;

struct ImemData
{
    std::vector<cv::Mat> mImages;
    std::size_t mFrame;
    int64_t mPts;
    int iFrameTime;
    boost::mutex Referance;
};

class VideoStreamerVlc
{
public:
    // Constructor, destructor
    VideoStreamerVlc();
    virtual ~VideoStreamerVlc();
    void setFrameInformation(int iWidth, int iHeight, int iChannels,
            float iFrameRate, string sPort, string sStreamName);
    void InitializeStreamer(bool bDebug = false, string sDebugLevel = "0");
    void addFrameForStream(Mat mImg);
    void sampleClip(Mat &mImg);
    void setCompressionMode(int iMode);
private:
    vector<Mat> generateBuffer();
    bool bSetFrameInformation;
    void Streamer();
    libvlc_instance_t *_plVlc;
    libvlc_media_player_t *_pMediaPlayer;
    libvlc_media_t *_pMedia;
    string sPort;
    string sStreamName;
    string sTranscodeParams;
    bool bDebug;
    string sDebugLevel;
    bool bFirstFrame;
    int x, y, i; //For Sample Clip
    int iStreamImageHeight;
    ImemData imData;
    int iStreamImageWidth;
    int iStreamImageChannels;
    float iStreamFrameTime;
    int iCurrentStreamedFrame;
    bool bH264Enabled;
    bool bMJPEGEnabled;
    bool bWEBEnabled;
};
#endif /* VIDEOSTREAMERVLC_H_ */

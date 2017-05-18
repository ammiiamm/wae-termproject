#include "VideoStreamerVlc.h"
boost::mutex _mCurrentStreamedFrame;

void VideoStreamerVlc::sampleClip(Mat& mImg)
{
    i++;
    if (i > 999) i = 0;
    mImg = Mat::zeros(iStreamImageHeight, iStreamImageWidth, CV_8UC3);
    char sTime[80];
    time_t timeCurrent = time(0);
    struct tm *pTime = localtime(&timeCurrent);
    strftime(sTime, 80, "%d %m %Y %T", pTime);
    putText(mImg, sTime, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8,
            cvScalar(200, 200, 250), 1, CV_AA);

    // prepare a dummy image

    for (y = 0; y < iStreamImageHeight; y++)
    {
        for (x = 0; x < iStreamImageWidth; x++)
        {
            if (y > 50)
            {
                mImg.at<Vec3b>(y, x)[0] = x + y + i * 3 + 0;
                mImg.at<Vec3b>(y, x)[1] = x + y + i * 4 + 100;
                mImg.at<Vec3b>(y, x)[2] = x + y + i * 5 + 200;
            }
        }
    }
}

void VideoStreamerVlc::setCompressionMode(int iMode)
{
    if (iMode == 1)
    {
        bH264Enabled = true;
    }
    if (iMode == 2)
    {
        bMJPEGEnabled = true;
    }
    if (iMode == 3)
    {
        bWEBEnabled = true;
    }
}

int ImemGetCallback(void* data, const char* cookie, int64_t* dts, int64_t* pts,
        unsigned int* flags, size_t* bufferSize, void** buffer)
{
    ImemData* imem = (ImemData*)data;
    if (imem == NULL) return 1;
    if (imem->mFrame >= imem->mImages.size())
    {
        imem->mFrame = 0;
    }
    int64_t uS = imem->iFrameTime; // 30 fps
    *dts = *pts = imem->mPts = imem->mPts + uS;
    cv::Mat img;
    {
        _mCurrentStreamedFrame.lock();
        img = imem->mImages[imem->mFrame++];
        _mCurrentStreamedFrame.unlock();
    }
    *bufferSize = img.rows * img.cols * img.channels();
    *buffer = malloc(*bufferSize);
    memcpy(*buffer, img.data, *bufferSize);
    return 0;
}

int ImemReleaseCallback(void *data, const char *cookie, size_t bufferSize,
        void * buffer)
{
    free(buffer);
    return 0;
}

VideoStreamerVlc::VideoStreamerVlc()
{
    bSetFrameInformation = false;
    bFirstFrame = false;
    iStreamImageHeight = 0;
    iStreamImageWidth = 0;
    iStreamImageChannels = 0;
    iStreamFrameTime = 0;
    iCurrentStreamedFrame = 0;
    bH264Enabled = false;
    bMJPEGEnabled = false;
    bWEBEnabled = false;
}

VideoStreamerVlc::~VideoStreamerVlc()
{
    libvlc_media_player_stop(_pMediaPlayer);
    libvlc_media_player_release(_pMediaPlayer);
    libvlc_release(_plVlc);
}

void VideoStreamerVlc::setFrameInformation(int iWidth, int iHeight,
        int iChannels, float iFrameRate, string sPort, string sStreamName)
{
    bool bFrameInfoCorrect = true;
    if (iChannels != 3 || iWidth == 0 || iHeight == 0 || iFrameRate == 0)
    {
        bFrameInfoCorrect = false;
    }
    if (bFrameInfoCorrect)
    {
        bSetFrameInformation = true;
    }
    else
    {
        cout << "Please Check Width,Height,Channels and frame rate" << endl;
        quick_exit(EXIT_FAILURE);
    }
    iStreamImageHeight = iHeight;
    iStreamImageWidth = iWidth;
    iStreamImageChannels = iChannels;
    iStreamFrameTime = (1 / iFrameRate) * 1000000;
    this->sPort = sPort;
    this->sStreamName = sStreamName;
}

vector<Mat> VideoStreamerVlc::generateBuffer()
{
    Mat mEmptyImage = Mat::zeros(iStreamImageHeight, iStreamImageWidth,
    CV_8UC3);
    int iBufferSize = 1;
    vector<Mat> vmBuffer;
    for (int iBuff = 0; iBufferSize > iBuff; iBuff++)
    {
        vmBuffer.push_back(mEmptyImage);
    }
    return vmBuffer;
}
void VideoStreamerVlc::Streamer()
{
    if (!bSetFrameInformation)
    {
        cout << "Please use setFrameInformation()" << endl;
        quick_exit(EXIT_FAILURE);
    }
    imData.mImages = this->generateBuffer();
    imData.iFrameTime = iStreamFrameTime;
    std::vector<const char*> options;
    std::vector<const char*>::iterator option;

    char imemDataArg[256];
    sprintf(imemDataArg, "--imem-data=%p", &imData);
    options.push_back(imemDataArg);

    char imemGetArg[256];
    sprintf(imemGetArg, "--imem-get=%p", ImemGetCallback);
    options.push_back(imemGetArg);

    char imemReleaseArg[256];
    sprintf(imemReleaseArg, "--imem-release=%p", ImemReleaseCallback);
    options.push_back(imemReleaseArg);

    options.push_back("--imem-cookie=\"IMEM\"");

    options.push_back("--imem-fps=30");
    
    options.push_back("--imem-codec=RV24");


    options.push_back("--imem-cat=2");

    char imemWidthArg[256];
    sprintf(imemWidthArg, "--imem-width=%d", iStreamImageWidth);
    options.push_back(imemWidthArg);

    char imemHeightArg[256];
    sprintf(imemHeightArg, "--imem-height=%d", iStreamImageHeight);
    options.push_back(imemHeightArg);

    char imemChannelsArg[256];
    sprintf(imemChannelsArg, "--imem-channels=%d", iStreamImageChannels);
    options.push_back(imemChannelsArg);
    if (bDebug)
    {
        string sDebugLevelSet = "--verbose=" + sDebugLevel;
        options.push_back(sDebugLevelSet.c_str());
    }

    _plVlc = libvlc_new(int(options.size()), options.data());

    _pMedia = libvlc_media_new_location(_plVlc, "imem://");

    options.clear();
    if (bH264Enabled)
    {
        sTranscodeParams =
                ":sout=#transcode{vcodec=h264,acodec=mpga,ab=128,channels=2,samplerate=44100}:rtp{sdp=rtsp://:"
                        + sPort + "/" + sStreamName + "}";
        cout << "Streaming Link rtsp://localhost:" + sPort + "/" + sStreamName
                << endl;
    }
    else if(bMJPEGEnabled)
    {
        sTranscodeParams =
                ":sout=#transcode{vcodec=MJPG,vb=800,scale=1,acodec=none}:duplicate{dst=std{access=http,mux=mpjpeg,noaudio,dst=:"
                        + sPort + "/" + sStreamName + ".mjpg}";
        cout
                << "Streaming Link http://localhost:" + sPort + "/"
                        + sStreamName + ".mjpg" << endl;
    }
    else if(bWEBEnabled)
    {
        sTranscodeParams =
                ":sout=#transcode{vcodec=theo,vb=4000,acodec=vorb,ab=128,channels=2,samplerate=44100}:http{mux=ogg,dst=:"
                        + sPort + "/" + sStreamName + ".ogg}";
        cout
                << "Streaming Link http://localhost:" + sPort + "/"
                        + sStreamName + ".ogg" << endl;
    }
    options.push_back(sTranscodeParams.c_str());
    //options.push_back(":sout=#transcode{vcodec=theo,vb=4000,acodec=vorb,ab=128,channels=2,samplerate=44100}:http{mux=ogg,dst=:8080/test.ogg}");
    for (option = options.begin(); option != options.end(); option++)
    {
        libvlc_media_add_option(_pMedia, *option);
    }

    _pMediaPlayer = libvlc_media_player_new_from_media(_pMedia);
    libvlc_media_release(_pMedia);
    libvlc_media_player_play(_pMediaPlayer);
}

void VideoStreamerVlc::InitializeStreamer(bool bDebug, string sDebugLevel)
{
    this->bDebug = bDebug;
    this->sDebugLevel = sDebugLevel;
    boost::thread(boost::bind(&VideoStreamerVlc::Streamer, this));
}

void VideoStreamerVlc::addFrameForStream(Mat mImg)
{

    int iStreamedFrame;
    {
        _mCurrentStreamedFrame.lock();
        iStreamedFrame = iCurrentStreamedFrame;
        imData.mImages[0] = mImg;
        _mCurrentStreamedFrame.unlock();
    }

    boost::this_thread::sleep(boost::posix_time::milliseconds(1));
}


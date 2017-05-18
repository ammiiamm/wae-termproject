/*
 * App.h
 * Ramesh Marikhu <marikhu@gmail.com>
 * An application that has FrameGrabber and Database integrated.
 * 4 May 2016
 */

#ifndef APP_H_
#define APP_H_

#include <cv.h>
#include <ml.h>
#include <opencv2/video/background_segm.hpp>

#include <boost/thread.hpp>
#include <boost/circular_buffer.hpp>

#include <vector>
#include <iostream>
using namespace std;

#include "CameraEmulator.h"
#include "Camera.h"
#include "CameraUSB.h"
#include "CameraIQEye.h"
#include "CameraAccessor.h"
#include "CameraCaptureConfig.h"
#include "AppConfig.h"
#include "Clock.h"
#include "ErrorHandler.h"
#include "FrameGrabber.h"
#include "FrameGrabberConfig.h"
#include "Logger.h"
#include "MotionDetector.h"
#include "Regions.h"
#include "Timer.h"
#include "Utilities.h"

extern bool volatile bTerminate;

class App
{
public:
    App();
    ~App();

    static void initErrorDiagnosticsList();
    static string getOutputFolder();

    void setFrameGrabberObj(FrameGrabber *pFrameGrabberObj);
    void startApp(bool bDebug = false);
    bool setFrameForInit(IplImage *pImageGrabbed);
    void displayCapturedImageForUserControl(IplImage *pImageGrabbed,
            bool &bSkipFrame, bool bDebug = false);
    void processFrame(const IplImage *pImageC, bool bDebug = false);
    void prepareResultsImage();

    void createVideoFile(int iVideoFileNo, IplImage *pImage);
    void saveFrameToVideo(IplImage *pImage, int iFrameNo);
    void writeToVideoTimestampFile(timeval curTime, int iFrameNo, bool bDebug =
            false);

    void specifyProcessingRegions(bool bDebug = false);
    void initializeSystem(bool bDebug = false);

    // Getters and Setters
    static int getObjCount();
    int getFrameNo();

    // Program parameters
    static void setDebugMode(teDebugMode debugMode);
    static void setNumFramesToIgnore(int iNumFramesToIgnore,
            bool bDebug = false);
    static int getNumFramesToIgnore();
    static void setCamera1(string sCamera1);
    static void setVideo1Path(string sVideo1Path);
    static void setCamera1No(int iCamera1No);
    static void setCaptureMode1(int iCaptureMode1);
    static void setImageType1(int iImageType1);
    static void setFPS(float fFPS);
    static void setNumFramesToGrab(int iNumFramesToGrab);
    static void setCamera1Resolution(int iWidth, int iHeight);
    static void setResizeImage(bool bResizeImage, int iResizedImgHeight,
            bool bStreamResizedImg);
    static void setConfigFile(char *pchConfigFile);
    static void setRegionsFromConfigFile(bool bSetRegionsFromConfigFile);

    // Timestamp
    static void setTimestampFile(char *pchTimestampFile);

    // Display image windows
    static void setDispImgCapture(bool bDispImgCapture);
    static void setDispImgResults(bool bDispImgResults);
    static void setResizeImage(bool bResizeImage, int iImgHeight);
    static void setUseWholeImageAsROI(bool bUseWholeImageAsROI);
    static void setProcessFrames(bool bProcessFrames);

    void setTerminateProgram(bool bTerminateProgram);
    bool getTerminateProgram();

private:
    static int _iObjCount;
    static ErrorHandler *_pErrorHandler;
    static Logger *_pLogger;
    static bool _bIsSetErrorDiagnosticsList;

    Camera *_pCamera;
    Clock *_pClock;
    MotionDetector *_pMotionDetector;
    Regions *_pProcessingRegions;
    Timer *_pTimer;
    Utilities *_pUtilities;

    IplImage *_pImage3Channel;
    IplImage *_pImageResized;
    IplImage *_pImageResults;
    int _iImageROIWidth;
    int _iImageROIHeight;

    IplImage *_pImageProcess;
    IplImage *_pImageProcessGray;

    CvRect _rectProcessingMaskROI;
    IplImage * _pImageProcessingMaskROI;
    IplImage *_pImageProcessingMaskROI_Neg;
    IplImage *_pImageProcessingMaskROI_cropped;
    int _iOffset_TL_X;
    int _iOffset_TL_Y;
    CvRect _rectViolationMaskROI;
    IplImage *_pImageViolationMaskROI;

    bool _bIsSystemInitialized;
    int _iFrameNo;
    int _iFrameIntvlCount;
    bool _bIsSetProcessingStartTime;

    stringstream _ssOut;
    string _sText;

    CvFont _font1;
    CvFont _font2;
    CvScalar _textColor;

    CvMemStorage *_pMemStorage;
    CvSeq *_pSeqBlobs;

    // Program parameters
    static teDebugMode _debugMode;
    static int _iNumFramesToIgnore;
    static string _sCamera1;
    static string _sVideo1Path;
    static int _iCamera1No;
    static int _iCaptureMode1;
    static int _iImageType1;
    static float _fFPS;
    static int _iNumFramesToGrab;
    static int _iNumFramesToSkip;
    static int _iCamera1Width;
    static int _iCamera1Height;
    static bool _bIsSetCamRes;
    static bool _bIsSetConfigFile;
    static string _sConfigFile;
    static bool _bIsSetRegionsFromConfigFile;
    static FileStorage _fsConfigFileRead;

    // Timestamp file, used for emulator to simulate live video timings
    static bool _bIsSetTimestampFile;
    static string _sTimestampFile;
    static FileStorage _fsTimestampFileRead;

    // Displaying images
    static bool _bDispImgCapture;
    static bool _bDispImgResults;

    static bool _bResizeImage;
    static int _iResizedImgHeight;
    static int _iResizedImgWidth;
    static bool _bStreamResizedImg;
    static float _fDimRatioWithOrigImg;

    static bool _bUseWholeImageAsROI;
    static bool _bProcessFrames;

    // Database variables
    int _iCurProgramRunID;

    // Saving to video file
    CvVideoWriter *_pVideoWriter;
    int _iVideoFileNo;
    bool _bSaveFrameToVideo;
    bool _bCreateNewVideoFile;
    FileStorage _fsVideoTimestampYML;
    timeval _tvTimeOfFrameCapture;
    int _iFrameNoInVideo;

    // Processing framerate
    timeval _tvPrev;
    double _dProcessingFPS;

    // Time of capture
    timeval _tvFrameCapture;
    timeval _tvFrameCapturePrev;
    float _fTimeDiffMs;
    bool _bIsSetTimeDiffMs;
    bool _bIsSetFrameCapturePrev;
    vector<timeval> _vtvTimeBetweenFrames;
    bool _bSlowDown;

    FrameGrabber *_pFrameGrabber;
    int _iLocationID;

    // Terminate program
    bool _bTerminateProgram;
};
#endif //#ifndef APP_H_

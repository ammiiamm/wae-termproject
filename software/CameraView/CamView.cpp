/*
 * App.cpp
 * Ramesh Marikhu <marikhu@gmail.com>
 */

#include <cv.h>
#include <highgui.h>
using namespace cv;
#include "App.h"

int App::_iObjCount = 0;
ErrorHandler* App::_pErrorHandler = NULL;
Logger* App::_pLogger = NULL;
bool App::_bIsSetErrorDiagnosticsList = false;

// Program parameters
teDebugMode App::_debugMode = NO_DEBUG;
int App::_iNumFramesToIgnore = 0;
string App::_sCamera1 = string("");
string App::_sVideo1Path = string("");
int App::_iCamera1No = 0;
int App::_iCaptureMode1 = 0;
int App::_iImageType1 = 0;
float App::_fFPS = 0;
int App::_iNumFramesToGrab = 0;
int App::_iCamera1Width = -1;
int App::_iCamera1Height = -1;
bool App::_bIsSetCamRes = false;
bool App::_bIsSetConfigFile = false;
string App::_sConfigFile = "";
bool App::_bIsSetRegionsFromConfigFile = false;
FileStorage App::_fsConfigFileRead;
bool App::_bIsSetTimestampFile = false;
string App::_sTimestampFile = "";
FileStorage App::_fsTimestampFileRead;

// Display image windows
bool App::_bDispImgCapture = false;
bool App::_bDispImgResults = false;

// Resize image for processing
bool App::_bResizeImage = false;
int App::_iResizedImgHeight = -1;
int App::_iResizedImgWidth = -1;
bool App::_bStreamResizedImg = false;
float App::_fDimRatioWithOrigImg = 1.0f;

bool App::_bUseWholeImageAsROI = false;

// Process frames
bool App::_bProcessFrames = false;

void App::initErrorDiagnosticsList()
{
    if (!App::_bIsSetErrorDiagnosticsList)
    {
        //NOTE: Enter the ID XXXXX for your class in ErrorHandler.h so that there is no conflict.
        // You will be required to change 92901 to XXX01
#define APP_INVALID_OBJECT                          92901
        _pErrorHandler->insertErrorDiagnostics((int)APP_INVALID_OBJECT,
                string("Class : Invalid object. "));
#define APP_MAX_PROCESSING_AREA_NOT_SET             92902
        _pErrorHandler->insertErrorDiagnostics(
                (int)APP_MAX_PROCESSING_AREA_NOT_SET,
                string("Class : Max processing area is not set."));
#define APP_NULL_IMAGE                              92903
        _pErrorHandler->insertErrorDiagnostics((int)APP_NULL_IMAGE,
                string("Class : Image is null."));
#define APP_INCONSISTENT_IMAGES                     92904
        _pErrorHandler->insertErrorDiagnostics((int)APP_INCONSISTENT_IMAGES,
                string("Class : Inconsistent images."));
#define APP_ROI_NOT_SET_YET                         92905
        _pErrorHandler->insertErrorDiagnostics((int)APP_ROI_NOT_SET_YET,
                string("Class : ROI is not set yet."));
#define APP_INVALID_ROI                             92906
        _pErrorHandler->insertErrorDiagnostics((int)APP_INVALID_ROI,
                string("Class : Invalid ROI. Minimum ROI is 2 x 2 pixels"));
#define APP_INVALID_NUM_CHANNELS                    92907
        _pErrorHandler->insertErrorDiagnostics((int)APP_INVALID_NUM_CHANNELS,
                string("Class : Image has invalid number of channels."));
#define APP_INVALID_CAMERA                          92908
        _pErrorHandler->insertErrorDiagnostics((int)APP_INVALID_CAMERA,
                string("InspectionDetectoor : Invalid camera specified"));
#define APP_INVALID_VIDEO_WRITER                    92909
        _pErrorHandler->insertErrorDiagnostics((int)APP_INVALID_VIDEO_WRITER,
                string("Class : Invalid video writer."));
#define APP_INVALID_TIMESTAMP_FILE                  92910
        _pErrorHandler->insertErrorDiagnostics((int)APP_INVALID_TIMESTAMP_FILE,
                string("Class : Invalid timestamp file."));
#define APP_MODULE_NOT_SET                          92911
        _pErrorHandler->insertErrorDiagnostics((int)APP_MODULE_NOT_SET,
                string("Class : Module is not set."));
#define APP_INVALID_MODULE                          92912
        _pErrorHandler->insertErrorDiagnostics((int)APP_INVALID_MODULE,
                string("Class : Module is invalid."));
#define APP_ERROR_ON_SYSTEM_CALL                    92913
        _pErrorHandler->insertErrorDiagnostics((int)APP_ERROR_ON_SYSTEM_CALL,
                string("Class : Error on system call."));
#define APP_SET_CAM_RES_BEFORE_SET_RESIZE           92914
        _pErrorHandler->insertErrorDiagnostics(
                (int)APP_SET_CAM_RES_BEFORE_SET_RESIZE,
                string(
                        "FrameGrabber : Set camera resolution before setting resize."));
#define APP_RESIZE_HEIGHT_GT_IMG_HEIGHT             92915
        _pErrorHandler->insertErrorDiagnostics(
                (int)APP_RESIZE_HEIGHT_GT_IMG_HEIGHT,
                string(
                        "FrameGrabber : Resize image height >= Grabbed image height"));

        // Escapable Exceptions
    }
    App::_bIsSetErrorDiagnosticsList = true;
}

App::App()
{
    App::_iObjCount++;
    _pErrorHandler = ErrorHandler::getInstance();
    _pLogger = Logger::getInstance();
    _pClock = new Clock();
    _pMotionDetector = new MotionDetector();
    _pProcessingRegions = new Regions();
    _pTimer = new Timer();
    _pUtilities = new Utilities();

    _pImage3Channel = NULL;
    _pImageResized = NULL;
    _iImageROIWidth = -1;
    _iImageROIHeight = -1;

    _pImageProcess = NULL;
    _pImageProcessGray = NULL;

    _pImageProcessingMaskROI = NULL;
    _pImageProcessingMaskROI_Neg = NULL;
    _pImageProcessingMaskROI_cropped = NULL;
    _iOffset_TL_X = 0;
    _iOffset_TL_Y = 0;

    _pImageProcessingMaskROI = NULL;

    _bIsSystemInitialized = false;
    _iFrameNo = 0;
    _iFrameIntvlCount = 0;
    _bIsSetProcessingStartTime = false;

    _ssOut.str("");
    _sText = string("");

    // Font initialization
    cvInitFont(&_font1, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 1, CV_AA);
    cvInitFont(&_font2, CV_FONT_HERSHEY_PLAIN, 3, 3, 0, 2, CV_AA);
    _textColor = CV_RGB(255, 0, 0);

    _pMemStorage = cvCreateMemStorage(0);

    // Saving to video file
    _pVideoWriter = NULL;
    _iVideoFileNo = 1;
    _bSaveFrameToVideo = false;
    _bCreateNewVideoFile = true;
    _iFrameNoInVideo = 0;

    // Terminate program
    _bTerminateProgram = false;

    // Processing framerate: Initialization
    _tvPrev = _pTimer->getCurrentTime();
    _dProcessingFPS = 0.0f;

    // Keep track of time between frames
    _fTimeDiffMs = -1.0f;
    _bIsSetTimeDiffMs = false;
    _bIsSetFrameCapturePrev = false;
    _vtvTimeBetweenFrames.clear();
    _bSlowDown = false;

    initErrorDiagnosticsList();

    _pFrameGrabber = NULL;
    _iLocationID = 1;
}

App::~App()
{
    App::_iObjCount--;

    // == Write to Log file ==
    // Time of exiting the program
    string sEndTime = _pClock->getCurrentDateTimeString(false);
    _pLogger->writeToFile(FILE_LOG, string("End Time :") + sEndTime);

    // No. of frames grabbed.
    _ssOut.str("");
    _ssOut << _iFrameNo;
    _pLogger->writeToFile(FILE_LOG,
            string("No. of frames grabbed:" + _ssOut.str()));

    if (_pClock) delete _pClock;
    if (_pMotionDetector) delete _pMotionDetector;
    if (_pProcessingRegions) delete _pProcessingRegions;
    if (_pTimer) delete _pTimer;
    if (_pUtilities) delete _pUtilities;

    if (_pErrorHandler) _pErrorHandler = NULL;
    if (_pLogger) _pLogger = NULL;

    _pImage3Channel = NULL;
    if (_pImageResized) cvReleaseImage(&_pImageResized);
    if (_pImageProcess) cvReleaseImage(&_pImageProcess);
    if (_pImageProcessGray) cvReleaseImage(&_pImageProcessGray);
    if (_pImageResults) cvReleaseImage(&_pImageResults);

    if (_pImageProcessingMaskROI) _pImageProcessingMaskROI = NULL;

    if (_pVideoWriter) cvReleaseVideoWriter(&_pVideoWriter);
    if (_fsVideoTimestampYML.isOpened()) _fsVideoTimestampYML.release();

    if (_pMemStorage) cvReleaseMemStorage(&_pMemStorage);

    if (!_fsTimestampFileRead.isOpened()) _fsTimestampFileRead.release();
    if (_fsConfigFileRead.isOpened()) _fsConfigFileRead.release();

    if (_pFrameGrabber) _pFrameGrabber = NULL;
}

string App::getOutputFolder()
{
    return string(Logger::getInstance()->getOutputFolder());
}

void App::setFrameGrabberObj(FrameGrabber *pFrameGrabberObj)
{
    this->_pFrameGrabber = pFrameGrabberObj;
}

void App::startApp(bool bDebug)
{
    // Initialize other objects and set variables for them
    _pLogger->writeToFile(FILE_LOG, "Location ID", _iLocationID);
    cout << "Location ID: " << _iLocationID << endl;

    // Start grabbing frames
    Frame *pFrame = new Frame();
    pFrame->bActive = false;
    pFrame->iFrameID = -1;
    pFrame->tvTimeCapture = _pTimer->getCurrentTime();
    if (App::_bResizeImage && App::_bStreamResizedImg)
    {
        // Resized image will be returned.
        pFrame->pImage = cvCreateImage(
                cvSize(App::_iResizedImgWidth, App::_iResizedImgHeight),
                IPL_DEPTH_8U, 3);
    }
    else
    {
        // Original image will be returned.
        pFrame->pImage = cvCreateImage(
                cvSize(App::_iCamera1Width, App::_iCamera1Height),
                IPL_DEPTH_8U, 3);
    }
    cvSetZero(pFrame->pImage);

    IplImage *pImageGrabbed = NULL;
    _iFrameNo = 0;

    // Enable user to select an initial frame (for e.g. for Bg Subtraction)
    bool bEnableSelectingFirstFrame = false;
    while (true)
    {
        bool bHasFrame = false;
        pFrame->bActive = false;
        pFrame->iFrameID = -1;
        bHasFrame = _pFrameGrabber->grabFrame(pFrame);
        if (!bHasFrame)
        {
            if (App::_debugMode == INFO) cout
                    << "\t---[Grab]---No new frame. Trying again..." << endl;
            if (getTerminateProgram()) break;
            continue;
        }
        pImageGrabbed = pFrame->pImage;
        _tvFrameCapture.tv_sec = pFrame->tvTimeCapture.tv_sec;
        _tvFrameCapture.tv_usec = pFrame->tvTimeCapture.tv_usec;

        // Enable user to control frame flow (pause, continue etc.)
        bool bSkipFrame = false;
        if (_bDispImgCapture) displayCapturedImageForUserControl(pImageGrabbed,
                bSkipFrame);
        if (bSkipFrame) continue;

        // Enable user to specify first frame for processing.
        if (bEnableSelectingFirstFrame)
        {
            if (!setFrameForInit(pImageGrabbed)) continue;
        }

        if (_bProcessFrames) processFrame(pImageGrabbed, bDebug);

        // Prepare for program termination.
        if (bTerminate || _pFrameGrabber->getTerminateProgram()) // Either from end of capture or Ctrl+C or kill
        {
            cerr << "Program terminated by user." << endl;
            cerr << "Cleaning up ..." << endl;
            setTerminateProgram(true);
            break;
        }

        if (getTerminateProgram()) break;
        usleep(15000); // 15ms
        _iFrameNo++;
    }
}

bool App::setFrameForInit(IplImage *pImageGrabbed)
{
    bool bIsSetFrameForInit = false;
    bool bMsgToSelectFrameDisplayed = false;
    string sWindowName = string("Grabbed image");
    char chKey = 0;
    if (!bIsSetFrameForInit)
    {
        if (!bMsgToSelectFrameDisplayed)
        {
            // Enable user to skip frames to find the right frame to define regions
            cout << endl << endl << "Select a frame for initialization." << endl
                    << "Press 'Q' to select frame" << endl;
            bMsgToSelectFrameDisplayed = true;
        }
        cvNamedWindow(sWindowName.c_str(), 0);
        _pUtilities->writeTextOnImage(pImageGrabbed,
                "Select a frame for initialization.", 0);
        _pUtilities->writeTextOnImage(pImageGrabbed,
                "Press 'Q' to select frame.", 1);
        cvShowImage(sWindowName.c_str(), pImageGrabbed);
        //cvSetWindowProperty(sWindowName.c_str(), CV_WND_PROP_FULLSCREEN,
        //        CV_WINDOW_FULLSCREEN);
        if (_sCamera1.compare("emulator") == 0)
        {
            chKey = cvWaitKey(100);
        }
        else
        {
            chKey = cvWaitKey(1000);
        }

        if (chKey == 'q' || chKey == 'Q')
        {
            bIsSetFrameForInit = true;
        }
    }
    return bIsSetFrameForInit;
}

void App::displayCapturedImageForUserControl(IplImage *pImageGrabbed,
        bool &bSkipFrame, bool bDebug)
{
    bSkipFrame = false;
    if (_iFrameNo % 10 == 0)
    {
        if (_iFrameNo % 100 == 0) cout << "Displaying every 10th frame..."
                << endl;
#if 0
        _pTimer->start();
#endif
        _pFrameGrabber->writeTimestampOnFrame(pImageGrabbed);
        _pUtilities->displayImage(pImageGrabbed, string("Grabbed frame"));
    }
    char chKey;
    if (APP_ENABLE_SKIPPING_FRAMES_MANUALLY || APP_ENABLE_PAUSE
            || APP_ENABLE_SLOWDOWN)
    {
        chKey = waitKey(40);
        if (chKey == 'c' || chKey == 'C') // skip
        {
            bSkipFrame = true;
            return;
        }
        else if (chKey == 'p' || chKey == 'P') // pause
        {
            cout << endl << "Paused. Press 'p' to continue..." << endl;
            chKey = waitKey(250);
            while (chKey != 'p' || chKey == 'P')
            {
                chKey = waitKey(1000);
            }
        }
        else if (chKey == 's' || chKey == 'S') // slow
        {
            _bSlowDown = !_bSlowDown;
        }
        if (_bSlowDown)
        {
            cout << endl << "Slow down mode. "
                    << "Press 's' to disable slow down..." << endl;
            chKey = waitKey(1000);
            if (chKey == 's' || chKey == 'S')
            {
                _bSlowDown = !_bSlowDown;
            }
        }
#if 0
        cout << "Time to display " << _pImageGrabbed->width << "x"
        << _pImageGrabbed->height << " image: "
        << _pTimer->getElapsedTimeInMilliSec() << "ms" << endl;
        cvWaitKey(1);
#endif
    }
}

void App::processFrame(const IplImage *pImageC, bool bDebug)
{
    // Compute processing frame rate
    if (_iFrameNo % 30 == 0)
    {
        timeval tvCur = _pTimer->getCurrentTime();
        double dDiffInMs = _pTimer->getElapsedTimeInMilliSec(_tvPrev, tvCur);
        double dNumFrames = 30;
        int iNumFramesSkipInterval = APP_NUM_FRAMES_SKIP_INTERVAL;
        if (iNumFramesSkipInterval > 0)
        {
            dNumFrames /= iNumFramesSkipInterval;
        }
        double dTimeForOneFrame = dDiffInMs / dNumFrames;
        _dProcessingFPS = 1000 / dTimeForOneFrame;
        //_dProcessingFPS = cvRound(_dProcessingFPS * 100) / 100;
        cout << "\tProcessing at " << _dProcessingFPS << " fps" << endl;
        if (APP_ENABLE_PAUSE)
        {
            cout << "Press 'p' to pause execution." << endl;
        }
        cout << endl;
        _tvPrev = tvCur;
    }

    // The frames are evaluated at a regular interval to reduce computation time to increase speed
    if (_iFrameIntvlCount < APP_NUM_FRAMES_SKIP_INTERVAL)
    {
        _iFrameIntvlCount++;
        return;
    }
    _iFrameIntvlCount = 0;

    if (pImageC->nChannels != 3)
    {
        _pErrorHandler->setErrorCode((int)APP_INVALID_NUM_CHANNELS,
                string("Need a 3-channel image."));
    }
    _pImage3Channel = (IplImage*)pImageC;

    if (!_bIsSystemInitialized)
    {
        if (App::_bResizeImage)
        {
            if (App::_bStreamResizedImg)
            {
                // Frame has resized image.
                if (_pImageResized) cvReleaseImage(&_pImageResized);
                _pImageResized = cvCloneImage(_pImage3Channel);
            }
            else
            {
                // Frame has original image. Use resized image.
                if (!_pImageResized)
                {
                    _pImageResized = cvCreateImage(
                            cvSize(App::_iResizedImgWidth,
                                    App::_iResizedImgHeight),
                            IPL_DEPTH_8U, _pImage3Channel->nChannels);
                }
                cvResize(_pImage3Channel, _pImageResized);
            }
        }
        else
        {
            // Frame has original image. Use original image.
            if (_pImageResized) cvReleaseImage(&_pImageResized);
            _pImageResized = cvCloneImage(_pImage3Channel);
        }

        // System initialization
        initializeSystem();
    }

    if (App::_bResizeImage)
    {
        if (App::_bStreamResizedImg)
        {
            // Frame has resized image.
            cvCopyImage(_pImage3Channel, _pImageResized);
        }
        else
        {
            // Frame has original image. Use resized image.
            cvResize(_pImage3Channel, _pImageResized);
        }
    }
    else
    {
        // Frame has original image. Use original image.
        cvCopyImage(_pImage3Channel, _pImageResized);
    }

    // Specify the ROI on the image
    cvSet(_pImageResized, cvScalar(0, 0, 0), _pImageProcessingMaskROI_Neg);
    cvSetImageROI(_pImageResized, _rectProcessingMaskROI);
    cvCopyImage(_pImageResized, _pImageProcess);
    cvResetImageROI(_pImageResized);
    if (App::_bDispImgResults) cvCopyImage(_pImageResized, _pImageResults);

    if (bDebug) _pUtilities->displayImage(_pImageProcess, "Image to process");

    // TODO: Do what is to be done.
    // ...

    if (App::_bDispImgResults)
    {
        prepareResultsImage();
        _pUtilities->displayImage(_pImageResults, "Results");
    }
}

// Prepare results image
void App::prepareResultsImage()
{
    // ...
}

void App::createVideoFile(int iVideoFileNo, IplImage *pImage)
{
    string sOutputFolder = getOutputFolder();
    string sDateTime = Clock::getFormattedDateTimeStringForMySQL(
            Clock::getCurrentTimeStampInSec());

    // Create new video file
    string sVideoFile = sOutputFolder + sDateTime + string(".avi");
    if (_pVideoWriter)
    {
        cvReleaseVideoWriter(&_pVideoWriter);
    }
    _pVideoWriter = cvCreateVideoWriter(sVideoFile.c_str(),
            CV_FOURCC('M', 'P', '4', 'V'), APP_FRAMERATE_IN_VIDEO_CLIP,
            cvGetSize(pImage), true);

    // Create a corresponding timestamp file
    string sTimestampFile = sOutputFolder + sDateTime + string(".yml");
    if (_fsVideoTimestampYML.isOpened())
    {
        _fsVideoTimestampYML.release();
    }
    _fsVideoTimestampYML.open(sTimestampFile, FileStorage::WRITE);
}

// TODO: Need to create a player to play the video with the timestamp, so
// that if the time between two frames is large, this needs to be simulated.
void App::saveFrameToVideo(IplImage *pImage, int iFrameNo)
{
    _iFrameNoInVideo++;
    cout << "Saving frame " << iFrameNo << " to video frame "
            << _iFrameNoInVideo << endl;
    _tvTimeOfFrameCapture = _pCamera->getTimeOfFrameCapture();

    if (_pVideoWriter)
    {
        cvWriteFrame(_pVideoWriter, pImage);
    }
    else
    {
        _pErrorHandler->setErrorCode((int)APP_INVALID_VIDEO_WRITER);
    }
    if (_fsVideoTimestampYML.isOpened())
    {
        writeToVideoTimestampFile(_tvTimeOfFrameCapture, _iFrameNoInVideo);
    }
    else
    {
        _pErrorHandler->setErrorCode((int)APP_INVALID_TIMESTAMP_FILE);
    }
}

void App::writeToVideoTimestampFile(timeval curTime, int iFrameNo, bool bDebug)
{
    _ssOut.str("");
    _ssOut << iFrameNo;
    string sFrameNo = string("f") + _ssOut.str();

    time_t tFrameTime;
    time(&tFrameTime);
    string sTime1 = _pClock->getFormattedDateTimeString(tFrameTime);
    double dTime1 = (double)(long)tFrameTime;
    double dSec = curTime.tv_sec;
    double dMicroSec = curTime.tv_usec;

    _fsVideoTimestampYML << sFrameNo << "{";
    _fsVideoTimestampYML << "sTime1" << sTime1;
    _fsVideoTimestampYML << "dTime1" << dTime1;
    _fsVideoTimestampYML << "dSec" << dSec;
    _fsVideoTimestampYML << "dMicroSec" << dMicroSec;
    _fsVideoTimestampYML << "}";
    if (bDebug)
    {
        cout << "sTime1: " << sTime1 << endl;
        cout << "dTime1: " << dTime1 << endl;
        cout << "dSec: " << dSec << endl;
        cout << "dMicroSec: " << dMicroSec << endl;
    }
}

void App::specifyProcessingRegions(bool bDebug)
{
    // == Specify Processing area(s) ==
    bool bEnableMultipleProcessingAreas = APP_ENABLE_MULTIPLE_PROCESSING_AREAS;
    _pProcessingRegions->setImage(_pImageResized);
    String sWindowName = string("Processing Area");
    int iNumProcessingAreas = 1;
    if (bEnableMultipleProcessingAreas)
    {
        cout << endl << "No. of processing areas:";
        cin >> iNumProcessingAreas;
        if (iNumProcessingAreas < 0)
        {
            cerr << "Invalid number of processing areas specified." << endl
                    << "Setting it to 1." << endl;
            iNumProcessingAreas = 1;
        }
    }

    for (int iCount = 0; iCount < iNumProcessingAreas; iCount++)
    {
        _ssOut.str("");
        _ssOut << (iCount + 1);
        sWindowName = string("Processing Area ") + _ssOut.str();
        _pProcessingRegions->setWindowName(sWindowName);
        _pProcessingRegions->setOverlayProperties(true, 1,
                cvScalar(255, 255, 0), cvScalar(0, 255, 0));
        _pProcessingRegions->specifyRegionManually("Specify Processing area",
                "Left click: Set point, Right click: Close region, 'Q': Exit");
        void* pvoidWinHandle = cvGetWindowHandle(sWindowName.c_str());
        if (pvoidWinHandle != 0) cvDestroyWindow(sWindowName.c_str());
    }
    _pImageProcessingMaskROI = cvCloneImage(
            _pProcessingRegions->getImageRegionsMask());
}

// Specify _pImageProcessingMaskROI
void App::initializeSystem(bool bDebug)
{
    if (!_bIsSystemInitialized)
    {
        cout << endl << "Initializing App system ..." << endl;

        // Specify violation regions and processing regions
        if (App::_bUseWholeImageAsROI)
        {
            // Specify ROI as whole image
            _pProcessingRegions->setImage(_pImageResized);
            Region *pRegion = new Region();
            CvPoint pt;
            pt.x = 0;
            pt.y = 0;
            pRegion->addPt(pt);
            pt.x = _pImageResized->width - 1;
            pt.y = 0;
            pRegion->addPt(pt);
            pt.x = _pImageResized->width - 1;
            pt.y = _pImageResized->height - 1;
            pRegion->addPt(pt);
            pt.x = 0;
            pt.y = _pImageResized->height - 1;
            pRegion->addPt(pt);
            pt.x = 0;
            pt.y = 0;
            pRegion->addPt(pt);
            _pProcessingRegions->addRegion(pRegion);
        }
        else
        {
            // Specify violation regions and processing regions
            if (_bIsSetRegionsFromConfigFile)
            {
                // Get Processing Regions
                string sProcessingRegion = "ProcessingRegion";
                string sNode = string("num") + sProcessingRegion;
                int iNumProcessingRegions = (int)_fsConfigFileRead[sNode];
                _pProcessingRegions->setImage(_pImageResized);
                for (int iRegion = 0; iRegion < iNumProcessingRegions;
                        iRegion++)
                {
                    _ssOut.str("");
                    _ssOut << iRegion;
                    sNode = sProcessingRegion + _ssOut.str();
                    Mat matPts;
                    _fsConfigFileRead[sNode] >> matPts;
                    CvMat matPoints = matPts;
                    Region *pRegion = new Region();
                    CvPoint pt;
                    for (int iPt = 0; iPt < matPoints.rows; iPt++)
                    {
                        pt.x = CV_MAT_ELEM(matPoints, int, iPt, 0);
                        pt.y = CV_MAT_ELEM(matPoints, int, iPt, 1);
                        pRegion->addPt(pt);
                    }
                    _pProcessingRegions->addRegion(pRegion);
                }
                _pImageProcessingMaskROI = cvCloneImage(
                        _pProcessingRegions->getImageRegionsMask());
            }
            else
            {
                specifyProcessingRegions(bDebug);
            }
        }

        // Get the first processing region
        Region *pProcessingRegion = _pProcessingRegions->getRegions()[0];
        _pImageProcessingMaskROI = _pProcessingRegions->getRegionMask(
                pProcessingRegion);
        _pImageProcessingMaskROI_Neg = cvCloneImage(_pImageProcessingMaskROI);
        cvNot(_pImageProcessingMaskROI_Neg, _pImageProcessingMaskROI_Neg);
        _rectProcessingMaskROI = _pUtilities->getROIFromAreaMask(
                _pImageProcessingMaskROI, bDebug);

        // Save the image with processing regions highlighted.
        string sFilename = this->getOutputFolder()
                + string("ProcessingRegions.jpg");
        IplImage *pImageSelection = _pProcessingRegions->getImageSelection();
        _pUtilities->overlayImage(_pImageProcessingMaskROI, pImageSelection, 1,
                true);
        cvSaveImage(sFilename.c_str(), pImageSelection);
        _pProcessingRegions->saveToConfigFile(
                string("ProcessingRegion").c_str(), bDebug);

        // == Validate the ROI that has been specified ==
        CvRect rect = _rectProcessingMaskROI;
        if (bDebug)
        {
            cout << rect.x << " " << rect.y << " " << rect.width << " "
                    << rect.height << endl;
        }
        _pLogger->writeToFile(FILE_LOG, "Processing Mask ROI No. ", 1);
        _pLogger->writeToFile(FILE_LOG, "ROI.x", rect.x);
        _pLogger->writeToFile(FILE_LOG, "ROI.y", rect.y);
        _pLogger->writeToFile(FILE_LOG, "ROI.width", rect.width);
        _pLogger->writeToFile(FILE_LOG, "ROI.height", rect.height);

        if (_rectProcessingMaskROI.width < 2
                || _rectProcessingMaskROI.height < 2
                || _rectProcessingMaskROI.x < 0 || _rectProcessingMaskROI.y < 0
                || _rectProcessingMaskROI.x + _rectProcessingMaskROI.width
                        > _pImageResized->width
                || _rectProcessingMaskROI.y + _rectProcessingMaskROI.height
                        > _pImageResized->height)
        {
            _pErrorHandler->setErrorCode((int)APP_INVALID_ROI);
        }
        if (_pImageProcessingMaskROI_cropped) cvReleaseImage(
                &_pImageProcessingMaskROI_cropped);
        _pImageProcessingMaskROI_cropped = cvCreateImage(
                cvSize(_rectProcessingMaskROI.width,
                        _rectProcessingMaskROI.height),
                _pImageProcessingMaskROI->depth,
                _pImageProcessingMaskROI->nChannels);
        cvSetImageROI(_pImageProcessingMaskROI, _rectProcessingMaskROI);
        cvCopyImage(_pImageProcessingMaskROI, _pImageProcessingMaskROI_cropped);
        cvResetImageROI(_pImageProcessingMaskROI);
        if (bDebug)
        {
            cout << "ProcessingMaskROI_cropped[" << "] (w,h): ("
                    << _pImageProcessingMaskROI_cropped->width << " "
                    << _pImageProcessingMaskROI_cropped->height << ")" << endl;
            cout << "_rectProcessingMaskROI[" << "] (w,h): ("
                    << _rectProcessingMaskROI.width << " "
                    << _rectProcessingMaskROI.height << ")" << endl;
        }

        // Initialize other objects
        // ...
        if (!_bUseWholeImageAsROI)
        {
            _iOffset_TL_X = _rectProcessingMaskROI.x;
            _iOffset_TL_Y = _rectProcessingMaskROI.y;
        }
        int iWidth = _rectProcessingMaskROI.width;
        int iHeight = _rectProcessingMaskROI.height;

        // == Initializing images ==
        _pImageProcess = cvCreateImage(cvSize(iWidth, iHeight),
        IPL_DEPTH_8U, 3);
        _pImageProcessGray = cvCreateImage(cvSize(iWidth, iHeight),
        IPL_DEPTH_8U, 1);
        _pImageResults = cvCreateImage(
                cvSize(_pImageResized->width, _pImageResized->height),
                IPL_DEPTH_8U, 3);
    }
    else
    {
        _pErrorHandler->setErrorCode((int)APP_INVALID_ROI);
    }

    // Set system as initialized.
    _bIsSystemInitialized = true;
    cout << "System initialized." << endl;
}

// == Getters and Setters ==

int App::getObjCount()
{
    return App::_iObjCount;
}

int App::getFrameNo()
{
    return _iFrameNo;
}

// Program parameters

void App::setDebugMode(teDebugMode debugMode)
{
    App::_debugMode = debugMode;
}

void App::setNumFramesToIgnore(int iNumFramesToIgnore, bool bDebug)
{
    _iNumFramesToIgnore = iNumFramesToIgnore;
}

int App::getNumFramesToIgnore()
{
    return App::_iNumFramesToIgnore;
}

void App::setCamera1(string sCamera1)
{
    _sCamera1 = sCamera1;
}

void App::setVideo1Path(string sVideo1Path)
{
    _sVideo1Path = sVideo1Path;
}

void App::setCamera1No(int iCamera1No)
{
    _iCamera1No = iCamera1No;
}

void App::setCaptureMode1(int iCaptureMode1)
{
    _iCaptureMode1 = iCaptureMode1;
}

void App::setImageType1(int iImageType1)
{
    _iImageType1 = iImageType1;
}

void App::setFPS(float fFPS)
{
    App::_fFPS = fFPS;
}

void App::setNumFramesToGrab(int iNumFramesToGrab)
{
    _iNumFramesToGrab = iNumFramesToGrab;
}

void App::setCamera1Resolution(int iWidth, int iHeight)
{
    App::_iCamera1Width = iWidth;
    App::_iCamera1Height = iHeight;
    App::_bIsSetCamRes = true;
}

void App::setResizeImage(bool bResizeImage, int iResizedImgHeight,
        bool bStreamResizedImg)
{
    App::_bResizeImage = bResizeImage;
    if (bResizeImage)
    {
        if (!App::_bIsSetCamRes)
        {
            _pErrorHandler->setErrorCode(
                    (int)APP_SET_CAM_RES_BEFORE_SET_RESIZE);
        }
        if (iResizedImgHeight >= App::_iCamera1Height)
        {
            _pErrorHandler->setErrorCode((int)APP_RESIZE_HEIGHT_GT_IMG_HEIGHT);
        }
        App::_iResizedImgHeight = iResizedImgHeight;
        float fHeightRatio = (float)App::_iResizedImgHeight
                / App::_iCamera1Height;
        App::_iResizedImgWidth = cvCeil(fHeightRatio * App::_iCamera1Width);
        App::_bStreamResizedImg = bStreamResizedImg;
    }
}

void App::setConfigFile(char *pchConfigFile)
{
    _bIsSetConfigFile = true;
    _sConfigFile = string(pchConfigFile);
    if (!_fsConfigFileRead.isOpened())
    {
        _fsConfigFileRead.open(_sConfigFile, FileStorage::READ);
    }
}

void App::setRegionsFromConfigFile(bool bSetRegionsFromConfigFile)
{
    _bIsSetRegionsFromConfigFile = bSetRegionsFromConfigFile;
}

void App::setTimestampFile(char *pchTimestampFile)
{
    _bIsSetTimestampFile = true;
    _sTimestampFile = string(pchTimestampFile);
    if (!_fsTimestampFileRead.isOpened())
    {
        _fsTimestampFileRead.open(_sTimestampFile, FileStorage::READ);
    }
}

void App::setDispImgCapture(bool bDispImgCapture)
{
    App::_bDispImgCapture = bDispImgCapture;
}

void App::setDispImgResults(bool bDispImgResults)
{
    App::_bDispImgResults = bDispImgResults;
}

void App::setResizeImage(bool bResizeImage, int iImgHeight)
{
    if (iImgHeight == _iCamera1Height) bResizeImage = false;
    App::_bResizeImage = bResizeImage;
    if (bResizeImage) App::_iResizedImgHeight = iImgHeight;
}

void App::setUseWholeImageAsROI(bool bUseWholeImageAsROI)
{
    App::_bUseWholeImageAsROI = bUseWholeImageAsROI;
}

void App::setProcessFrames(bool bProcessFrames)
{
    App::_bProcessFrames = bProcessFrames;
}

// = Program termination by user control =

void App::setTerminateProgram(bool bTerminateProgram)
{
    _bTerminateProgram = bTerminateProgram;
    _pFrameGrabber->setTerminateProgram(bTerminateProgram);
}

bool App::getTerminateProgram()
{
    return _bTerminateProgram;
}

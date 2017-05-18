/*
 * AppTest.cpp
 * Ramesh Marikhu <marikhu@gmail.com>
 * Template for integrating FrameGrabber module 
 * 4 May 2016
 */

#include <signal.h>
#include <iostream>
using namespace std;

#include <cv.h>
#include <highgui.h>
using namespace cv;

#include "CameraCaptureConfig.h"
#include "App.h"
#include "AppConfig.h"
#include "FrameGrabber.h"
#include "FrameGrabberConfig.h"
#include "ErrorHandler.h"
#include "Logger.h"
#include "Utilities.h"
#include "SystemConfig.h"

#define OUTPUT_FOLDER           "../output/"
#define PROJECT_FOLDER          "AppTest"

bool volatile bTerminate = false;
void sigterm_handler(int i)
{
    cout << "Program terminated by user." << endl;
    bTerminate = true;
}

int main(int argc, char* argv[])
{
    cout << "\n\n" << endl;
    cout << "===================================" << endl;
    cout << "=   App Test with FrameGrabber    =" << endl;
    cout << "= --------------------------------=" << endl;
    cout << "=     CSIM/ISE, AIT, Thailand     =" << endl;
    cout << "===================================" << endl;

    // SIGTERM handler to terminate the program properly.
    bTerminate = false;
    struct sigaction sa;
    sa.sa_handler = &sigterm_handler;
    sigaction(SIGTERM, &sa, NULL);    // kill signal handler
    signal(SIGINT, sigterm_handler);  // Ctrl+C handler

    Logger *pLoggerTest = Logger::getInstance();
    ErrorHandler *pErrorHandlerTest = ErrorHandler::getInstance();
    Utilities *pUtilities = new Utilities();

    // == Initialization ==
    teDebugMode debugMode = NO_DEBUG;
    string sLicenseFile = string("");
    bool bIsSetLicenseFile = false;
    bool bUseCameraEmulator = false;
    string sCamera1 = string("");
    string sVideo1Path = string("");
    int iCamera1No = -1;
    int iCaptureMode1 = (int)CAPTURE_MODE;
    int iCameraResWidth = -1;
    int iCameraResHeight = -1;
    bool bResizeImage = false;
    int iResizedImgHeight = -1;
    bool bStreamResizedImg = false;
    string sProxy = string("");
    bool bIsSetProxy = false;
    float fFPS = 30.0f;
    int iImageType = (int)IMAGE_TYPE;
    bool bFileSizeGt2GB = false;
    int iNumFramesToGrab = NUM_FRAMES_TO_GRAB;
    int iNumFramesToIgnore = 0;
    bool bProcessFrames = false;
    bool bSaveVideo = false;
    string sOutputFolder = OUTPUT_FOLDER;
    bool bSetConfigFile = false;
    string sConfigFile = string("");
    bool bSetRegionsFromConfigFile = false;
    bool bUseWholeImageAsROI = false;
    bool bSkipFrozenFrames = false;

    // Display image windows
    bool bDispImgCapture = false;
    bool bDispImgResults = false;

    // Write timestamp on frame
    bool bWriteTimestampOnFrame = false;

    // Circular buffer
    bool bEnableCircBuf = false;
    int iNumFramesInCircBuf = FG_MIN_NUM_FRAMES_IN_CAPTURE_CIRC_BUF;
    bool bIsSetThreadPriority = false;
    int iPriorityValue = 0;

    // == Parse arguments passed to the program ==
    cout << "Obtaining inputs from arguments passed to the program..." << endl;
    int iArgsCount = 1;

    string sArguments = string("Arguments passed to the program:\n");
    while (iArgsCount < argc)
    {
        sArguments += string(argv[iArgsCount]) + string(" ");
        iArgsCount++;
    }

    // Parse the arguments effectively for increased flexibility
    bool bValidArgs = true;
    iArgsCount = 1;
    while (iArgsCount < argc)
    {
        string sCommand = string(argv[iArgsCount]);
        if (sCommand.compare("-c1") == 0)
        {
            if ((iArgsCount + 1) >= argc)
            {
                bValidArgs = false;
                break;
            }
            sCamera1 = string(argv[iArgsCount + 1]);
            if (!(sCamera1.compare(string("emulator")) == 0
                    || sCamera1.compare(string("ipcam")) == 0
                    || sCamera1.compare(string("usb")) == 0))
            {
                bValidArgs = false;
                cout << "Camera must be set to one of the followings:" << endl
                        << "{ emulator, ipcam, usb }" << endl;
                break;
            }
            iArgsCount++;
        }
        else if (sCommand.compare("-p1") == 0)
        {
            sVideo1Path = string(argv[iArgsCount + 1]);
            if (sVideo1Path.length() < 5)
            {
                bValidArgs = false;
                cout
                        << "Check the specified path of the video file or video stream."
                        << endl;
                break;
            }
            iArgsCount++;
        }
        else if (sCommand.compare("-n1") == 0)
        {
            iCamera1No = pUtilities->convertStringToInteger(
                    string(argv[iArgsCount + 1]));
            if (iCamera1No < 0 || iCamera1No > 10)
            {
                bValidArgs = false;
                cout << "Camera No. should be specified in the range [0 10]"
                        << endl;
                break;
            }
            iArgsCount++;
        }
        else if (sCommand.compare("-cm1") == 0)
        {
            iCaptureMode1 = pUtilities->convertStringToInteger(
                    string(argv[iArgsCount + 1]));
            if (!(iCaptureMode1 == (int)VIDEO
                    || iCaptureMode1 == (int)IMAGE_LIST
                    || iCaptureMode1 == (int)IMAGE))
            {
                bValidArgs = false;
                cout
                        << "Capture mode should be set for emulator only, from the set {video = 1, image_list = 2, image = 3}"
                        << endl;
                break;
            }
            iArgsCount++;
        }
        else if (sCommand.compare("-w1") == 0)
        {
            iCameraResWidth = pUtilities->convertStringToInteger(
                    string(argv[iArgsCount + 1]));
            if (iCameraResWidth < 240 || iCameraResWidth > 4000)
            {
                bValidArgs = false;
                cout << "Width of the image should be in the range [240, 4000]"
                        << endl;
                break;
            }
            iArgsCount++;
        }
        else if (sCommand.compare("-h1") == 0)
        {
            iCameraResHeight = pUtilities->convertStringToInteger(
                    string(argv[iArgsCount + 1]));
            if (iCameraResHeight < 240 || iCameraResHeight > 4000)
            {
                bValidArgs = false;
                cout << "Height of the image should be in the range [240, 1944]"
                        << endl;
                break;
            }
            iArgsCount++;
        }
        else if (sCommand.compare("-resizeimage") == 0)
        {
            bResizeImage = true;
            iResizedImgHeight = pUtilities->convertStringToInteger(
                    string(argv[iArgsCount + 1]));
            iArgsCount++;
        }
        else if (sCommand.compare("-streamresizedimg") == 0)
        {
            bStreamResizedImg = true;
        }
        else if (sCommand.compare("-proxy") == 0)
        {
            sProxy = string(argv[iArgsCount + 1]);
            if (sProxy.length() < 1)
            {
                bValidArgs = false;
                cout << "Invalid proxy. "
                        << "Do not use -proxy option if no proxy needed."
                        << endl;
                break;
            }
            bIsSetProxy = true;
            iArgsCount++;
        }
        else if (sCommand.compare("-fps") == 0)
        {
            fFPS = pUtilities->convertStringToFloat(
                    string(argv[iArgsCount + 1]));
            if (fFPS < 0.001 || fFPS > 100.0)
            {
                bValidArgs = false;
                cout << "FPS should be in the range [0.001, 100.0]" << endl;
                break;
            }
            iArgsCount++;
        }
#if 0
        else if( sCommand.compare( "-i" ) == 0 )
        {
            iImageType = pUtilities->convertStringToInteger( string( argv[iArgsCount+1]) );
            if( !( iImageType == MONOCHROME || iImageType == COLOR ) )
            {
                bValidArgs = false;
                cout << "Image type should be from the set {monochrome, color}" << endl;
                break;
            }
            iArgsCount++;
        }
#endif
        else if (sCommand.compare("-numframestoignore") == 0)
        {
            iNumFramesToIgnore = pUtilities->convertStringToInteger(
                    string(argv[iArgsCount + 1]));
            iArgsCount++;
        }
        else if (sCommand.compare("-numframestograb") == 0)
        {
            iNumFramesToGrab = pUtilities->convertStringToInteger(
                    string(argv[iArgsCount + 1]));
            iArgsCount++;
        }
        else if (sCommand.compare("-resizeimage") == 0)
        {
            bResizeImage = true;
            iResizedImgHeight = pUtilities->convertStringToInteger(
                    string(argv[iArgsCount + 1]));
            iArgsCount++;
        }
        else if (sCommand.compare("-usewholeimageasroi") == 0)
        {
            bUseWholeImageAsROI = true;
        }
        else if (sCommand.compare("-process") == 0)
        {
            bProcessFrames = true;
        }
        else if (sCommand.compare("-largefile") == 0)
        {
            bFileSizeGt2GB = true;
        }
        else if (sCommand.compare("-savevideo") == 0)
        {
            bSaveVideo = true;
        }
        else if (sCommand.compare("-skipfrozenframes") == 0)
        {
            bSkipFrozenFrames = true;
        }
        else if (sCommand.compare("-outputfolder") == 0)
        {
            sOutputFolder = string(argv[iArgsCount + 1]);
            iArgsCount++;
        }
        else if (sCommand.compare("-c") == 0)
        {
            sConfigFile = string(argv[iArgsCount + 1]);
            // Check if the specified file is a valid file
            bool bIsFile = false;
            bool bIsFileOrFolder = pUtilities->getIsFileOrFolderPresent(
                    sConfigFile, bIsFile);
            if (!bIsFileOrFolder || !bIsFile)
            {
                bValidArgs = false;
                cout << "Invalid configuration file" << endl;
                break;
            }
            bSetConfigFile = true;
            iArgsCount++;
        }
        else if (sCommand.compare("-setregionsfromconfigfile") == 0)
        {
            bSetRegionsFromConfigFile = true;
        }
        else if (sCommand.compare("-dispimgcapture") == 0)
        {
            bDispImgCapture = true;
        }
        else if (sCommand.compare("-dispimgresults") == 0)
        {
            bDispImgResults = true;
        }
        else if (sCommand.compare("-writetimestamponframe") == 0)
        {
            bWriteTimestampOnFrame = true;
        }
        else if (sCommand.compare("-enablecircbuf") == 0)
        {
            bEnableCircBuf = true;
        }
        else if (sCommand.compare("-numframesincircbuf") == 0)
        {
            iNumFramesInCircBuf = pUtilities->convertStringToInteger(
                    string(argv[iArgsCount + 1]));
            iArgsCount++;
        }
        else if (sCommand.compare("-setthreadpriority") == 0)
        {
            iPriorityValue = pUtilities->convertStringToInteger(
                    string(argv[iArgsCount + 1]));
            bIsSetThreadPriority = true;
            iArgsCount++;
        }
        else if (sCommand.compare("-debugmode") == 0)
        {
            int iDebugMode = pUtilities->convertStringToInteger(
                    string(argv[iArgsCount + 1]));
            debugMode = (teDebugMode)iDebugMode;
            iArgsCount++;
        }
        else
        {
            cout << "Invalid option: " << sCommand << endl;
            bValidArgs = false;
            break;
        }
        iArgsCount++;
    }

    // == Additional checks on the inputs ==
    if (bSetRegionsFromConfigFile && !bSetConfigFile)
    {
        bValidArgs = false;
        cout
                << "Setting the regions from config file requires setting the config file."
                << endl;
    }

    if (sCamera1.compare(string("")) == 0)
    {
        bValidArgs = false;
        cout << "Please specify necessary information for camera 1." << endl;
    }

    if (sCamera1.compare(string("usb")) == 0)
    {
        if (iCamera1No < 0 || iCamera1No > 10)
        {
            bValidArgs = false;
            cout << "Please provide a valid camera number for camera 1."
                    << endl;
        }
    }

    bUseCameraEmulator = sCamera1.compare(string("emulator")) == 0;
    if (bUseCameraEmulator || sCamera1.compare(string("ipcam")) == 0)
    {
        if (sVideo1Path.length() < 5)
        {
            bValidArgs = false;
            cout
                    << "Please provide a valid path for video file or video stream of camera 1."
                    << endl;
        }
    }

    if (bUseCameraEmulator)
    {
        if (iCaptureMode1 < 1)
        {
            bValidArgs = false;
            cout << "Please specify the capture mode for camera 1: emulator."
                    << endl
                    << "Capture mode should be set from the set {video = 1, image_list = 2, image = 3} "
                    << endl;
        }
        if (sVideo1Path.length() < 5)
        {
            bValidArgs = false;
            cout
                    << "Please provide a valid path for video file or video stream of camera 1."
                    << endl;
        }
    }

    if (sCamera1.length() > 0 && bUseCameraEmulator)
    {
        if (iCameraResWidth < 0)
        {
            bValidArgs = false;
            cout << "Please specify width of the image for camera 1." << endl;
        }
        if (iCameraResHeight < 0)
        {
            bValidArgs = false;
            cout << "Please specify height of the image for camera 1." << endl;
        }
    }

    if (bResizeImage)
    {
        if (iResizedImgHeight < 10 || iResizedImgHeight > 1944
                || iResizedImgHeight > iCameraResHeight)
        {
            bValidArgs = false;
            cout << "Height of resized image is invalid." << iResizedImgHeight
                    << endl;
        }
        if (iResizedImgHeight >= iCameraResHeight)
        {
            bValidArgs = false;
            cout
                    << "Image not to be resized as specified image height for resize"
                    << " >= height of original image." << endl;
        }
    }

    if (bStreamResizedImg && !bResizeImage)
    {
        bValidArgs = false;
        cout
                << "Streaming resized image is valid only if image is to be resized."
                << endl << "\tSpecify -resizeimage option to resize image."
                << endl;
    }

    if (argc < 3 || !bValidArgs)
    {
        cout << "Input command: " << endl;
        iArgsCount = 1;
        while (iArgsCount < argc)
        {
            cout << string(argv[iArgsCount++]) << " ";
        }
        cout << "\n\n<Usage>" << endl
                << "(1) ./AppTest -c1 ipcam -w1 640 -h1 480 -n1 0 " << endl
                << "\t-p1 http://10.0.6.2/now.jpg?snap=spush&pragma=motion"
                << endl << "\t-c1 ipcam: Sets camera 1 to ipcam" << endl
                << "\t-n1 0: The camera no. of camera 1 is set to 0" << endl
                << "(2) ./AppTest -c1 usb -w1 640 -h1 480 -n1 0 " << endl
                << "(3) ./AppTest -c1 ipcam -w1 640 -h1 480 -p1 rtsp://192.168.0.202/now.mp4"
                << endl
                << "\t-n1 0: cam no. of camera 1. If usb, video0 will be used"
                << endl << " -savevideo : Save video file." << endl
                << "\t-c <config file> : Specify config file." << endl
                << "\t-setregionsfromconfigfile : Set regions from config file"
                << endl << "\t-outputfolder Specify output folder" << endl
                << "\t-usewholeimageasroi: Specify use of whole image as ROI"
                << endl << "\t-process : Process images" << endl
                << "\t-numframestoignore <num>: No. of frames to ignore" << endl
                << "(4) ./AppTest -c1 emulator -w1 640 -h1 480 -cm1 1 -p1 ../../videos/Hua-Hin_01.avi"
                << endl << "\t-c1 emulator: Sets camera 1 to emulator." << endl
                << "\t-cm1 1: Sets capture mode for camera 1 to VIDEO" << endl
                << "\t-p1 ../../videos/Hua-Hin_01.avi: Sets the path of the video file."
                << endl << endl
                << "(5) ./AppTest -c1 emulator -w1 640 -h1 480 -cm1 2 -p1 img_list_folder"
                << endl
                << "\t-cm1 2: Sets capture mode for camera 1 to IMAGE_LIST_FOLDER."
                << endl
                << "\t-p1 img_list_folder: Sets the image list folder to specified directory."
                << endl << endl
                << "(6) ./AppTest -c1 emulator -w1 640 -h1 480 -cm1 3 -p1 image_file"
                << endl << "\t-cm1 2: Sets capture mode for camera 1 to IMAGE."
                << endl
                << "\t-p1 image: Sets the image to specified image file."
                << endl << endl << "(7) ./AppTest -c1 emulator -w1 640 -h1 480 "
                << "--cm1 1 p1 ../../videos/Hua-Hin_01.avi -process "
                << " -savevideo -c ../ROI.config -setregionsfromconfigfile"
                << endl << "\t-process : Process images" << endl
                << "\t-fgparams <file>: The FrameGrabber parameter file" << endl
                << "\t-savevideo : Save video using images captured from camera 1"
                << endl << "\t-c config_file : Specify config file" << endl
                << "\t-setregionsfromconfigfile : Set region from config file."
                << endl
                << "\t-writetimestamponframe: Enable writing timestamp on frame."
                << endl
                << "\t-enablecircbuf: Enable using circular buffer for capturing all frames."
                << endl
                << "\t-numframesincircbuf <num>: No. of frames in buffer in the range [10, 120]"
                << endl
                << "\t-setthreadpriority <num>: Thread priority value from in the range [1 99]"
                << "\t\tNOTE: Setting thread priority requires sudo access."
                << endl << endl << "--- Additional options for debug mode ---"
                << endl
                << "\tUse -debugmode <0 | 1 | 2> :Debug mode <NO_DEBUG | INFO | VERBOSE>"
                << endl
                << "\t-largefile : Specify that the video file size is greater than 2GB"
                << endl
                << "\t-resizeimage iResizedImgHeight: Resize image with height "
                << "range [10,1944] for processing " << endl
                << "\t-streamresizedimg : Stream resized image." << endl
                << "\t-numframestoignore : Number of frames to ignore from processing/grabbing"
                << endl << "\t-proxy : Specify the proxy string if needed."
                << "\t\t<usage> -proxy \":http-proxy=http://proxy-host:proxy-port/\""
                << endl;
        cout << "\tEnable displaying intermediate images :" << endl;
        cout << "\t\t[-dispimgcapture -dispimgresults ]" << endl;
        return 0;
    }

    // Check if the specified folder is a valid folder
    bool bIsFile = false;
    bool bIsFileOrFolder = pUtilities->getIsFileOrFolderPresent(sOutputFolder,
            bIsFile);
    if (!bIsFileOrFolder || bIsFile)
    {
        cout << "Invalid output folder" << endl;
        cout << "Specified output folder is not present." << endl
                << "Creating the specified output folder " << sOutputFolder
                << endl;
        pLoggerTest->createFolder(sOutputFolder);
    }

    // Set output folder
    pLoggerTest->setOutputFolder((char*)string(PROJECT_FOLDER).c_str(),
            (char*)string(sOutputFolder).c_str());
    cout << "System output folder: " << sOutputFolder << endl;
    String sCurOutputFolder = pLoggerTest->getOutputFolder();
    cout << "Current output folder: " << sCurOutputFolder << endl << endl;
    pLoggerTest->writeToFile(FILE_LOG, sArguments);

    // Displaying parameters in console
    cout << "Parameters specified by the user" << endl;
    cout << "No. of frames to grab: " << iNumFramesToGrab << endl;
    cout << "Camera 1: " << sCamera1 << endl;
    cout << "Image type: " << pchArrImageType[iImageType] << endl;
    cout << "Large file: " << pchArrBoolean[bFileSizeGt2GB] << endl;
    cout << "Camera No.: " << iCamera1No << endl;
    cout << "Video Path: " << sVideo1Path << endl;
    cout << "Capture mode for camera 1: " << pchArrCaptureMode[iCaptureMode1]
            << endl;
    cout << "Image resolution: " << iCameraResWidth << " x " << iCameraResHeight
            << endl;
    cout << "Resize image: " << pchArrBoolean[bResizeImage] << endl;
    if (bResizeImage)
    {
        cout << "Resized image height: " << iResizedImgHeight << endl;
        cout << "Stream resized image: " << pchArrBoolean[bStreamResizedImg]
                << endl;
    }
    cout << "Process frames: " << pchArrBoolean[bProcessFrames] << endl;
    cout << "Use whole image as ROI: " << pchArrBoolean[bUseWholeImageAsROI]
            << endl;
    cout << "Save video: " << pchArrBoolean[bSaveVideo] << endl;
    cout << "Frame rate of capture: " << fFPS << " fps" << endl;
    if (bSetConfigFile)
    {
        cout << "Config file: " << sConfigFile << endl;
        cout << "Set regions from config file: "
                << pchArrBoolean[bSetRegionsFromConfigFile] << endl;
    }
    cout << "Output folder: " << sOutputFolder << endl;

    if (bEnableCircBuf)
    {
        cout << "Capture buffer enabled: " << pchArrBoolean[bEnableCircBuf]
                << endl;
        cout << "Num frames in circular buffer: " << iNumFramesInCircBuf
                << endl;
    }

    if (bIsSetProxy) cout << "Proxy set to: " << sProxy << endl;
    if (bEnableCircBuf) cout << "Circular buffer enabled." << endl;
    if (bIsSetThreadPriority) cout << "Thread priority set to "
            << iPriorityValue << endl;
    cout << "Debug Mode: " << (int)debugMode;
    cout << endl;

    cout << "Input arguments ................................ [ OK ]" << endl;
    iArgsCount = 1;
    while (iArgsCount < argc)
    {
        cout << string(argv[iArgsCount++]) << " ";
    }
    if (ENABLE_PAUSE)
    {
        cout << "Press ENTER to continue..." << endl;
        cin.get();
    }
    cout << endl;

    // == Set parameters to FrameGrabber class ==

    // Camera Parameters
    FrameGrabber::setCaptureMode(iCaptureMode1);
    FrameGrabber::setImageType(iImageType);
    FrameGrabber::setFileSizeGt2GB(bFileSizeGt2GB);
    FrameGrabber::setCameraType(sCamera1);
    FrameGrabber::setCameraNo(iCamera1No);
    FrameGrabber::setVideoPath(sVideo1Path);
    FrameGrabber::setFPS(fFPS);
    FrameGrabber::setCameraResolution(iCameraResWidth, iCameraResHeight);
    FrameGrabber::setResizeImage(bResizeImage, iResizedImgHeight,
            bStreamResizedImg);

    FrameGrabber::setNumFramesToGrab(iNumFramesToGrab);
    FrameGrabber::setNumFramesToIgnore(iNumFramesToIgnore);
    FrameGrabber::skipFrozenFrames(bSkipFrozenFrames);
    FrameGrabber::setSaveVideo(bSaveVideo);
    FrameGrabber::setWriteTimestampOnFrame(bWriteTimestampOnFrame);

    // Proxy
    if (bIsSetProxy) IQEyeVideoStreamer::setProxy(sProxy);

    // Circ buffer
    if (bEnableCircBuf)
    {
        FrameGrabber::setEnableCircBuf(bEnableCircBuf);
        FrameGrabber::setNumFramesInCircBuf(iNumFramesInCircBuf);
    }
    if (bIsSetThreadPriority) FrameGrabber::setThreadPriority(iPriorityValue);
    FrameGrabber::setDebugMode(debugMode);

    // Start the application
    FrameGrabber *pFrameGrabber = new FrameGrabber();

    // == Set parameters to App class ==
    App::setDebugMode(debugMode);
    App::setCamera1Resolution(iCameraResWidth, iCameraResHeight);
    App::setResizeImage(bResizeImage, iResizedImgHeight,
            bStreamResizedImg);
    if (bSetConfigFile)
    {
        App::setConfigFile((char*)string(sConfigFile).c_str());
        App::setRegionsFromConfigFile(bSetRegionsFromConfigFile);
    }
    if (bUseWholeImageAsROI) App::setUseWholeImageAsROI(bUseWholeImageAsROI);
    App::setDispImgCapture(bDispImgCapture);
    App::setProcessFrames(bProcessFrames);
    App::setDispImgResults(bDispImgResults);
    if (bResizeImage)
    {
        if (iResizedImgHeight >= iCameraResHeight)
        {
            cout << "Specified resize image height >= "
                    << "height of original image." << endl;
            bResizeImage = false;
        }
    }

    // Start the application
    App *pApp = new App();
    pApp->setFrameGrabberObj(pFrameGrabber);
    pApp->startApp();

    // Clean up
    if (pApp) delete pApp;
    if (pFrameGrabber) delete pFrameGrabber;
    if (pUtilities) delete pUtilities;
    pLoggerTest = NULL;
    pErrorHandlerTest = NULL;

    cout << "Program terminated." << endl;
    //kill(getpid(), SIGKILL);
    return 0;
}

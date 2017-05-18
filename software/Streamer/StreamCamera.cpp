#include <zmq.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include "VideoStreamerVlc.h"
#include <string.h>

using namespace cv;
using namespace std;
VideoStreamerVlc Streamer;
boost::mutex _messageStatus;

string sMessage;
bool bGotNewMessage = false;
bool bView = false;

struct sCamStreams {
  Mat mImg;
  string sName;
  string sIp;
  bool init;
  VideoStreamerVlc *vlcStreamer;
  VideoCapture vcCapture;
} ;

vector <sCamStreams> vsCamStreams;


void msgInfo(string sMsg)
{
  stringstream ss(sMsg);
  istream_iterator<string> begin(ss);
  istream_iterator<string> end;
  vector<string> vstrings(begin, end);
  if (vstrings[0] == "S")
  {
    sCamStreams sCamStruct;
    sCamStruct.sName = vstrings[2];
    sCamStruct.sIp = vstrings[1];
    sCamStruct.init = false;
    vsCamStreams.push_back(sCamStruct);
  }
  if (vstrings[0] == "E")
  {
    int index;
    for(int i = 0; i < vsCamStreams.size(); i++)
    {
      if(vsCamStreams[i].sName == vstrings[2])
      {
	index = i;
	break;
      }
    }
    if(bView)
    {
      destroyWindow(vsCamStreams[index].sName);
    }
    vsCamStreams.erase(vsCamStreams.begin()+index);
  }
   if (vstrings[0] == "I")
   {
     Mat mImgResize;
     Mat mSaveImg;
     VideoCapture Capture;
     Capture.open(vstrings[1]);
     boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
     Capture.read(mSaveImg);
     boost::this_thread::sleep(boost::posix_time::milliseconds(200));
     Capture.read(mSaveImg);
     boost::this_thread::sleep(boost::posix_time::milliseconds(200));
     resize(mSaveImg, mImgResize , Size(640,480));
     string sPath = "/home/theekshana/Documents/web16-03/ParkingWebApp/public/" + vstrings[2] + ".jpg";
     cout << "FIle Saved " << sPath << endl;
     imwrite(sPath, mImgResize);
   }
}

void zmqThread()
{
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REP);
  socket.bind ("tcp://*:5555");
  cout << "Started ZMQ Thread tcp://*:5555" << endl;
  while(1)
  {
     zmq::message_t request;
     socket.recv (&request);
     boost::this_thread::sleep(boost::posix_time::milliseconds(5));
     _messageStatus.lock();
     {
	bGotNewMessage = true;
	sMessage = (char*) request.data();
     }
     boost::this_thread::sleep(boost::posix_time::milliseconds(5));
     _messageStatus.unlock();
     std::cout << "Message Recieved" << std::endl;
     sleep(1);
     zmq::message_t reply (5);
     memcpy (reply.data (), "1", 1);
     socket.send (reply);
  }
}
Mat mImgResize;
int main () {
  boost::thread(boost::bind(zmqThread));
  bool bNewmsg;
  string sMsg;
  //msgInfo("I http://10.32.20.201/now.jpg?snap=spush?dummy=0 1");

  while (true) {
     boost::this_thread::sleep(boost::posix_time::milliseconds(5));
      _messageStatus.lock();
     {
       	bNewmsg = bGotNewMessage;
        if(bNewmsg)
        {
          sMsg = sMessage;
          bGotNewMessage = false;
	  //msgInfo("S http://10.32.20.201/now.jpg?snap=spush?dummy=0 testcam");
	  msgInfo(sMsg);
        }
     }
     _messageStatus.unlock();  
    boost::this_thread::sleep(boost::posix_time::milliseconds(5));
    
    for(int i = 0; i < vsCamStreams.size(); i++)
    {
      mImgResize.release();
      if(vsCamStreams[i].init == false)
      {
	cout << "Starting Capture " << vsCamStreams[i].sName << endl;
	vsCamStreams[i].vcCapture.open(vsCamStreams[i].sIp); 
	cout << "Starting Stream " << vsCamStreams[i].sName << endl;
	vsCamStreams[i].vlcStreamer = new  VideoStreamerVlc();
	vsCamStreams[i].vlcStreamer->setFrameInformation(640, 480, 3, 30, "8080", vsCamStreams[i].sName);
	vsCamStreams[i].vlcStreamer->setCompressionMode(USE_WEB);
        vsCamStreams[i].vlcStreamer->InitializeStreamer(0, "1");
	if(bView)
	{
	  namedWindow(vsCamStreams[i].sName, CV_WINDOW_AUTOSIZE);
	}
	vsCamStreams[i].init = true;
      }
      else
      {
	vsCamStreams[i].vcCapture.read(vsCamStreams[i].mImg);
	resize(vsCamStreams[i].mImg, mImgResize , Size(640,480));
	vsCamStreams[i].vlcStreamer->addFrameForStream(mImgResize);
	if(bView)
	{
	  imshow(vsCamStreams[i].sName, mImgResize);
	}      
      }
    }
    cvWaitKey(10);
  }
  return 0;
}

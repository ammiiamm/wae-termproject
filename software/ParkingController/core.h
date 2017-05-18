#ifndef CORE_H_
#define CORE_H_

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
#include <pqxx/pqxx>
#include "VideoStreamerVlc.h"

using namespace cv;
using namespace std;
using namespace pqxx;

class core
{
public:
  // Constructor, destructor
  core();
  virtual ~core();
  void init(string username,string password, string dbname, string camIp, string sConfigName, string szone);
  void testDataBase();
  void changeFreeSpace(int sVal);
  void process();
private:
  Mat mImgResize;
  Mat mImg;
  string sUserName;
  string sPassWord;
  string sDbName;
  string sCamIp;
  string sConfigName;
  string sZone;
  bool bInit;
  VideoStreamerVlc *vlcStreamer;
  VideoCapture vcCapture;
  int counter;
};
#endif /* CORE_H_ */

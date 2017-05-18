#include <zmq.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/io/io.hpp>
#include <boost/geometry/arithmetic/arithmetic.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <string.h>

using namespace cv;
using namespace std;
string task;
string values;
string sdivide;

Point2d getImageCordinate(Mat H, Point2d x)
{
    Mat HH = H.inv();
    Mat C2 = (Mat_<double>(3, 1) << x.x, x.y, 1);
    Mat cal2 = HH * C2;
    return Point2d(cal2.at<double_t>(0, 0) / cal2.at<double_t>(2, 0),
            cal2.at<double_t>(1, 0) / cal2.at<double_t>(2, 0));
}


float slopeBetweenTwoPoints(Point2d a, Point2d b )
{
    float slope;

    if (abs(b.x - a.x) != 0)
    {
        slope = fabs(b.y - a.y) / fabs(b.x - a.x);
    }
    else
    {
        slope = 10000;
    }

    return slope;
}
vector <Point2d> linedivide(Point2d start, Point2d end,int divider)
{
  vector <Point2d> result;
  float y = start.y;
  float sloap = slopeBetweenTwoPoints(start,end);
  float c = start.y - (sloap*start.x);
  if (sloap >= 1)
  {
    float differance = (end.y -start.y)/divider;
     for(int i = 0; i < divider ; i++)
     {
       float x = (y - c)/sloap;
       result.push_back(Point2d(x,y));
       y = y + differance;
     }
     result.push_back(Point2d(end));
  }
  else
  {
    
  }
  return result;
}
int main(int argc, char* argv[])
{
    int divide;
    task = argv[1];
    sdivide = argv[2];
    values = argv[3];
    divide = atoi(sdivide.c_str());
    if(task == "1")
    {
      
    }
    if(task == "2")
    {
    std::vector<float> vect;

    std::stringstream ss(values);

    float i;

    while (ss >> i)
    {
        vect.push_back(i);

        if (ss.peek() == ',')
        ss.ignore();
    }

      vector <Point2d> world;
      vector <Point2d> image;
      for(int i = 0; i < 8 ; i=i+2)
      {
	image.push_back(Point2d(vect[i],vect[i+1]));
      }
      for(int i = 8; i < 16 ; i=i+2)
      {
	world.push_back(Point2d(vect[i],vect[i+1]));
      }
      for (int i = 0; i < world.size(); i++)
      {
	//cout << world[i] << endl;
      }
      Mat H = findHomography(image,world,CV_RANSAC);
      vector <Point2d> worldDivided;
      double x= 0;
      x = world[0].x;
      for(int i = 0; i < ((divide+1)) ; i++)
      {
	double length = world[1].x - world[0].x ;
        worldDivided.push_back(Point2d(x,world[0].y));
	x = x + (length/divide);
      }
      x = world[3].x;
      for(int i = 0; i < ((divide+1)) ; i++)
      {
	double length = world[2].x - world[3].x ;
        worldDivided.push_back(Point2d(x,world[2].y));
	x = x + (length/divide);      
      }
      for (int i = 0; i < worldDivided.size(); i++)
      {
	Point2d newp = getImageCordinate(H,worldDivided[i]);
	cout << round(newp.x) << "," << round(newp.y) << ",";
      }
     }
     //vector <Point2d> result = linedivide(Point2d(0,0),Point2d(10,15),5);
  return 0;
}

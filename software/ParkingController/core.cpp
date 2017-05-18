#include "core.h"

core::core()
{

}

core::~core()
{

}
void core::init(string username,string password, string dbname, string camIp, string sConfigName, string szone)
{
  counter = 0;
  sUserName = username;
  sPassWord = password;
  sDbName = dbname;
  sCamIp = camIp;
  sConfigName = sConfigName;
  sZone = szone;
  vcCapture.open(camIp);
  vlcStreamer = new  VideoStreamerVlc();
  vlcStreamer->setFrameInformation(640, 480, 3, 30, "8081", sConfigName);
  vlcStreamer->setCompressionMode(USE_WEB);
  vlcStreamer->InitializeStreamer(0, "1");
}
void core::testDataBase()
{
   try{
      connection C("dbname="+ sDbName +" user="+ sUserName +" password="+sPassWord+" \
      hostaddr=127.0.0.1 port=5432");
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
      }    
     
      C.disconnect ();
   }catch (const std::exception &e){
      cerr << e.what() << std::endl;
   }
}

void core::changeFreeSpace(int sVal)
{
     char * sql;

   try{
      connection C("dbname="+ sDbName +" user="+ sUserName +" password="+sPassWord+" \
      hostaddr=127.0.0.1 port=5432");
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
      }    
       string test = "INSERT INTO PARKINGTOTALS (ZONE_ID,FREE,created_at,updated_at) VALUES ("+ sZone +","+ to_string(sVal) +",now(),now());";
       //strcpy(sql, test.c_str());
       sql = &test[0];
       cout << test << endl;
       cout << sql << endl;
      work W(C);
      W.exec( sql );
      W.commit();
      cout << "Records created successfully" << endl;
      C.disconnect ();
   }catch (const std::exception &e){
      cerr << e.what() << std::endl;
   }
}
void core::process()
{
  counter++;
  if(counter > 200)
  {
    int v1 = rand() % 10; 
    changeFreeSpace(v1);
    counter =0;
  }
   vcCapture.read(mImg);
   resize(mImg, mImgResize , Size(640,480));
   circle(mImgResize,Point(100,100), 30, Scalar(0,200,20));
   vlcStreamer->addFrameForStream(mImgResize);
}

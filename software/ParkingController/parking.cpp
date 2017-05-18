#include <zmq.hpp>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <core.h>
using namespace std;
boost::mutex _messageStatus;
string sMessage;
bool bGotNewMessage = false;
bool bView = false;

struct sParking {
    string sName;
    core coreRun;
};
vector<sParking> vcParking;

void msgInfo(string sMsg)
{
    stringstream ss(sMsg);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> vstrings(begin, end);
    if (vstrings[0] == "S") {
        sParking park;
        park.sName = "";
        park.coreRun.init("web3", "Web3group", "ParkingWebApp_development", vstrings[3], vstrings[1], vstrings[2]);
	park.coreRun.testDataBase();
        vcParking.push_back(park);
    }
    if (vstrings[0] == "E") {
        int index;
        for (int i = 0; i < vcParking.size(); i++) {
            if (vcParking[i].sName == vstrings[2]) {
                index = i;
                break;
            }
        }
        if (bView) {
            destroyWindow(vcParking[index].sName);
        }
        vcParking.erase(vcParking.begin() + index);
    }
}

void zmqThread()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5556");
    cout << "Started ZMQ Thread tcp://*:5556" << endl;
    while (1) {
        zmq::message_t request;
        socket.recv(&request);
        boost::this_thread::sleep(boost::posix_time::milliseconds(5));
        _messageStatus.lock();
        {
            bGotNewMessage = true;
            sMessage = (char*)request.data();
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(5));
        _messageStatus.unlock();
        std::cout << "Message Recieved" << std::endl;
        sleep(1);
        zmq::message_t reply(5);
        memcpy(reply.data(), "1", 1);
        socket.send(reply);
    }
}

int main(int argc, char* argv[])
{
    boost::thread(boost::bind(zmqThread));
    bool bNewmsg;
    string sMsg;
    //msgInfo("S 1 6 http://10.32.20.201/now.jpg?snap=spush?dummy=0");

    while (true) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(5));
        _messageStatus.lock();
        {
            bNewmsg = bGotNewMessage;
            if (bNewmsg) {
                sMsg = sMessage;
                bGotNewMessage = false;
                //msgInfo("S http://10.32.20.201/now.jpg?snap=spush?dummy=0 testcam");
                msgInfo(sMsg);
            }
        }
        _messageStatus.unlock();
        boost::this_thread::sleep(boost::posix_time::milliseconds(5));
        for (int i = 0; i < vcParking.size(); i++) {
            vcParking[0].coreRun.process();
        }
        cvWaitKey(10);
    }
    return 0;
}
#include <iostream>
#include <cstring>
#include "server.h"
using namespace std;


void help()
{
    cout<<" --help        display this infomation"<<endl;
    cout<<" -p <port>     specify the port which you want to listen"<<endl;
}

int main(int argc, char **argv)
{
    if(argc==1 || argc>3) {
        help();
        return 0;
    }
    if(argc==2 && !strcmp(argv[1], "--help")) {
        help();
        return 0;
    }
    unsigned short port = 9200;
    if(argc==3 && !strcmp(argv[1], "-p")) {
        port = stoi(string(argv[2]));
    } else {
        cout<<"Wrong argument"<<endl;
        return 0;
    }
    // the only instance
    Server &serv = Server::GetInstance();
    // set listening port
    serv.SetListenPort(port);
    // set buffer size
    serv.SetBufferSize(MAX_BUFF_SIZE);
    // just run
    serv.Run();

    return 0;
}

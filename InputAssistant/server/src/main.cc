#include "../include/Configuration.h"
#include "../include/DictIndex.h"
#include "../include/DictProducer.h"
#include "../include/TestTcpServer.h"
#include "../include/MyDict.h"


#include <map>
#include <string>
#include <iterator>
#include <iostream>

using std::string;
using std::map;
using std::cout;
using std::endl;

using namespace tsk;


int main(){

	//Configuration conf;

	//cout << conf.getConfigMap().size() << endl;
	//map<string, string> m(conf.getConfigMap());
	//cout << m.size() << endl;
	//auto iter = m.begin();
	//++iter;
	//cout << iter->second << endl;
	//++iter;
	//select source file of english

	//预处理
	MyDict *mydict = MyDict::getInstance();

	//mydict->print();

	//cout << "after preprocessing()" << endl;
#if 1

	string confInfo("socket_ip");
	string ip = mydict->getPath(confInfo);
	cout << "local ip = " << ip << endl;
	confInfo = "socket_port";
	string port = mydict->getPath(confInfo);
	cout << "local port = " << port << endl;

	tsk::Threadpool threadpool(4, 10); 
	threadpool.start();

	
	gThreadpool = &threadpool;

	tsk::TcpServer server(ip, atoi(port.c_str()));


	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.setCloseCallback(onClose);

	server.start();

#endif

	return 0;
}


#include "../include/Configuration.h"
#include "../include/DictIndex.h"
#include "../include/DictProducer.h"
#include "../include/TestTcpServer.h"

#include <map>
#include <string>
#include <iterator>
#include <iostream>

using std::string;
using std::map;
using std::cout;
using std::endl;

using namespace tsk;

void preprocessing(Configuration &);

int main(){
	Configuration conf;
	//cout << conf.getConfigMap().size() << endl;
	//map<string, string> m(conf.getConfigMap());
	//cout << m.size() << endl;
	//auto iter = m.begin();
	//++iter;
	//cout << iter->second << endl;
	//++iter;
	//select source file of english

	//init index file and directory file
	preprocessing(conf);

	string confInfo = "socket_ip";
	string ip = conf.getPath(confInfo);
	confInfo = "socket_port";
	string port = conf.getPath(confInfo);

	tsk::Threadpool threadpool(4, 10); 
	threadpool.start();

	
	gThreadpool = &threadpool;

	tsk::TcpServer server(ip, atoi(port.c_str()));

	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.setCloseCallback(onClose);

	server.start();


	return 0;
}

void preprocessing(Configuration &conf){
	string confInfo("SrcEnglish");
	//cout << confInfo << endl;

	string path = conf.getPath(confInfo);
	DictProducer dir(path);
	dir.build_dict();

	//write IndexLinkEnglish file
	confInfo = "IndexLinkEnglish";
	path = conf.getPath(confInfo);
	dir.store_dict(path.c_str());

	//build english index file
	DictIndex idx(dir);
	confInfo = "IndexEnglish";
	string indexfile = conf.getPath(confInfo);
	idx.bulidIndex(path, indexfile);
	dir.show_files();
}

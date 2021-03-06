 ///
 /// @file    TestAcceptor.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-05-07 15:42:14
 ///
#ifndef __TESTTCPSERVER_H__
#define __TESTTCPSERVER_H__
 
#include "Threadpool.h"
#include "TcpServer.h"
#include "MyDict.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <utility>

using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::vector;


extern tsk::Threadpool * gThreadpool;


class Task
{
public:
	Task(const string & msg,
		 const tsk::TcpConnectionPtr & conn)
	: _msg(msg)
	, _conn(conn)
	{}

	//运行在线程池的某一个子线程中
	void process();
	//{
	//	string s1 = _msg;

	//	_conn->sendInLoop(s1);
	//}

	vector<pair<string, int>> compute(string &, tsk::MyDict *);
private:
	string _msg;
	tsk::TcpConnectionPtr _conn;
};
 
//回调函数体现了扩展性
void onConnection(const tsk::TcpConnectionPtr & conn);
//{
//	cout << conn->toString() << " has connected!" << endl;
//	conn->send("welcome to server.");
//}

void onMessage(const tsk::TcpConnectionPtr & conn);
//{
//	//cout << "onMessage...." << endl;
//	string msg = conn->receive();
//	cout << msg << endl;
//	//cout << "receive end" << endl;
//	//cout << ">> receive msg from client: " << msg << endl;
//	//业务逻辑的处理要交给线程池处理
//	//decode
//	//compute
//	//encode
//	//::sleep(2);//碰到需要长时间的处理时，响应速度会降下来
//	//conn->send(msg);
//	Task task(msg, conn);
//
//	gThreadpool->addTask(std::bind(&Task::process, task));
//}

void onClose(const tsk::TcpConnectionPtr & conn);
//{
//	//cout << "onClose...." << endl;
//	cout << conn->toString() << " has closed!" << endl;
//}

//using namespace tsk;
//class EchoServer
//{
//public:
//	void onConnection(const TcpConnectionPtr & conn);
//	void onMessage(const TcpConnectionPtr & conn);
//	void onClose(const TcpConnectionPtr & conn);
//
//	void process();//业务逻辑的处理
//
//private:
//	Threadpool _threadpool;
//	TcpServer _server;
//};


//int main(int argc, char *argv[])
//{
//	if(argc != 3){
//		cout << "number of argument error!" << endl;
//		return -1;
//	}
//	tsk::Threadpool threadpool(4, 10); 
//	threadpool.start();
//	
//	gThreadpool = &threadpool;
//
//	tsk::TcpServer server(argv[1], atoi(argv[2]));
//
//	server.setConnectionCallback(onConnection);
//	server.setMessageCallback(onMessage);
//	server.setCloseCallback(onClose);
//
//	server.start();
//
//	return 0;
//}


#endif

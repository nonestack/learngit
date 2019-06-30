 ///
 /// @file    TestAcceptor.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-05-07 15:42:14
 ///
 
#include "Threadpool.h"
#include "TcpServer.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;
using std::string;

tsk::Threadpool * gThreadpool = nullptr;

class Task
{
public:
	Task(const string & msg,
		 const tsk::TcpConnectionPtr & conn)
	: _msg(msg)
	, _conn(conn)
	{}

	//运行在线程池的某一个子线程中
	void process()
	{
		//decode
		//compute
		//encode
		cout << "MyTask::process()" << endl;
		cout << "client information = "
			 << _msg << endl;
		string response = _msg;//要返回给客户端的消息
		//_conn->send(response);//由线程池的线程(计算线程)完成数据的发送,在设计上来说，是不合理的
							  //数据发送的工作要交还给IO线程(Reactor所在的线程)完成
							  //将send的函数的执行延迟到IO线程取操作
		cout << "------------------------------------------" << endl;
		_conn->send("hahahaha");
		//_conn->sendInLoop(response);
		cout << "send()" << endl;
		cout << "------------------------------------------" << endl;
	}
private:
	string _msg;
	tsk::TcpConnectionPtr _conn;
};
 
//回调函数体现了扩展性
void onConnection(const tsk::TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has connected!" << endl;
	conn->send("welcome to server.");
}

void onMessage(const tsk::TcpConnectionPtr & conn)
{
	//cout << "onMessage...." << endl;
	string msg = conn->receive();
	cout << msg << endl;
	//cout << "receive end" << endl;
	//cout << ">> receive msg from client: " << msg << endl;
	//业务逻辑的处理要交给线程池处理
	//decode
	//compute
	//encode
	//::sleep(2);//碰到需要长时间的处理时，响应速度会降下来
	//conn->send(msg);
	Task task(msg, conn);

	gThreadpool->addTask(std::bind(&Task::process, task));
}

void onClose(const tsk::TcpConnectionPtr & conn)
{
	//cout << "onClose...." << endl;
	cout << conn->toString() << " has closed!" << endl;
}

using namespace tsk;
class EchoServer
{
public:
	void onConnection(const TcpConnectionPtr & conn);
	void onMessage(const TcpConnectionPtr & conn);
	void onClose(const TcpConnectionPtr & conn);

	void process();//业务逻辑的处理

private:
	Threadpool _threadpool;
	TcpServer _server;
};

#if 0

int main(int argc, char *argv[])
{
	if(argc != 3){
		cout << "number of argument error!" << endl;
		return -1;
	}
	tsk::Threadpool threadpool(4, 10); 
	threadpool.start();
	
	gThreadpool = &threadpool;

	tsk::TcpServer server(argv[1], atoi(argv[2]));

	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.setCloseCallback(onClose);

	server.start();

	return 0;
}

#endif

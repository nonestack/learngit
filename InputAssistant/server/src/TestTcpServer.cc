#include "../include/TestTcpServer.h"
#include "../include/LevenshteinDistance.h"

#include <set>
#include <utility>
#include <vector>
#include <algorithm>
#include <string>

using std::set;
using std::make_pair;
using std::sort;

tsk::Threadpool *gThreadpool = nullptr;


void Task::process(){
	//decode
	tsk::MyDict *mydict = tsk::MyDict::getInstance();
	//compute
	//cout << "Task::process() : _msg = " << _msg << endl;
	auto minDistance = compute(_msg, mydict);
	//encode
	//string response = _msg;//要返回给客户端的消息
	//_conn->send(response);//由线程池的线程(计算线程)完成数据的发送,在设计上来说，是不合理的
						  //数据发送的工作要交还给IO线程(Reactor所在的线程)完成
						  //将send的函数的执行延迟到IO线程取操作
	auto iter = minDistance.begin();
	for(; iter != minDistance.end(); ++iter){
		_conn->sendInLoop(iter->first);
	}
	//_conn->sendInLoop("hahahaha");
}

bool isShorter(const pair<string, int> &p1, const pair<string, int> &p2){
	return p1.second < p2.second;
}

void SortDistance(vector<pair<string, int>> &minNum){
	sort(minNum.begin(), minNum.end(), isShorter);
}


vector<pair<string, int>> Task::compute(string &msg, tsk::MyDict *mydict){
	vector<string> &linkEnglish = mydict->getLinkEnglish();

	cout << "linkEnglish size = " << linkEnglish.size() << endl;
	vector<pair<string, int>> minNum;
	int num;
	auto iter = linkEnglish.begin();
	int i;
	for(i = 0; i != 3; ++i){
		num = editDistance(msg.c_str(), msg.size(),
							iter->c_str(), iter->size());
		//cout << "num = " << num << endl;
		auto tmp = make_pair(*iter, num);
		++iter;
		minNum.push_back(tmp);
	}
	//cout << "minNum of size = " << minNum.size() << endl;


	//auto tmpMax = minNum.begin() + 2;
	for(; iter != linkEnglish.end(); ++iter){
		num = editDistance(msg.c_str(), msg.size(),
							iter->c_str(), iter->size());
		SortDistance(minNum);
		if(minNum.back().second > num){
			minNum.pop_back();
			minNum.push_back(make_pair(*iter, num));
		}
	}
	auto a = minNum.begin();
	for(; a != minNum.end(); ++a){
		cout << a->first << " : " << a->second << endl;
	}

	return vector<pair<string, int>>(minNum.begin(), minNum.end());
}

void onConnection(const tsk::TcpConnectionPtr &conn){
	cout << conn->toString() << " has connected!" << endl;
	conn->send("welcome to server.");
}

void onMessage(const tsk::TcpConnectionPtr &conn){
	//cout << "onMessage...." << endl;
	string msg = conn->receive();
	cout << msg << " of size = " << msg.size() << endl;
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

void onClose(const tsk::TcpConnectionPtr &conn){
	//cout << "onClose...." << endl;
	cout << conn->toString() << " has closed!" << endl;
}

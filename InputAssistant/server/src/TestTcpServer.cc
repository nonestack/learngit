#include "../include/TestTcpServer.h"
#include "../include/LevenshteinDistance.h"

#include <set>
#include <utility>

using std::set;
using std::pair;

void Task::process(){
		//decode
	tsk::MyDict *mydict = tsk::MyDict::getInstance();
		//compute
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

pair<string, int> MaxDistance(map<string, int> &minNum){
	auto iter = minNum.begin();
	string maxStr = iter->first;
	int maxNum = iter->second;

	++iter;
	for(; iter != minNum.end(); ++iter){
		if(maxNum > iter->second){
			maxStr = iter->first;
			maxNum = iter->second;
		}
	}
	return std::make_pair(maxStr, maxNum);
}


map<string, int> Task::compute(string &msg, tsk::MyDict *mydict){
	vector<string> &linkEnglish = mydict->getLinkEnglish();

	map<string, int> minNum;
	int num;
	auto iter = linkEnglish.begin();
	for(; iter != iter + 3; ++iter){
		num = editDistance(msg.c_str(), msg.size(),
							iter->c_str(), iter->size());
		auto tmp = make_pair(*iter, num);
		minNum.insert(tmp);
	}

	//auto tmpMax = minNum.begin() + 2;
	for(; iter != linkEnglish.end(); ++iter){
		num = editDistance(msg.c_str(), msg.size(),
							iter->c_str(), iter->size());
		auto tmpMax = MaxDistance(minNum);
		if(tmpMax.second > num){
			minNum.erase(tmpMax.first);
			minNum.insert(make_pair(*iter, num));
		}
	}

	return map<string, int>(minNum.begin(), minNum.end());

}

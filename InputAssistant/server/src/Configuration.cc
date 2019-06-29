#include "../include/Configuration.h"

#include <fstream>
#include <iostream>

using std::ifstream;
using std::cout;
using std::endl;

//using std::

namespace tsk{

Configuration::Configuration(const string &filepath)
:_filepath(filepath)
{
	cout << "path is " << filepath << endl;
	ifstream ifs;
	ifs.open(filepath);

	string s1, s2;
	while(ifs >> s1 >> s2){
		//cout << s1 << " : " << s2 << endl;
		_configMap.insert(make_pair(s1, s2));
	}

	//cout << "-----------------------------------------" << endl;
	//cout << "_configMap.size() = " << _configMap.size() << endl;

	//auto iter = _configMap.begin();
	//while(iter != _configMap.end()){
	//	//cout << iter->first << " : " << iter->second << endl;
	//	++iter;
	//}
	
		

	ifs.close();
}

map<string, string> &Configuration::getConfigMap(){
	return _configMap;
}

string Configuration::getPath(const string &confInfo){
	auto iter = _configMap.begin();
	while(iter != _configMap.end()){
		if(iter->first == confInfo){
			return iter->second;
		}
		++iter;
	}
	if(iter == _configMap.end()){
		cout << "no such information" << endl;
	}
	return string();
}

}//end of namespace tsk

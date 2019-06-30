#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <map>

using std::map;
using std::string;

namespace tsk{

class Configuration{
	public:
		Configuration(const string &filepath = string("/home/genius/Program/InputAssistant/server/conf/conf.txt"));	//构造函数
		map<string, string> &getConfigMap();	//获取存放配置文件内容的map
		string getPath(const string &confInfo);
	private:
		string _filepath;		//配置文件路径
		map<string, string> _configMap;	//配置文件内容
};

}//end of namespace tsk

#endif

#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include <string>
#include <vector>

using std::vector;
using std::string;

namespace tsk{

class SplicTool{
	public:
		SplicTool();	//构造函数
		virtual ~SplicTool();	//虚析构函数
		virtual vector<string> cut(const string &sentence) = 0; //分词函数，纯虚函数，提供接口
};

}//end of namespace tsk

#endif

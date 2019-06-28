#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include "SplitTool.h"

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

namespace tsk{

class DictProducer{
	public:
		DictProducer(const string &dir);	//构造函数
		//DictProducer(const string &dir, SplitTool *splitTool); //构造函数,专为中文处理

		void build_dict();	//创建英文字典
		//void build_cn_dict(); //创建中文字典
		void store_dict(const char *filepath);	//将词典写入文件
		void show_files() const;	//查看文件路径，作为测试用
		void show_dict() const;		//查看词典，作为测试用
	private:
		//string get_files();			//获取文件的绝对路径
		//void push_dict(const string &word);	//存储某个单词
	private:
		string _dirEn;	//english语料库存放的绝对路径
		string _dirZh;	//chinese语料库存放的绝对路径
		vector<string> _files;	//语料库文件的绝对路径集合
		map<string, int> _dict;	//配置文件内容
		//SplitTool *_splitTool;	//分词工具
};

}//end of namespace tsk

#endif

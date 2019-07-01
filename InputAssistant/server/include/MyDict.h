#ifndef __MYDICT_H__
#define __MYDICT_H__

#include "Configuration.h"

#include <vector>
#include <map>

using std::vector;
using std::map;

namespace tsk{

class MyDict{
	public:
		static MyDict *getInstance();
		static void destory();
		void print();
		vector<string> &getLinkEnglish(){
			return _linkEnglish;
		}

		string getPath(string &conf){
			_conf.getPath(conf);
		}
	private:
		MyDict();
		~MyDict() = default;
	private:
		static MyDict *_pInstance;
		Configuration _conf;
		vector<string> _linkEnglish;
		map<char, vector<int>> _dictEnglish;
};

}//end of namespace tsk

#endif
